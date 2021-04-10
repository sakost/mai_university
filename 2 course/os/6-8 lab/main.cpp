#include <lab.pb.h>

#include <thread>
#include <wait.h>
#include <cinttypes>
#include <vector>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <iostream>

#ifndef _WIN32

#include <unistd.h>

#else
#error "Windows is not supported"
#endif

#define ZMQ_BUILD_DRAFT_API

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "logger.hpp"

#define UNUSED(x) (void)(x)

using node_id_t = int64_t;

namespace fs = std::filesystem;

const node_id_t default_parent_nid = 0;


using namespace std;

loglevel_e loglevel = logERROR;

const int WAIT_TIME = 3000; // in milliseconds

class node_exception : exception {
public:
    explicit node_exception(Execute_ErrorType errorType) : exception() {
        m_error_type = errorType;
    }

    node_exception(Execute_ErrorType errorType, const string &msg) : node_exception(errorType) {
        m_error_type = errorType;
        this->msg = msg;
    }

    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        switch (m_error_type) {
            case Execute_ErrorType_CUSTOM:
            default:
                string ans = "Some unhandled error";
                if (!this->msg.empty()) {
                    ans = msg;
                }
                char *buf = new char[ans.size() + 1];
                memcpy(buf, ans.data(), sizeof(char) * (ans.size() + 1));
                return buf;
        }
    }

    Execute_ErrorType m_error_type;
protected:
    string msg;
};

static fs::path init_exe_path() noexcept {
#ifdef _WIN32
    wchar_t path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    return path;
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
#endif
}

static fs::path exepath = init_exe_path();

inline fs::path getexepath() {
    return exepath;
}

inline pid_t create_child(const string &path_to_exec, const string &nid, const string &parent) {
    if (!fs::exists(path_to_exec)) {
        throw runtime_error("path not exists");
    }
    pid_t pid = fork();
    if (pid > 0) return pid;
    if (pid < 0) throw runtime_error("fork error");
    execl(path_to_exec.c_str(), path_to_exec.c_str(), nid.c_str(), parent.c_str());
    throw runtime_error("execl error");
}

class logger_d {
public:
    explicit logger_d(const string &mes, loglevel_e lvl, const string &addon) {
        msg = mes;
        this->addon = addon;
        this->lvl = lvl;
        log_(lvl, addon) << msg;
    }

    explicit logger_d(const string &&mes, loglevel_e lvl, const string &addon) {
        msg = mes;
        this->lvl = lvl;
        this->addon = addon;
        log_(lvl, addon) << msg;
    }

    ~logger_d() {
        log_(lvl, addon) << "Ok(" << msg << ')';
    }

private:
    string msg, addon;
    loglevel_e lvl;
};

class Node {
public:
    static const size_t tree_base = 2; // count of children of each node

    enum class S_IDXS : int {
        FLOW,
        CALLBACK,
        SUBSCRIBE,
        UPSTREAM,
    };

    static string get_address(const string &str) {
        return "ipc://@" + str + ".ipc";
    }

    static string get_pub_sub_address(const string &str) {
        return get_address("socket_flow_" + str);
    }

    static string get_pub_sub_address(const string &&str) {
        return get_address("socket_flow_" + str);
    }

    static string get_pub_sub_address(const node_id_t &nodeId) {
        return get_pub_sub_address(::to_string(nodeId));
    }

    static string get_upstream_callback_address(const string &str) {
        return get_address("socket_callback_" + str);
    }

    static string get_upstream_callback_address(const string &&str) {
        return get_address("socket_callback_" + str);
    }

    static string get_upstream_callback_address(const node_id_t parentId, const node_id_t nodeId) {
        return get_upstream_callback_address(::to_string(parentId) + "_" + ::to_string(nodeId));
    }


