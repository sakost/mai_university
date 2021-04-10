#include <zmq.hpp>

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <filesystem>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdio>

#include "zhelpers.hpp"
#include "definitions.h"
#include "logger.hpp"

#define FLOW_SOCKET 0
#define CALLBACK_SOCKET 1
#define SUBSCRIBE_SOCKET 2
#define PUSH_UP_SOCKET 3

using namespace std;
namespace fs = std::filesystem;

const size_t tree_base = 1;

loglevel_e loglevel = logDEBUG4;

node_id_t node_id = 0, parent_node_id=-1;

vector<zmq::socket_t> sockets;
zmq::context_t *ctx;

// zmq::socket_t *flow_socket = nullptr, *callback_socket = nullptr, *subscribe_socket;
size_t count_children = 0;

string default_sockets_path("sockets.lab");
string default_sockets_flow_path("sockets.lab.flow");


class path_does_not_exists{
public:
    path_does_not_exists(const std::string&& what_) : msg(what_){}
    path_does_not_exists(const std::string& what_) : msg(what_){}
    const char* what(){
        return msg.c_str();
    }
private:
    string msg;
};

class bad_fork{};


void suspend_zmq(){
    if(!sockets.empty()){
        log(logDEBUG) << "deleting sockets...";
        sockets.clear();
        log(logDEBUG) << "OK";
    }
    log(logDEBUG) << "deleting context...";
    delete ctx;
    log(logDEBUG) << "OK";
}

void deleter(){
    log(logINFO) << "deleter called";
    suspend_zmq();
}



pid_t create_child(const string path_to_exec, const string nid, const string parent){
    if(!fs::exists(path_to_exec)){
        throw path_does_not_exists(path_to_exec);
    }
    pid_t pid = fork();
    if(pid > 0) return pid;
    if(pid < 0) throw bad_fork();
    execl(path_to_exec.c_str(), path_to_exec.c_str(), nid.c_str(), parent.c_str());
    throw bad_fork();
}

bool send_msg(const Execute& msg){
    s_sendmore(sockets[FLOW_SOCKET], "[" + ::to_string(node_id) + "]");
    s_send(sockets[FLOW_SOCKET], msg.SerializeAsString());

    for (size_t i = 0; i < count_children; i++)
    {
        string ans = s_recv(sockets[CALLBACK_SOCKET]);

        Execute msg;
        msg.ParseFromString(ans);
        if(msg.error_type() == Execute_ErrorType::Execute_ErrorType_OK){
            return true;
        }
        else if(msg.error_type() != Execute_ErrorType::Execute_ErrorType_NOT_FOR_ME){
            string error_msg = msg.error_message();
            if(!error_msg.empty()){
                throw runtime_error(error_msg.c_str());
            }
            throw runtime_error("Something went wrong");
        }
    }
    return false;
}

bool ping(node_id_t nid){
    Execute msg;
    msg.set_client_id(nid);
    return send_msg(msg);
}

bool upstream_msg(const Execute& msg){
    auto m = msg.SerializeAsString();
    s_send(sockets[PUSH_UP_SOCKET], m);
    return true;
}


string build_socket_ipc_path(const string& str){
    return "ipc://@" + default_sockets_path  + str + ".ipc";
}

string build_socket_ipc_path(const string&& str){
    return "ipc://@" + default_sockets_path  + str + ".ipc";
}
string build_socket_ipc_flow_path(const string& str){
    return "ipc://@" + default_sockets_flow_path  + str + ".ipc";
}
string build_socket_ipc_flow_path(const string&& str){
    return "ipc://@" + default_sockets_flow_path  + str + ".ipc";
}

string build_socket_ipc_path(node_id_t nid){
    return build_socket_ipc_path(::to_string(nid));
}

string build_socket_ipc_flow_path(node_id_t nid){
    return build_socket_ipc_flow_path(::to_string(nid));
}


inline string build_prefix(node_id_t nid){
    return "[" + ::to_string(nid) + "]";
}
inline string build_prefix(const string& nid){
    return "[" + nid + "]";
}


namespace calc_node{
     void resume_zmq(){
        log(logINFO) << "resuming zmq(creating new)...";
        ctx = new zmq::context_t;
        sockets.emplace_back(*ctx, ZMQ_PUB); // flow
        sockets.emplace_back(*ctx, ZMQ_PULL); // callback
        sockets.emplace_back(*ctx, ZMQ_SUB); // subscribe
        sockets.emplace_back(*ctx, ZMQ_PUSH); // push up
    }

