#include <iostream>
#include "thread_pool.h"

void thread_one() {
    cout << "thread one." << endl;
}

void thread_two() {
    cout << "thread two." << endl;
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    ThreadPool pool(2);
    pool.Submit(thread_one);
    pool.Submit(thread_two);
    return 0;
}