    Node(node_id_t nodeId, node_id_t parentId, zmq::context_t *ctx) {
        m_ctx = ctx;
        m_node_id = nodeId;
        m_parent_id = parentId;
        sockets.emplace_back(*ctx, zmq::socket_type::pub); // flow
        sockets.emplace_back(*ctx, zmq::socket_type::pull); // callback
//        get(S_IDXS::CALLBACK).set(zmq::sockopt::rcvtimeo, WAIT_TIME);

        bind_flow();
    }

    void bind_flow() {
        logger_d ll("binding flow socket...", logDEBUG3, ::to_string(m_node_id));
        log_(logDEBUG4, ::to_string(m_node_id)) << "address to flow bind: " << get_pub_sub_address(m_node_id);
        get(S_IDXS::FLOW).bind(get_pub_sub_address(m_node_id));
    }

    void connect_callback(node_id_t nodeId) {
        logger_d ll("connecting callback socket...", logDEBUG3, ::to_string(m_node_id));
        log_(logDEBUG4, ::to_string(m_node_id)) << "node id to callback connect: " << nodeId << "; parent id: "
                                                << m_node_id;
        get(S_IDXS::CALLBACK).connect(get_upstream_callback_address(m_node_id, nodeId));
    }

    class exit_exception : runtime_error {
    public:
        exit_exception() : runtime_error("exiting") {}
    };


    virtual void loop() = 0;

    size_t children_count = 0;
protected:
    virtual Execute wait_child() {
        logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
        zmq::message_t msg;
        Execute proto_msg;
        auto rc = get(S_IDXS::CALLBACK).recv(msg, zmq::recv_flags::none);
        if (!rc) {
            log_(logWARNING, m_node_id) << "timeout";
            proto_msg.set_client_id(-1);
            return proto_msg;
        }
        if (!proto_msg.ParseFromString(msg.to_string())) {
            log_(logERROR, ::to_string(m_node_id)) << "invalid format";
            return Execute();
        }
        Execute_ErrorType type = proto_msg.error_type();
        if (proto_msg.msg() == Execute_MessageType_ERROR && type != Execute_ErrorType_NOT_FOR_ME &&
            type != Execute_ErrorType_OK) {
            log_(logERROR, m_node_id) << string(proto_msg.error_message());
            throw node_exception(type, proto_msg.error_message());
        }
        return proto_msg;
    }

    zmq::socket_t &get(S_IDXS idx) {
        return sockets[(size_t) idx];
    }

    void send_flow(zmq::message_t &msg) {
        logger_d ll("sending flow message...", logDEBUG3, ::to_string(m_node_id));
        UNUSED(get(S_IDXS::FLOW).send(zmq::buffer(""), zmq::send_flags::sndmore));
        UNUSED(get(S_IDXS::FLOW).send(msg, zmq::send_flags::none));
    }

    void send_flow(const Execute &proto_msg) {
        string raw_msg = proto_msg.SerializeAsString();
        zmq::message_t msg(raw_msg);
        send_flow(msg);
    }

    void add_pid(node_id_t nid, pid_t pid) {
        pids[nid] = pid;
    }

    void delete_pid(node_id_t nid) {
        int status;
        log_(logINFO, m_node_id) << "waiting for " << nid << "(pid: " << pids[nid] << ")";
        waitpid(pids[nid], &status, 0);
        pids.erase(nid);
    }


    virtual void process_callback_message(const Execute &input_msg) = 0;

    map<node_id_t, pid_t> pids;
    vector<zmq::socket_t> sockets;
    node_id_t m_node_id, m_parent_id;
    zmq::context_t *m_ctx;
};

class CalcNode : Node {
public:
    CalcNode(node_id_t node_id, node_id_t parent_id, zmq::context_t *ctx) : Node(node_id, parent_id, ctx) {
        sockets.emplace_back(*ctx, zmq::socket_type::sub); // subscribe
        sockets.emplace_back(*ctx, zmq::socket_type::push); // upstream

        log_(logDEBUG1, ::to_string(m_node_id)) << "address to subscribe connect: " << get_pub_sub_address(m_parent_id);
        connect_subscribe();
        log_(logDEBUG1, ::to_string(m_node_id)) << "setting subscribe opt to \"\"";
        get(S_IDXS::SUBSCRIBE).set(zmq::sockopt::subscribe, "");
        bind_upstream();
        log_(logDEBUG1, ::to_string(m_node_id)) << "all sockets are connected and bound(at CalcNode)";

        Execute proto_msg;
        proto_msg.set_msg(Execute_MessageType_CREATE);
        proto_msg.set_client_id(m_node_id);
        send_upstream(proto_msg);
    }