    void bind_connect_sockets(){
        auto str = build_socket_ipc_flow_path(node_id);
        log(logDEBUG2) << "bind flow socket with path: " << str;
        sockets[FLOW_SOCKET].bind(str);

        // str = build_socket_ipc_path(node_id);
        // log(logDEBUG2) << "connect callback socket with path: " << str;
        // sockets[CALLBACK_SOCKET].connect(str);

        str = build_socket_ipc_flow_path(parent_node_id);
        log(logDEBUG2) << "connecting sub socket to path: " << str;
        sockets[SUBSCRIBE_SOCKET].connect(str);
        log(logDEBUG4) << "setting filter of subscriber to: " << build_prefix(parent_node_id);
        sockets[SUBSCRIBE_SOCKET].setsockopt(ZMQ_SUBSCRIBE, ""); // build_prefix(parent_node_id)

        str = build_socket_ipc_path(parent_node_id);
        log(logDEBUG2) << "bind push up socket with path: " << str;
        sockets[PUSH_UP_SOCKET].bind(str);
    }


    bool process_message(Execute& msg, string path_to_me){
        auto msg_type = msg.msg();
        if(msg_type == Execute_MessageType::Execute_MessageType_PING)
        {
            if(msg.client_id() == node_id || count_children == 0){
                if(msg.client_id() == node_id){
                    msg.set_error_type(Execute_ErrorType::Execute_ErrorType_OK);
                }
                else{
                    msg.set_error_type(Execute_ErrorType::Execute_ErrorType_NOT_FOR_ME);
                }
                string msg_s = msg.SerializeAsString();
                s_send(sockets[PUSH_UP_SOCKET], msg_s);
                return true;
            }
            return ping(msg.client_id());
        }
        else if(msg_type == Execute_MessageType::Execute_MessageType_CREATE)
        {
            if(msg.client_id() == node_id || count_children == 0){
                if(msg.client_id() == node_id){
                    try{
                        pid_t child_pid;
                        child_pid = create_child(path_to_me, ::to_string(msg.args(0)), ::to_string(node_id));
                        msg.set_error_type(Execute_ErrorType::Execute_ErrorType_OK);
                        msg.set_client_id(child_pid);
                        msg.clear_args();
                        count_children++;
                    } catch(path_does_not_exists &err){
                        msg.set_error_type(Execute_ErrorType::Execute_ErrorType_CUSTOM);
                        msg.set_error_message(err.what());
                    }
                }
                else{
                    msg.set_error_type(Execute_ErrorType::Execute_ErrorType_NOT_FOR_ME);
                }
                string msg_s = msg.SerializeAsString();
                s_send(sockets[PUSH_UP_SOCKET], msg_s);
                return true;
            }
            return send_msg(msg);
        }
        else if(msg_type == Execute_MessageType::Execute_MessageType_GET_MIN_DEPTH){
            if(count_children != 2){
                msg.set_client_id(node_id);
                return upstream_msg(msg);
            } else{
                msg.set_args(0, msg.args(0) + 1);
                return send_msg(msg);
            }
        }
        else if(msg_type == Execute_MessageType::Execute_MessageType_EXEC){
            if(msg.client_id() != node_id){
                return send_msg(msg);
            }
            int64_t ans = 0;
            for (size_t i = 0; i < msg.args_size(); i++)
            {
                ans += msg.args(i);
            }
            msg.clear_args();
            msg.set_args(0, ans);
            return upstream_msg(msg);
        }
        else if(msg.msg() == Execute_MessageType::Execute_MessageType_EXIT_ALL){
            log(logINFO) << "exiting successfully\n";
            exit(EXIT_SUCCESS);
        }
        return false;
    }


    void calc_node(int argc, char** argv){ // filename node_id parent_id
        log(logDEBUG3) << "arguments of calc node(count: " << argc << "):";
        for (size_t i = 0; i < argc; i++)
        {
            log(logDEBUG3) << argv[i];
        }
        
        assert(argc == 3);
        string nid_s = argv[1];
        string parent_id_s = argv[2];

        istringstream ss(nid_s + " " + parent_id_s);
        if(!(ss >> node_id >> parent_node_id)){
            throw logic_error("something went wrong");
        }

        bind_connect_sockets();


        log(logDEBUG3) << "enter process loop";
        while (true)
        {
            vector<zmq::pollitem_t> items  = {
                { static_cast<void*>(sockets[SUBSCRIBE_SOCKET]), 0, ZMQ_POLLIN, 0 },
                { static_cast<void*>(sockets[CALLBACK_SOCKET]), 0, ZMQ_POLLIN, 0 }
            };
            log(logDEBUG4) << "waiting for an event...";
            
            // zmq::poll(items, -1);
            log(logDEBUG) << "got a message!!!!1111!: " << s_recv(sockets[SUBSCRIBE_SOCKET]);
            log(logDEBUG) << s_recv(sockets[SUBSCRIBE_SOCKET]);
            break;
            log(logDEBUG2) << "polled an event";
            static bool got_depth = false;

            if(items[0].revents & ZMQ_POLLIN){
                log(logDEBUG2) << "Recieved message from " << s_recv(sockets[SUBSCRIBE_SOCKET]);
                string message_s = s_recv(sockets[SUBSCRIBE_SOCKET]);
                Execute msg;
                msg.ParseFromString(message_s);
                process_message(msg, string(argv[0]));
            }
            if(items[1].revents & ZMQ_POLLIN){
                log(logDEBUG2) << "Recieved callback";
                string message_s = s_recv(sockets[CALLBACK_SOCKET]);

                Execute msg;
                msg.ParseFromString(message_s);

                if(msg.msg() == Execute_MessageType::Execute_MessageType_GET_MIN_DEPTH){
                    if(got_depth){
                        got_depth = false;
                        continue;
                    }
                    got_depth = true;
                }

                if(msg.error_type() != Execute_ErrorType::Execute_ErrorType_NOT_FOR_ME)
                    s_send(sockets[PUSH_UP_SOCKET], message_s);
            }
        }
        suspend_zmq();
    }
}


