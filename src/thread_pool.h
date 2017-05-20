#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <non_copyable.h>
#include <thread>
#include <vector>
#include <atomic>
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

    std::vector<std::thread>    _threads;
    std::atomic_bool            _running;

    std::vector<int>            _free_tasks;

    size_t                      _task_count;
    std::vector<unsigned>       _task_pending_count;

    std::vector<int>            _free_jobs;
    std::vector<int>            _pending_jobs;

    size_t                      _job_count;
    std::vector<int>            _job_task;
    std::vector<Job>            _job_function;

    std::condition_variable     _cv_worker;
    std::condition_variable     _cv_master;
    mutable std::mutex          _mutex;
};

#endif
