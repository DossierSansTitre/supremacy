#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <non_copyable.h>
#include <thread>
#include <vector>
#include <condition_variable>

class ThreadPool : private NonCopyable
{
public:
    using Task = void (*)(void*, size_t, size_t);

    ThreadPool();
    ~ThreadPool();

    size_t  pool_size() const { return _threads.size(); }
    int     create();
    bool    finished(int job) const;
    void    perform(int job, Task task, void* task_obj, size_t task_start, size_t task_len);
    void    wait(int job);

    template <typename T>
    void perform(int job, void (*task)(T*, size_t, size_t), T* obj, size_t start, size_t len)
    {
        perform(job, reinterpret_cast<Task>(task), static_cast<void*>(obj), start, len);
    }

    template <typename T>
    void perform_over(int job, void (*task)(T*, size_t, size_t), T* obj, size_t length, size_t slice)
    {
        size_t denom = length / slice;
        size_t remain = length % slice;

        for (size_t i = 0; i < denom; ++i)
            perform(job, task, obj, i * slice, slice);
        if (remain)
            perform(job, task, obj, length - remain, remain);
    }

    template <typename T>
    void run_over(void (*task)(T*, size_t, size_t), T* obj, size_t length, size_t slice)
    {
        int job;

        job = create();
        perform_over(job, task, obj, length, slice);
        wait(job);
    }

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
    std::vector<void*>          _task_obj;
    std::vector<size_t>         _task_start;
    std::vector<size_t>         _task_len;

    std::condition_variable     _cv_worker;
    std::condition_variable     _cv_master;
};

#endif
