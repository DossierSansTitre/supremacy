#include <core/os.h>
#include <thread_pool.h>

#if defined(__APPLE__)
# include <mach/thread_act.h>
# include <mach/thread_policy.h>
#endif

#if !defined(NO_THREADING)
ThreadPool::ThreadPool()
: _running(true)
, _job_size(0u)
, _task_size(0u)
{
    size_t thread_count;

    thread_count = std::thread::hardware_concurrency();

    if (thread_count == 0)
        thread_count = 1;

    _threads.reserve(thread_count);

    for (size_t i = 0; i < thread_count; ++i)
    {
        _threads.emplace_back(&ThreadPool::worker_main, this);

#if OS_MAC
        auto& thread = _threads.back();
        thread_act_t thread_id = pthread_mach_thread_np(thread.native_handle());
        thread_affinity_policy_data_t affinity;
        affinity.affinity_tag = i + 1;
        thread_policy_set(thread_id, THREAD_AFFINITY_POLICY, (thread_policy_t)&affinity, THREAD_AFFINITY_POLICY_COUNT);
#endif
    }
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

int ThreadPool::create()
{
    std::lock_guard<std::mutex> lock(_mutex);
    int job;

    if (_free_jobs.empty())
    {
        job = static_cast<int>(_job_size);
        _job_size++;
        _job_pending_count.resize(_job_size);
    }
    else
    {
        job = _free_jobs.back();
        _free_jobs.pop_back();
    }
    _job_pending_count[job] = 0u;

    return job;
}

bool ThreadPool::finished(int job) const
{
    std::lock_guard<std::mutex> lock(_mutex);

    return _job_pending_count[job] == 0u;
}

void ThreadPool::perform(int job, Task task, void* obj, size_t start, size_t len)
{
    std::unique_lock<std::mutex> lock(_mutex);

    _task_job.push_back(job);
    _task_function.push_back(task);
    _task_obj.push_back(obj);
    _task_start.push_back(start);
    _task_len.push_back(len);
    _job_pending_count[job]++;
    _task_size++;
    lock.unlock();
    _cv_worker.notify_one();
}

void ThreadPool::wait(int job)
{
    std::unique_lock<std::mutex> lock(_mutex);

    while (_job_pending_count[job] > 0)
        _cv_master.wait(lock);

    _free_jobs.push_back(job);
}

void ThreadPool::worker_main()
{
    std::unique_lock<std::mutex> lock(_mutex);

    while (_running)
    {
        _cv_worker.wait(lock);

        for (;;)
        {
            if (_task_size == 0)
                break;

            int job = _task_job.back();
            Task task = _task_function.back();
            void* obj = _task_obj.back();
            size_t start = _task_start.back();
            size_t len = _task_len.back();
            _task_job.pop_back();
            _task_function.pop_back();
            _task_obj.pop_back();
            _task_start.pop_back();
            _task_len.pop_back();
            _task_size--;

            lock.unlock();

            (*task)(obj, start, len);

            lock.lock();
            _job_pending_count[job]--;
            if (_job_pending_count[job] == 0)
                _cv_master.notify_all();
        }
    }
}

#else

ThreadPool::ThreadPool()
: _running(true)
, _task_size(0u)
, _job_size(0u)
{
}

ThreadPool::~ThreadPool()
{
}

int ThreadPool::create()
{
    return 0;
}

bool ThreadPool::finished(int job) const
{
    return true;
}

void ThreadPool::perform(int job, Task task, void* obj, size_t start, size_t len)
{
    task(obj, start, len);
}

void ThreadPool::wait(int job)
{
}

void ThreadPool::worker_main()
{
}

#endif