    void loop() override {
        zmq::active_poller_t poller;
        poller.add(get(S_IDXS::SUBSCRIBE), zmq::event_flags::pollin, [&](zmq::event_flags ef) {
            Execute proto_msg;
            if (handle_msg(proto_msg, S_IDXS::SUBSCRIBE)) {
                process_subscribe_message(proto_msg);
            }
        });
        poller.add(get(S_IDXS::CALLBACK), zmq::event_flags::pollin, [&](zmq::event_flags ef) {
            Execute proto_msg;
            if (handle_msg(proto_msg, S_IDXS::CALLBACK)) {
                process_callback_message(proto_msg);
            }
        });

        static const chrono::milliseconds timeout(-1);
        while (true) {
            auto n = poller.wait(timeout);
            if (!n) {
                break;
            }
        }
    }

private:
    bool handle_msg(Execute &proto_msg, S_IDXS sock_type) {
        zmq::message_t msg;
        auto res = get(sock_type).recv(msg, zmq::recv_flags::none);
        if (!res) {
            log_(logERROR, ::to_string(m_node_id)) << "something went wrong...";
            return false;
        }
        if (sock_type == S_IDXS::SUBSCRIBE) {
            res = get(sock_type).recv(msg, zmq::recv_flags::none);
            if (!res) {
                log_(logERROR, ::to_string(m_node_id)) << "something went wrong...";
                return false;
            }
        }
        bool parsed = proto_msg.ParseFromString(msg.to_string());
        if (!parsed) {
            log_(logERROR, ::to_string(m_node_id)) << "wrong message format";
            return false;
        }
        return true;
    }

    void process_subscribe_message(const Execute &input_msg) {
        log_(logDEBUG2, ::to_string(m_node_id)) << "processing message from subscribe socket...";
        switch (input_msg.msg()) {
            case Execute_MessageType_CREATE:
                create_command(input_msg);
                break;
            case Execute_MessageType_PING:
                ping_command(input_msg);
                break;
            case Execute_MessageType_EXEC:
                exec_command(input_msg);
                break;
            case Execute_MessageType_REMOVE:
                remove_command(input_msg);
                break;
            case Execute_MessageType_GET_MIN_DEPTH:
                get_min_depth_command(input_msg);
                break;
            case Execute_MessageType_EXIT_ALL:
                exit_all_command(input_msg);
                break;
            case Execute_MessageType_ERROR:
                error_command(input_msg);
                break;
            default:
                return;
        }
    }

    void process_callback_message(const Execute &input_msg) override {
//        log_(logWARNING, ::to_string(m_node_id)) << "not expected to be got there";
        logger_d ll("processing message from callback socket(just resend it to up)...", logDEBUG3,
                    ::to_string(m_node_id));
        send_upstream(input_msg);
    }

