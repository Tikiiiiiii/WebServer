/*

抽象出线程池的内容，auto add实现右值移动，完美转发，提升效率
由于动态分配线程效率太低，因此固定初始的线程数，size指定这个个数
以任务队列的形式存储待执行的业务，线程自动在任务队列取任务执行
为了避免忙等，加入了条件变量condition_variable
访问公共资源注意锁的使用

*/
#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    template<class F, class... Args>
    auto add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

};


//C++编译器不支持模版的分离编译
template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    cv.notify_one();
    return res;
}