//
// Created by sakost on 20.12.2020.
//

#ifndef OOP_EXCERCISE_07_HANDLER_H
#define OOP_EXCERCISE_07_HANDLER_H

#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>
#include <vector>

#include <iostream>

template<typename T>
class Handler {
public:
    explicit Handler(std::size_t queue_max_size){
        this->queue_max_size = queue_max_size;
    };

    ~Handler(){
        if(running && started){
            stop();
        }
    }

    void register_handler(std::function<void(const std::vector<T>&)>&& func){
        if(!started) {
            handlers.push_back(func);
        } else{
            throw std::runtime_error("can't register handler while running");
        }
    }

    void start(){
        started = true;
        thread = std::thread(execution_in_thread, this);
    }

    void stop(){
        running = false;
        cv.notify_one();
        thread.join();
    }

    void push(T el){
        using namespace std;
        std::unique_lock<std::mutex> lk(mutex_queue);
        queue.push(el);
        if(queue_full()) {
            cv.notify_one();
            cv.wait(lk, [this](){
                return queue.empty();
            });
        }
    }


    bool queue_full(){
        return queue.size() == queue_max_size;
    }

protected:

    static void execution_in_thread(Handler *this_){
        using namespace std;
        while(this_->running) {
            std::unique_lock<std::mutex> lk(this_->mutex_queue);
            this_->cv.wait(lk, [this_]() {
                return this_->queue_full() || !this_->running;
            });
            vector<T> vec;
            vec.reserve(this_->queue.size());
            while(!this_->queue.empty()) {
                vec.push_back(this_->queue.front());
                this_->queue.pop();
            }
            reverse(vec.begin(), vec.end());
            if(!vec.empty()) {
                for (const auto &item: this_->handlers) {
                    item(vec);
                }
            }
            lk.unlock();
            this_->cv.notify_one();
        }
    }

    bool started = false, running = true;
    std::mutex mutex_queue;
    std::thread thread;
    std::condition_variable cv;
    std::queue<T> queue;
    std::vector<std::function<void(const std::vector<T>&)>> handlers;
    std::size_t queue_max_size = 0;
};


#endif //OOP_EXCERCISE_07_HANDLER_H
