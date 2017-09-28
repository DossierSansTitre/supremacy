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
    using Task = std::function<void(void)>;

    ThreadPool();
    ~ThreadPool();

    size_t  pool_size() const { return _threads.size(); }
    int     create();
    bool    finished(int job) const;
    void    perform(int job, const Task& task);
    void    wait(int job);

private:
    void worker_main();

    bool                        _running;
    mutable std::mutex          _mutex;
    std::vector<std::thread>    _threads;

    size_t                      _job_size;
    std::vector<int>            _free_jobs;
    std::vector<unsigned>       _job_pending_count;

    size_t                      _task_size;
    std::vector<int>            _task_job;
    std::vector<Task>           _task_function;

    std::condition_variable     _cv_worker;
    std::condition_variable     _cv_master;
};

#endif
