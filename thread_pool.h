//
// Created by Administrator on 2022/3/11.
//

#ifndef THREAD_POOL_THREAD_POOL_H
#define THREAD_POOL_THREAD_POOL_H
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <future>

namespace {
    using  namespace std;
    class ThreadPool {
        using Task = function<void()>; //定义task为func
        vector<thread> pool_; // 线程池
        queue<Task> tasks_; // 任务队列
        mutex lock_; //线程的互斥设施
        bool run_ {true};
        condition_variable task_cv_;
        uint32_t idle_thread_num; //空闲线程数

    public:
        ThreadPool(uint32_t thread_num = 4) {
            AddThread(thread_num);
        }
        ~ThreadPool() {
            run_ = false;
            task_cv_.notify_all();
            for (thread& thread : pool_) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }
        // Init thread pool
        void AddThread(uint32_t thread_num) {
            for (uint32_t size = 0 ; size < thread_num; ++size) {
                pool_.emplace_back([this]() {
                    while (run_) {
                        Task task;
                        {
                            unique_lock<mutex> lock(lock_);
                            task_cv_.wait(lock, [this]() {return !run_ || !tasks_.empty(); });
                            task = move(tasks_.front());
                            tasks_.pop();
                        }
                        task();
                    }
                });
                cout << "Add thread." << endl;
            }
        }
        // Add task to queue
        template<class F, class... Args>
        auto Submit(F&& f, Args&&... args) ->future<decltype(f(args...))> {
            if (!run_) {
                cout << "Thread Pool is stoped." << endl;
            }
            using RetType = decltype(f(args...)); //自动推导函数返回的类型
            auto task = make_shared<packaged_task<RetType()>>(bind(forward<F>(f), forward<Args>(args)...));
            future<RetType> future = task->get_future();
            {
                lock_guard<mutex> lock(lock_);
                tasks_.emplace([task](){ (*task)(); });
            }
            task_cv_.notify_one(); // 唤醒一个线程执行
            return future;
        }
    };
}

#endif //THREAD_POOL_THREAD_POOL_H