    void create_command(const Execute &input_msg) {
        if (input_msg.client_id() != m_node_id) {
            if (children_count > 0) {
                send_flow(input_msg);
                Execute proto_msg;
                proto_msg.set_msg(Execute_MessageType_ERROR);
                proto_msg.set_error_type(Execute_ErrorType_NOT_FOR_ME);
                for (size_t i = 0; i < children_count; ++i) {
                    logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
                    Execute msg = wait_child();
                    if (msg.client_id() == -1 && pids.count(input_msg.client_id()) > 0) {
                        log_(logERROR, m_node_id) << "child not available(pid: " << pids[input_msg.client_id()]
                                                  << "; nid: " << input_msg.client_id() << ")";
                        delete_pid(input_msg.client_id());
                    }
                    if (msg.msg() == Execute_MessageType_CREATE && msg.error_type() == Execute_ErrorType_OK) {
                        proto_msg.set_msg(Execute_MessageType_CREATE);
                        proto_msg.set_error_type(Execute_ErrorType_OK);
                        proto_msg.set_client_id(msg.client_id());
                        proto_msg.add_args(msg.args(0));
                        break;
                    }
                }
                send_upstream(proto_msg);
            } else {
                Execute proto_msg;
                proto_msg.set_client_id(m_node_id);
                proto_msg.set_msg(Execute_MessageType_ERROR);
                proto_msg.set_error_type(Execute_ErrorType_NOT_FOR_ME);
                send_upstream(proto_msg);
            }
            return;
        }
        node_id_t nid = input_msg.args(0);
        log_(logDEBUG, ::to_string(m_node_id)) << "I'm going to be a parent for " << nid;
        if (children_count < tree_base) {
            log_(logDEBUG2, m_node_id) << "children count: " << children_count;
            children_count++;
            pid_t pid = create_child(getexepath(), ::to_string(nid), ::to_string(m_node_id));
            add_pid(nid, pid);
            connect_callback(nid);
            log_(logDEBUG, ::to_string(m_node_id)) << "Waiting for a child";
            wait_child();

            Execute proto_msg;
            proto_msg.set_msg(Execute_MessageType_CREATE);
            proto_msg.set_error_type(Execute_ErrorType_OK);
            proto_msg.set_client_id(nid);
            proto_msg.add_args(pid);
            send_upstream(proto_msg);
        } else {
            Execute proto_msg;
            proto_msg.set_client_id(m_node_id);
            proto_msg.set_msg(Execute_MessageType_ERROR);
            proto_msg.set_error_type(Execute_ErrorType_CUSTOM);
            proto_msg.set_error_message("This node has already had all children");
            send_upstream(proto_msg);
        }
    }

    void ping_command(const Execute &input_msg) {
        if (input_msg.client_id() != m_node_id) {
            bool sent = false;
            logger_d ll("I was NOT pinged(pinged not me)", logDEBUG2, ::to_string(m_node_id));
            if (children_count > 0) {
                bool my_child = pids.count(input_msg.client_id()) > 0;

                send_flow(input_msg);
                Execute proto_msg;
                proto_msg.set_msg(Execute_MessageType_ERROR);
                proto_msg.set_error_type(Execute_ErrorType_NOT_FOR_ME);
                log_(logDEBUG2, m_node_id) << "children count: " << children_count;
                for (size_t i = 0; i < children_count; ++i) {
                    Execute msg;
                    try {
                        logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
                        msg = wait_child();
                    } catch (node_exception &err) {
                        continue;
                    }
                    if (msg.client_id() == -1 && pids.count(input_msg.client_id()) > 0 &&
                        proto_msg.error_type() != Execute_ErrorType_OK) {
                        proto_msg.set_msg(Execute_MessageType_ERROR);
                        proto_msg.set_error_type(Execute_ErrorType_NOT_AVAILABLE);
                        proto_msg.set_client_id(input_msg.client_id());
                    }
                    if (msg.msg() == Execute_MessageType_PING && msg.error_type() == Execute_ErrorType_OK) {
                        proto_msg.set_msg(Execute_MessageType_PING);
                        proto_msg.set_error_type(Execute_ErrorType_OK);
                        proto_msg.set_client_id(msg.client_id());
                        sent = true;
                        send_upstream(msg);
                    }
                }
                if (my_child) {
                    // node is not available
                    if (proto_msg.error_type() != Execute_ErrorType_OK &&
                        proto_msg.error_type() != Execute_ErrorType_NOT_FOR_ME) {
                        log_(logINFO, m_node_id) << "deleting node with nid: " << input_msg.client_id();
                        children_count--;
                        delete_pid(input_msg.client_id());
                    }
                }
                if(!sent) {
                    send_upstream(proto_msg);
                }
            } else {
                Execute proto_msg;
                proto_msg.set_client_id(m_node_id);
                proto_msg.set_msg(Execute_MessageType_ERROR);
                proto_msg.set_error_type(Execute_ErrorType_NOT_FOR_ME);
                send_upstream(proto_msg);
            }
            return;
        }
        log_(logDEBUG, ::to_string(m_node_id)) << "I was pinged";
        Execute proto_msg(input_msg);
        proto_msg.set_error_type(Execute_ErrorType_OK);
        proto_msg.set_msg(Execute_MessageType::Execute_MessageType_PING);
        proto_msg.set_client_id(m_node_id);
        send_upstream(proto_msg);
    }