namespace head_node{

    void bind_sockets(){
        auto str = build_socket_ipc_flow_path(node_id);
        log(logDEBUG) << "bind flow head socket to path: " << str;
        sockets[FLOW_SOCKET].bind(str);
        str = build_socket_ipc_path(node_id);
        log(logDEBUG) << "bind callback head socket to path: " << str;
        sockets[CALLBACK_SOCKET].connect(str);

    }

    void resume_zmq(){
        log(logDEBUG) << "resuming zmq(creating new)...";
        ctx = new zmq::context_t;
        sockets.emplace_back(*ctx, ZMQ_PUB); // flow
        sockets.emplace_back(*ctx, ZMQ_PULL); // callback
        bind_sockets();
        log(logDEBUG) << "OK";
    }

    void unbind_sockets(){
        log(logDEBUG) << "undbind head sockets...";
        sockets.clear();
        // auto str = build_socket_ipc_flow_path(node_id);
        // log(logDEBUG4) << "unbind flow socket from path: " << str;
        // sockets[FLOW_SOCKET].unbind(str);
        // str = build_socket_ipc_path(node_id);
        // log(logDEBUG4) << "disconnect callback socket from path: " << str;
        // sockets[CALLBACK_SOCKET].disconnect(str);
        log(logDEBUG) << "OK";
    }



    void head_node(int argc, char** argv){
        int major, minor, patch;
        zmq_version (&major, &minor, &patch);
        printf ("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
        major = GOOGLE_PROTOBUF_VERSION / (int)(1e6);
        minor = (GOOGLE_PROTOBUF_VERSION / (int)(1e3)) % (int)1e3;
        patch = GOOGLE_PROTOBUF_VERSION % (int)(1e3);
        printf("Current Protobuf version is %d.%d.%d\n", major, minor, patch);

        bind_sockets();
        atexit(unbind_sockets); // todo: make for children

        string cmd;
        cout << "> " << flush;
        while(cin >> cmd){
            node_id_t nid;
            cin >> nid;

            if(cmd == "create")
            {
                if(count_children < tree_base){
                    // suspend_zmq();
                    pid_t child_pid;
                    try{
                        child_pid = create_child(argv[0], ::to_string(nid), ::to_string(node_id));
                    } catch(path_does_not_exists& err){
                        cout << "Error: child process file(\""<< err.what() <<"\") does not exists" << endl;
                        exit(EXIT_FAILURE);
                    } catch(bad_fork&){
                        cout << "Error: can't create child process" << endl;
                        exit(EXIT_FAILURE);
                    }
                    // resume_zmq();
                    count_children++;

                    cout << "Ok: " << child_pid << endl;

                    cout << "> " << flush;
                    continue;
                }


                Execute message;
                message.set_client_id(0); // determine in children
                message.set_msg(Execute_MessageType::Execute_MessageType_GET_MIN_DEPTH);
                message.add_args(nid);

                string raw_msg = message.SerializeAsString();
                message.Clear();

                while(true){
                    // s_sendmore(sockets[FLOW_SOCKET], string("lol")); //build_prefix(node_id)
                    s_send(sockets[FLOW_SOCKET], raw_msg);

                    vector<zmq::pollitem_t> items  = {
                        { static_cast<void*>(sockets[CALLBACK_SOCKET]), 0, ZMQ_POLLIN, 0 }
                    };
                    zmq::poll(items, -1);
                    if(items[0].revents & ZMQ_POLLIN){
                        break;
                    }
                }
                
                raw_msg = s_recv(sockets[CALLBACK_SOCKET]);
                message.ParseFromString(raw_msg);

                message.set_args(0, nid);
                raw_msg = message.SerializeAsString();

                // s_sendmore(sockets[FLOW_SOCKET], string("lol")); // build_prefix(node_id)
                s_send(sockets[FLOW_SOCKET], raw_msg);
                
            }
            else if(cmd == "remove")
            {

            }
            else if(cmd == "ping")
            {  
                try{
                   if(!ping(nid)){
                       cout << "Ok: 0" << endl;
                   }
                } catch(runtime_error& err){
                    cout << "Error: " << err.what() << endl;
                    cout << "> " << flush;
                    continue;
                }
                cout << "Ok: 1" << endl;
                
            }
            else if(cmd == "exec"){
                size_t n;
                cin >> n;

                Execute msg;
                msg.set_client_id(nid);
            
                for (size_t i = 0; i < n; i++)
                {
                    int64_t tmp;
                    cin >> tmp;
                    msg.add_args(tmp);
                }
                
                // s_sendmore(sockets[FLOW_SOCKET], string("lol")); //build_prefix(node_id)
                s_send(sockets[FLOW_SOCKET], msg.SerializeAsString());  // todo: recv execute result
            }

            cout << "> " << flush;
        }
        cout << endl;
        Execute msg;
        msg.set_msg(Execute_MessageType::Execute_MessageType_EXIT_ALL);
        string raw_string = msg.SerializeAsString();
        // s_sendmore(sockets[FLOW_SOCKET], string("lol")); // build_prefix(node_id)
        s_send(sockets[FLOW_SOCKET], raw_string);
    }
}

class MyMonitor: public zmq::monitor_t{
    public:

