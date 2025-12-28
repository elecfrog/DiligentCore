#pragma once

#include "../../Primitives/interface/Object.h"
#include "CommonDefinitions.h"


namespace Diligent
{

/// Asynchronous task status
DILIGENT_TYPED_ENUM(ASYNC_TASK_STATUS, UInt32){
    /// The asynchronous task status is unknown.
    ASYNC_TASK_STATUS_UNKNOWN,

    /// The asynchronous task has not been started yet.
    ASYNC_TASK_STATUS_NOT_STARTED,

    /// The asynchronous task is running.
    ASYNC_TASK_STATUS_RUNNING,

    /// The asynchronous task was cancelled.
    ASYNC_TASK_STATUS_CANCELLED,

    /// The asynchronous task is complete.
    ASYNC_TASK_STATUS_COMPLETE};


// {B06D1DDA-AEA0-4CFD-969A-C8E2011DC294}
static constexpr INTERFACE_ID IID_AsyncTask =
    {0xb06d1dda, 0xaea0, 0x4cfd, {0x96, 0x9a, 0xc8, 0xe2, 0x1, 0x1d, 0xc2, 0x94}};


#include "../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IAsyncTaskInclusiveMethods \
    IObjectInclusiveMethods;       \
    IAsyncTaskMethods AsyncTask

/// Asynchronous task interface
struct IAsyncTask : public IObject
{
    /// Run the asynchronous task.

    /// \param [in] ThreadId - Id of the thread that is running this task.
    ///
    /// Before starting the task, the thread pool sets its
    /// status to Diligent::ASYNC_TASK_STATUS_RUNNING.
    ///
    /// The method must return one of the following values:
    ///   - Diligent::ASYNC_TASK_STATUS_CANCELLED to indicate that the task was cancelled.
    ///   - Diligent::ASYNC_TASK_STATUS_COMPLETE to indicate that the task is finished successfully.
    ///   - Diligent::ASYNC_TASK_STATUS_NOT_STARTED to request the task to be rescheduled.
    ///
    /// The thread pool will set the task status to the returned value after
    /// the Run() method completes. This way if the GetStatus() method returns
    /// any value other than Diligent::ASYNC_TASK_STATUS_RUNNING, it is guaranteed that the task
    /// is not executed by any thread.
    virtual ASYNC_TASK_STATUS DG_CALL_TYPE Run(UInt32 ThreadId) = 0;

    /// Cancel the task, if possible.

    /// If the task is running, the task implementation should
    /// abort the task execution, if possible.
    virtual void METHOD(Cancel)() = 0;

    /// Sets the task status, see Diligent::ASYNC_TASK_STATUS.
    virtual void METHOD(SetStatus)(
        ASYNC_TASK_STATUS TaskStatus) = 0;

    /// Gets the task status, see Diligent::ASYNC_TASK_STATUS.
    virtual ASYNC_TASK_STATUS METHOD(GetStatus)() const = 0;

    /// Sets the task priorirty.
    virtual void METHOD(SetPriority)(
        float fPriority) = 0;

    /// Returns the task priorirty.
    virtual float METHOD(GetPriority)() const = 0;

    /// Checks if the task is finished (i.e. cancelled or complete).
    virtual bool METHOD(IsFinished)() const = 0;

    /// Waits until the task is complete.

    /// \note   This method must not be called from the same thread that is
    ///         running the task or a deadlock will occur.
    virtual void METHOD(WaitForCompletion)() const = 0;

    /// Waits until the tasks is running.

    /// \warning  An application is responsible to make sure that
    ///           tasks currently in the queue will eventually finish
    ///           allowing the task to start.
    ///
    /// This method must not be called from the worker thread.
    virtual void METHOD(WaitUntilRunning)() const = 0;
};

// {8BB92B5E-3EAB-4CC3-9DA2-5470DBBA7120}
static constexpr INTERFACE_ID IID_ThreadPool =
    {0x8bb92b5e, 0x3eab, 0x4cc3, {0x9d, 0xa2, 0x54, 0x70, 0xdb, 0xba, 0x71, 0x20}};

#include "../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IThreadPoolInclusiveMethods \
    IObjectInclusiveMethods;        \
    IThreadPoolMethods ThreadPool

/// Thread pool interface
struct IThreadPool : public IObject
{
    /// Enqueues asynchronous task for execution.