    void exec_command(const Execute &input_msg) {
        if (input_msg.client_id() != m_node_id) {
            send_flow(input_msg);
            return;
        }
        // proceed command
        int64_t ans(0);
        for (const auto &el: input_msg.args()) {
            ans += el;
        }
        // send result
        Execute proto_msg;
        proto_msg.set_msg(Execute_MessageType_EXEC);
        proto_msg.set_error_type(Execute_ErrorType_OK);
        proto_msg.set_client_id(m_node_id);
        proto_msg.add_args(ans);
        send_upstream(proto_msg);
    }

    void get_min_depth_command(const Execute &input_msg) {
        if (children_count == tree_base) {
            Execute proto_msg(input_msg);
            proto_msg.set_args(0, proto_msg.args(0) + 1);
            send_flow(proto_msg);

            size_t min_depth = numeric_limits<size_t>::max();
            node_id_t nid = -1;
            for (int i = 0; i < children_count; ++i) {
                zmq::message_t msg;
                auto res = get(S_IDXS::CALLBACK).recv(msg, zmq::recv_flags::none);
                if (!res) {
                    log_(logERROR, m_node_id) << "no received messages";
                    return;
                }
                string raw_msg = msg.to_string();

                if (!proto_msg.ParseFromString(raw_msg)) {
                    log_(logERROR, m_node_id) << "wrong format";
                    return;
                }

                if (proto_msg.args(0) < min_depth) {
                    min_depth = proto_msg.args(0);
                    nid = proto_msg.client_id();
                }

                proto_msg.Clear();
            }
            assert(nid != -1 && "Some wrong behavior");
            proto_msg.set_msg(Execute_MessageType_GET_MIN_DEPTH);
            proto_msg.set_error_type(Execute_ErrorType_OK);
            proto_msg.set_client_id(nid);
            proto_msg.add_args(min_depth);
            send_upstream(proto_msg);
        } else {
            Execute proto_msg(input_msg);
            proto_msg.set_client_id(m_node_id);
            send_upstream(proto_msg);
        }
    }

    void remove_command(const Execute &input_msg) {
        if (input_msg.client_id() == m_node_id) {
            logger_d ll("remove command processing...", logDEBUG2, ::to_string(m_node_id));
            Execute proto_msg(input_msg);
            proto_msg.set_client_id(m_parent_id);
            proto_msg.set_msg(Execute_MessageType_REMOVE);
            proto_msg.set_error_type(Execute_ErrorType_OK);
            send_upstream(proto_msg);

            zmq::poller_t<> poller;
            poller.add(get(S_IDXS::UPSTREAM), zmq::event_flags::pollout);
            vector<zmq::poller_event<>> events(1);
            const chrono::milliseconds timeout(-1);
            poller.wait_all(events, timeout);

            proto_msg.Clear();
            proto_msg.set_client_id(m_node_id);
            proto_msg.set_msg(Execute_MessageType_EXIT_ALL);
            exit_all_command(proto_msg);
            return;
        }

        if (children_count > 0) {
            send_flow(input_msg);
            Execute msg;
            for (size_t i = 0; i < children_count; ++i) {
                logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
                Execute temp_msg;
                temp_msg = wait_child();
                if (temp_msg.error_type() == Execute_ErrorType_OK) {
                    msg = temp_msg;
                }
            }
            if (msg.error_type() == Execute_ErrorType_OK) {
                if (msg.client_id() == m_node_id) {
                    children_count--;
                    delete_pid(input_msg.client_id());
                }
                send_upstream(msg);
                return;
            }
        }
        Execute proto_msg(input_msg);
        proto_msg.set_msg(Execute_MessageType_ERROR);
        proto_msg.set_error_type(Execute_ErrorType_NOT_FOR_ME);
        send_upstream(proto_msg);
    }

