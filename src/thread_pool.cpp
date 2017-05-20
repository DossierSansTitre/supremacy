#include <thread_pool.h>

ThreadPool::ThreadPool()
: _running(true)
, _task_count(0u)
, _job_count(0u)
{
    size_t thread_count;
    
    thread_count = std::thread::hardware_concurrency();

    if (thread_count == 0)
        thread_count = 1;

    _threads.reserve(thread_count);

    for (size_t i = 0; i < thread_count; ++i)
    {
        _threads.emplace_back(&ThreadPool::worker_main, this);
    }
}

ThreadPool::~ThreadPool()
{
    _running = false;

    _cv_worker.notify_all();
    for (auto& t : _threads)
        t.join();
}

int ThreadPool::task_create()
{
    std::lock_guard<std::mutex> lock(_mutex);
    int task;

    if (_free_tasks.empty())
    {
        task = static_cast<int>(_task_count);
        _task_count++;
        _task_pending_count.resize(_task_count);
    }
    else
    {
        task = _free_tasks.back();
        _free_tasks.pop_back();
    }
    _task_pending_count[task] = 0u;

    return task;
}

bool ThreadPool::task_finished(int task) const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return (_task_pending_count[task] == 0u);
}

void ThreadPool::task_perform(int task, const Job& job)
{
    std::lock_guard<std::mutex> lock(_mutex);
    int job_index;

    if (_free_jobs.empty())
    {
        job_index = static_cast<int>(_job_count);
        _job_count++;
        _job_task.resize(_job_count);
        _job_function.resize(_job_count);
    }
    else
    {
        job_index = _free_jobs.back();
        _free_jobs.pop_back();
    }
    _job_task[job_index] = task;
    _job_function[job_index] = job;
    _task_pending_count[task]++;
    _pending_jobs.push_back(job_index);
    _cv_worker.notify_one();
}

void ThreadPool::task_wait(int task)
{
    std::unique_lock<std::mutex> lock(_mutex);

    for (;;)
    {
        _cv_master.wait(lock);

        if (!_task_pending_count[task])
            break;
    }

    _free_tasks.push_back(task);
}

void ThreadPool::worker_main()
{
    std::unique_lock<std::mutex> lock(_mutex);
    int task;
    int job;
    Job job_function;

    while (_running)
    {
        _cv_worker.wait(lock);

        for (;;)
        {
            if (_pending_jobs.empty())
                break;

            job = _pending_jobs.back();
            _pending_jobs.pop_back();

            task = _job_task[job];
            job_function = _job_function[job];

            lock.unlock();

            job_function();

            lock.lock();
            _task_pending_count[task]--;
            if (!_task_pending_count[task])
                _cv_master.notify_all();
            _free_jobs.push_back(job);
        }
    }
    lock.unlock();
}
