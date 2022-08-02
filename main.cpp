#include <iostream>
#include "thread_pool.h"

void thread_one() {
    cout << "thread one." << endl;
}

void thread_two() {
    cout << "thread two." << endl;
}

int thread_three(int a, int b) {
    int res = a + b;
    return res;
}

int main() {
    //
    std::cout << "Hello, World!" << std::endl;
    ThreadPool pool(5);
    // 测试初始化线程池、增加task任务基础功能
    pool.Submit(thread_one);
    pool.Submit(thread_two);
    // 测试异步获取函数返回值
    auto res_three = pool.Submit(thread_three, 2, 3);
    // 等待计算结果
    auto three = res_three.get();
    std::cout << "Three: " << three << std::endl;
    return 0;
}