    void exit_all_command(const Execute &input_msg) {
        if (children_count > 0) {
            send_flow(input_msg);
        }
        zmq::poller_t<> poller;
        poller.add(get(S_IDXS::FLOW), zmq::event_flags::pollout);
        const chrono::milliseconds timeout(-1);
        vector<zmq::poller_event<>> events(1);
        poller.wait_all(events, timeout);
        throw exit_exception();
    }

    void error_command(const Execute &input_msg) {
        send_upstream(input_msg);
    }

    void bind_upstream() {
        logger_d ll(string("binding upstream socket..."), logDEBUG3, ::to_string(m_node_id));
        log_(logDEBUG4, ::to_string(m_node_id)) << "address to bind upstream: "
                                                << get_upstream_callback_address(m_parent_id, m_node_id);
        get(S_IDXS::UPSTREAM).bind(get_upstream_callback_address(m_parent_id, m_node_id));
    }

    void connect_subscribe() {
        logger_d ll(string("connecting subscribe socket..."), logDEBUG3, ::to_string(m_node_id));
        log_(logDEBUG4, ::to_string(m_node_id)) << "address to connect subscribe: " << get_pub_sub_address(m_parent_id);
        get(S_IDXS::SUBSCRIBE).connect(get_pub_sub_address(m_parent_id));
    }


    void send_upstream(zmq::message_t &msg) {
        logger_d ll(string("sending upstream message..."), logDEBUG3, ::to_string(m_node_id));
        UNUSED(get(S_IDXS::UPSTREAM).send(msg, zmq::send_flags::none));
    }

    void send_upstream(const Execute &proto_msg) {
        string raw_msg = proto_msg.SerializeAsString();
        zmq::message_t msg(raw_msg);
        send_upstream(msg);
    }
};

class HeadNode : Node {
public:
    HeadNode(node_id_t nodeId, node_id_t parentId, zmq::context_t *ctx) : Node(nodeId, parentId, ctx) {
        thr = thread([this]() {
            this->loop();
        });
    }

    ~HeadNode() {
        running = false;
        thr.join();
        exit_all_command();
    }

    pid_t create_node(node_id_t nid) {
        if (nid == m_node_id) return -1;
        if (ping_command(nid)) {
            return -1;
        }
        logger_d ll("creating new node with " + ::to_string(nid) + " nid", logDEBUG, ::to_string(m_node_id));
        if (children_count < tree_base) {
            children_count++;

            lock_guard<mutex> lk(m_mut);
            pid_t pid = create_child(getexepath(), ::to_string(nid), ::to_string(m_node_id));
            connect_callback(nid);

            logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
            wait_child();
            return pid;
        }

        auto depth = get_min_depth();

        assert(depth.first != node_id_t(-1) && "Count of actual children is zero, but correspond variable is not zero");

        log_(logDEBUG1, ::to_string(m_node_id)) << "got min depth " << depth.first << " from node with id "
                                                << depth.second;

        Execute proto_msg;
        proto_msg.set_msg(Execute_MessageType::Execute_MessageType_CREATE);
        proto_msg.set_client_id(depth.second);
        proto_msg.add_args(nid);

        lock_guard<mutex> lk(m_mut);
        send_flow(proto_msg);

        pid_t pid = 0;
        for (int i = 0; i < children_count; ++i) {
            auto msg = wait_child();
            if (msg.error_type() == Execute_ErrorType_OK) {
                pid = msg.args(0);
            } else if (msg.error_type() != Execute_ErrorType_NOT_FOR_ME) {
                log_(logWARNING, ::to_string(m_node_id)) << "some error occurred: " << msg.error_message();
            } else {
                log_(logDEBUG, ::to_string(m_node_id)) << "not for " << msg.client_id();
            }
        }
        return pid;
    }

