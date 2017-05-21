#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <non_copyable.h>
#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>

class ThreadPool : private NonCopyable
{
public:
    using Job = std::function<void(void)>;

    ThreadPool();
    ~ThreadPool();

    size_t  pool_size() const { return _threads.size(); }
    int     task_create();
    bool    task_finished(int task) const;
    void    task_perform(int task, const Job& job);
    void    task_wait(int task);

private:
    void worker_main();

    bool                        _running;
    mutable std::mutex          _mutex;
    std::vector<std::thread>    _threads;

    size_t                      _task_size;
    std::vector<int>            _free_tasks;
    std::vector<unsigned>       _task_pending_count;

    size_t                      _job_size;
    std::vector<int>            _job_task;
    std::vector<Job>            _job_function;

    std::condition_variable     _cv_worker;
    std::condition_variable     _cv_master;
};

#endif