    /// \param[in] pTask            - Task to run.
    /// \param[in] ppPrerequisites  - Array of task prerequisites, e.g. the tasks
    ///                               that must be completed before this task can start.
    /// \param[in] NumPrerequisites - Number of prerequisites.
    ///
    /// Thread pool will keep a strong reference to the task,
    /// so an application is free to release it after enqueuing.
    ///
    /// \note       An application must ensure that the task prerequisites are not circular
    ///             to avoid deadlocks.
    virtual void METHOD(EnqueueTask)(
        IAsyncTask*                  pTask,
        IAsyncTask** ppPrerequisites DEFAULT_VALUE(nullptr),
        UInt32 NumPrerequisites      DEFAULT_VALUE(0)) = 0;


    /// Reprioritizes the task in the queue.

    /// \param[in] pTask - Task to reprioritize.
    ///
    /// \return     true if the task was found in the queue and was
    ///             successfully reprioritized, and false otherwise.
    ///
    /// When the tasks is enqueued, its priority is used to
    /// place it in the priority queue. When an application changes
    /// the task priority, it should call this method to update the task
    /// position in the queue.
    virtual bool METHOD(ReprioritizeTask)(
        IAsyncTask* pTask) = 0;


    /// Reprioritizes all tasks in the queue.

    /// This method should be called if task priorities have changed
    /// to update the positions of all tasks in the queue.
    virtual void METHOD(ReprioritizeAllTasks)() = 0;


    /// Removes the task from the queue, if possible.

    /// \param[in] pTask - Task to remove from the queue.
    ///
    /// \return    true if the task was successfully removed from the queue,
    ///            and false otherwise.
    virtual bool METHOD(RemoveTask)(
        IAsyncTask* pTask) = 0;


    /// Waits until all tasks in the queue are finished.

    /// The method blocks the calling thread until all
    /// tasks in the quque are finished and the queue is empty.
    /// An application is responsible to make sure that all tasks
    /// will finish eventually.
    virtual void METHOD(WaitForAllTasks)() = 0;


    /// Returns the current queue size.
    virtual UInt32 METHOD(GetQueueSize)() = 0;

    /// Returns the number of currently running tasks
    virtual UInt32 METHOD(GetRunningTaskCount)() const = 0;


    /// Stops all worker threads.

    /// his method makes all worker threads to exit.
    /// If an application enqueues tasks after calling this methods,
    /// this tasks will never run.
    virtual void METHOD(StopThreads)() = 0;


    /// Manually processes the next task from the queue.

    /// \param[in] ThreadId    - Id of the thread that is running this task.
    /// \param[in] WaitForTask - whether the function should wait for the next task:
    ///                          - if true, the function will block the thread until the next task
    ///                            is retrieved from the queue and processed.
    ///                          - if false, the function will return immediately if there are no
    ///                            tasks in the queue.
    ///
    /// \return     Whether there are more tasks to process. The calling thread must keep
    ///             calling the function until it returns false.
    ///
    /// This method allows an application to implement its own threading strategy.
    /// A thread pool may be created with zero threads, and the application may call
    /// ProcessTask() method from its own threads.
    ///
    /// An application must keep calling the method until it returns false.
    /// If there are unhandled tasks in the queue and the application stops processing
    /// them, the thread pool will hang up.
    ///
    /// An example of handling the tasks is shown below:
    ///
    ///     // Initialization
    ///     auto pThreadPool = CreateThreadPool(ThreadPoolCreateInfo{0});
    ///
    ///     std::vector<std::thread> WorkerThreads(4);
    ///     for (UInt32 i =0; i < WorkerThreads.size(); ++i)
    ///     {
    ///         WorkerThreads[i] = std::thread{
    ///             [&ThreadPool = *pThreadPool, i] //
    ///             {
    ///                 while (ThreadPool.ProcessTask(i, true))
    ///                 {
    ///                 }
    ///             }};
    ///     }
    ///
    ///     // Enqueue async tasks
    ///
    ///     pThreadPool->WaitForAllTasks();
    ///
    ///     // Stop all threads in the pool
    ///     pThreadPool->StopThreads();
    ///
    ///     // Cleanup (must be done after all threads are stopped)
    ///     for (auto& Thread : WorkerThreads)
    ///     {
    ///         Thread.join();
    ///     }
    ///
    virtual bool DG_CALL_TYPE ProcessTask(UInt32 ThreadId, bool WaitForTask) = 0;
};

} // namespace Diligent