    MyMonitor(string name): zmq::monitor_t(){
        m_name = name;
    }

    void on_event_connected(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_connected(e, m);
        logThis(string("event connected: ") + m);
    }
    
    void on_event_listening(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_listening(e, m);
        logThis(string("event listening: ") + m);
    }

    void on_event_accepted(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_accepted(e, m);
        logThis(string("event accepted: ") + m);
    }

    void on_event_connect_delayed(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_connect_delayed(e, m);
        logThis(string("event delayed: ") + m);
    }

    void on_event_handshake_succeeded(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_handshake_succeeded(e, m);
        logThis(string("event handshake succeed: ") + m);
    }
    void on_event_bind_failed(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_bind_failed(e, m);
        logThis(string("event bind failed: ") + m);
    }

    void on_event_connect_retried(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_connect_retried(e, m);
        logThis(string("event connect retried: ") + m);
    }
    void on_event_accept_failed(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_accept_failed(e, m);
        logThis(string("event accept failed: ") + m);
    }
    void on_event_disconnected(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_disconnected(e, m);
        logThis(string("event disconnected: ") + m);
    }
    void on_event_unknown(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_unknown(e, m);
        logThis(string("event unknown: ") + m + " errno: " + ::to_string(e.value) + " id of event: " + ::to_string(e.event));
    }

    void on_event_handshake_failed_auth(const zmq_event_t &e, const char *m) override{
        zmq::monitor_t::on_event_handshake_failed_auth(e, m);
        logThis(string("event handshake failed auth: ") + m);
    }
    
    private: 

    void logThis(string msg){
        log(logDEBUG4) << "socket name: " << m_name << " message: " << msg;
    }

    string m_name;
};


int main(int argc, char** argv) {

    try{
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        ctx = new zmq::context_t;
        atexit(deleter);

        sockets.emplace_back(*ctx, ZMQ_PUB); // flow
        sockets.emplace_back(*ctx, ZMQ_PULL); // callback

        MyMonitor monitor1("flow"), monitor2("callback");
        monitor1.init(sockets[FLOW_SOCKET], "inproc://monitor_flow.req");
        monitor2.init(sockets[CALLBACK_SOCKET], "inproc://monitor_callback.req");

        auto thread1 = std::thread([&monitor1]
        {
            while (monitor1.check_event(-1)) {
            }
        });
        auto thread2 = std::thread([&monitor2]
        {
            while (monitor2.check_event(-1)) {
            }
        });
            

        if(argc > 1){
            MyMonitor monitor3("sub"), monitor4("push_up");

            sockets.emplace_back(*ctx, ZMQ_SUB); // subscribe
            monitor3.init(sockets[SUBSCRIBE_SOCKET], "inproc://monitor_sub.req");
            auto thread3 = std::thread([&monitor3]
            {
                while (monitor3.check_event(-1)) {
                }
            });

            monitor4.init(sockets[SUBSCRIBE_SOCKET], "inproc://monitor_push_up.req");
            auto thread4 = std::thread([&monitor4]
            {
                while (monitor4.check_event(-1)) {
                }
            });

            
            sockets.emplace_back(*ctx, ZMQ_PUSH); // push up
            calc_node::calc_node(argc, argv);
        } else{
            head_node::head_node(argc, argv);
        }
        google::protobuf::ShutdownProtobufLibrary();
    }catch(exception& err){
        log(logERROR) << err.what();
    }
}