    pair<size_t, node_id_t> get_min_depth() {
        logger_d ll("getting minimal depth...", logDEBUG2, ::to_string(m_node_id));
        Execute proto_msg;
        proto_msg.set_client_id(default_parent_nid);
        proto_msg.add_args(1);
        proto_msg.set_msg(::Execute_MessageType_GET_MIN_DEPTH);

        lock_guard<mutex> lk(m_mut);
        send_flow(proto_msg);

        size_t min_depth = -1;
        node_id_t nid = m_node_id;
        for (size_t i(0); i < sockets.size(); ++i) {
            logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
            proto_msg = wait_child();

            if (proto_msg.client_id() == -1) {
                continue;
            }

            if (proto_msg.args(0) < min_depth) {
                min_depth = proto_msg.args(0);
                nid = proto_msg.client_id();
            }
        }
        return {min_depth, nid};
    }

    void loop() override {
        zmq::poller_t<> poller;
        poller.add(get(S_IDXS::CALLBACK), zmq::event_flags::pollin);

        const chrono::milliseconds timeout(0);
        vector<zmq::poller_event<>> events(1);
        while (running) {
            this_thread::sleep_for(chrono::milliseconds(100));
            m_mut.lock();
            zmq::message_t msg;
            const auto nin = poller.wait_all(events, timeout);
            if (!nin) {
                m_mut.unlock();
                log_(logDEBUG4, m_node_id) << "nothing to process";
                continue;
            }
            UNUSED(events[0].socket.recv(msg, zmq::recv_flags::none));
            m_mut.unlock();

            Execute proto_msg;
            bool parsed = proto_msg.ParseFromString(msg.to_string());
            if (!parsed) {
                log_(logERROR, m_node_id) << "wrong message format";
                return;
            }
            process_callback_message(proto_msg);
        }
    }

    bool remove_command(node_id_t nid) {
        if (children_count > 0) {
            Execute proto_msg;
            proto_msg.set_client_id(nid);
            proto_msg.set_msg(Execute_MessageType_REMOVE);

            lock_guard<mutex> lk(m_mut);
            send_flow(proto_msg);
            for (int i = 0; i < children_count; ++i) {
                logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
                Execute msg = wait_child();
                if (msg.error_type() == Execute_ErrorType_OK) {
                    int ind((int) children_count - i - 1);
                    for (int j = 0; j < ind; ++j) {
                        logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
                        wait_child();
                    }
                    if (msg.client_id() == m_node_id) {
                        delete_pid(nid);
                        children_count--;
                    }
                    return true;
                }
            }

        }
        return false;
    }

    bool ping_command(node_id_t nid) {
        if (children_count > 0) {
            bool my_child = pids.count(nid) > 0;

            Execute proto_msg;
            proto_msg.set_client_id(nid);
            proto_msg.set_msg(Execute_MessageType_PING);
            {
                lock_guard<mutex> lk(m_mut);
                send_flow(proto_msg);
                bool res = false;
                for (size_t i = 0; i < children_count; ++i) {
                    logger_d ll("waiting for a child answer...", logDEBUG2, ::to_string(m_node_id));
                    Execute msg = wait_child();
                    if (msg.client_id() == -1) {
                        if (my_child) {
                            delete_pid(nid);
                            children_count--;
                        }
                        return res;
                    }
                    if (msg.msg() == Execute_MessageType_PING && msg.error_type() == Execute_ErrorType_OK) {
                        res = true;
                    }
                }
                return res;
            }
        }
        return false;
    }

