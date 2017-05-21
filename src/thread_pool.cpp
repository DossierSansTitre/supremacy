#include <thread_pool.h>

ThreadPool::ThreadPool()
: _running(true)
, _task_size(0u)
, _job_size(0u)
{
    size_t thread_count;
    
    thread_count = std::thread::hardware_concurrency();

    if (thread_count == 0)
        thread_count = 1;

    _threads.reserve(thread_count);

    for (size_t i = 0; i < thread_count; ++i)
        _threads.emplace_back(&ThreadPool::worker_main, this);
}

ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(_mutex);

    _running = false;

    _cv_worker.notify_all();
    lock.unlock();
    for (auto& t : _threads)
        t.join();
}

int ThreadPool::task_create()
{
    std::lock_guard<std::mutex> lock(_mutex);
    int task;

    if (_free_tasks.empty())
    {
        task = static_cast<int>(_task_size);
        _task_size++;
        _task_pending_count.resize(_task_size);
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

    return _task_pending_count[task] == 0u;
}

void ThreadPool::task_perform(int task, const Job& job)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _job_task.push_back(task);
    _job_function.push_back(job);
    _task_pending_count[task]++;
    _job_size++;
    _cv_worker.notify_one();
}

void ThreadPool::task_wait(int task)
{
    std::unique_lock<std::mutex> lock(_mutex);

    while (_task_pending_count[task] > 0)
        _cv_master.wait(lock);

    _free_tasks.push_back(task);
}

void ThreadPool::worker_main()
{
    std::unique_lock<std::mutex> lock(_mutex);

    while (_running)
    {
        _cv_worker.wait(lock);

        for (;;)
        {
            if (_job_size == 0)
                break;

            int task = _job_task.back();
            Job job = _job_function.back();
            _job_task.pop_back();
            _job_function.pop_back();
            _job_size--;

            lock.unlock();

            job();

            lock.lock();
            _task_pending_count[task]--;
            if (_task_pending_count[task] == 0)
                _cv_master.notify_all();
        }
    }
}