    bool exec_command(node_id_t nid, const vector<int64_t> &args) {
        if (!ping_command(nid)) {
            return false;
        }

        Execute proto_msg;
        proto_msg.set_msg(Execute_MessageType_EXEC);
        proto_msg.set_client_id(nid);
        for (const auto &el: args) {
            proto_msg.add_args(el);
        }
        send_flow(proto_msg);
        return true;
    }

    void exit_all_command() {
        Execute proto_msg;
        proto_msg.set_msg(Execute_MessageType_EXIT_ALL);
        send_flow(proto_msg);

        zmq::poller_t<> poller;
        poller.add(get(S_IDXS::FLOW), zmq::event_flags::pollout);

        const chrono::milliseconds timeout(-1);
        vector<zmq::poller_event<>> events(1);
        poller.wait_all(events, timeout);
    }


private:
    void process_callback_message(const Execute &input_msg) override {
        if (input_msg.msg() == Execute_MessageType_EXEC) {
            cout << "Ok: " << input_msg.args(0) << endl;
        } else if (input_msg.msg() == Execute_MessageType_ERROR) {
            cout << "Error: " << input_msg.error_message() << endl;
        } else {
            log_(logWARNING, m_node_id) << "Unexpected callback message received";
        }
    }

    thread thr;
    mutex m_mut;
    volatile bool running = true;
};

void print_versions(ostream &out) {
    auto zver = zmq::version();
    out << "Using 0MQ version " << get<0>(zver) << '.' << get<1>(zver) << '.' << get<2>(zver) << endl;
    out << "Using Protobuf version " << google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION) << endl;
}


int main(int argc, char **argv) { // node_id parent_id
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    zmq::context_t ctx;

    if (argc > 1) {
        string node_id_s(argv[1]), parent_id_s(argv[2]);

        int64_t node_id = stoi(node_id_s), parent_id = stoi(parent_id_s);

        log_(logDEBUG4, node_id) << "node id: " << node_id << " parent id: " << parent_id;

        CalcNode node(node_id, parent_id, &ctx);
        try {
            log_(logDEBUG1, node_id) << "going to loop...";
            node.loop();
        } catch (Node::exit_exception &err) {
            log_(logINFO, node_id) << "exiting...";
        }
    } else {
        print_versions(cout);

        HeadNode head(default_parent_nid, -1, &ctx);

        string cmd;
        cout << "> " << flush;
        while (cin >> cmd) {
            int nid;
            if (!(cin >> nid)) {
                break;
            }
            if (cmd == "create") {
                pid_t pid;
                try {
                    pid = head.create_node(nid);
                } catch (node_exception &err) {
                    cout << "Error: " << err.what() << endl;
                    continue;
                }
                if (pid <= 0) {
                    cout << "Error: node already exists" << endl;
                } else {
                    cout << "Ok: " << pid << endl;
                }
            } else if (cmd == "ping") {
                bool res;
                try {
                    res = head.ping_command(nid);
                } catch (node_exception &err) {
                    cout << "Error: " << err.what() << endl;
                    continue;
                }
                if (!res) {
                    cout << "Ok: 0" << endl;
                } else {
                    cout << "Ok: 1" << endl;
                }
            } else if (cmd == "remove") {
                bool res;
                try {
                    res = head.remove_command(nid);
                } catch (node_exception &err) {
                    cout << "Error: " << err.what() << endl;
                    continue;
                }
                if (!res) {
                    cout << "Error: node is unavailable" << endl;
                } else {
                    cout << "Ok" << endl;
                }
            } else if (cmd == "exec") {
                size_t n;
                if (!(cin >> n)) {
                    break;
                }
                vector<int64_t> args(n);
                for (size_t i = 0; i < n; ++i) {
                    cin >> args[i];
                }
                if (!head.exec_command(nid, args)) {
                    cout << "Error: node is unavailable" << endl;
                }
            }
            cout << "> " << flush;
        }
        google::protobuf::ShutdownProtobufLibrary();
    }

    return 0;
}
