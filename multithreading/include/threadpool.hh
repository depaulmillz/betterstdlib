#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include "lthread.hh"
#include <vector>
#include <queue>
#include <stack>

#ifndef BETTERSTDLIB_THREADPOOL_HH
#define BETTERSTDLIB_THREADPOOL_HH

namespace betterstd {

    /**
     * Thread pool
     */
    class threadpool {
    public:

        threadpool() = delete;

        /**
         * Constructs a thread pool of the given size
         * @param size
         */
        threadpool(int size) {
            for (int i = 0; i < size; i++) {
                threads.push(new lthread([this]() { this->thread_execution(); }));
            }
        }

        threadpool(const threadpool &) = delete;

        threadpool(threadpool &&other) = delete;

        /**
         * Deletes the thread pool. It will break promises if there are still ongoing tasks.
         */
        ~threadpool() {
            while (!threads.empty()) {
                auto l = threads.top();
                threads.pop();
                delete l;
            }
        }

        /**
         * Returns the size of the thread pool
         * @return
         */
        int size() { return threads.size(); }

        /**
         * Returns the elements left in the queue
         * @return
         */
        int queueSize() {
            mtx.lock();
            int size = tasks.size();
            mtx.unlock();
            return size;
        }

        /**
         * Adds a task to the thread pool returning a future
         * @tparam R
         * @tparam Args
         * @param f
         * @param args
         * @return
         */
        template<typename Function, typename... Args>
        std::future<typename std::result_of<Function(Args...)>::type> add_task(Function f, Args... args) {

            using returnType = typename std::result_of<Function(Args...)>::type;

            std::shared_ptr<std::packaged_task<returnType()>> t = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<Function>(f), std::forward<Args>(args)...));

            std::future<returnType> fut = t->get_future();

            mtx.lock();
            tasks.push([t](){
                (*t)();
            });
            mtx.unlock();

            return fut;
        }

    private:
        std::stack<lthread *> threads;
        std::mutex mtx;
        std::queue<std::function<void()>> tasks;

        void thread_execution() {
            mtx.lock();
            if (!tasks.empty()) {
                auto task = tasks.front();
                tasks.pop();
                mtx.unlock();
                task();
            } else {
                mtx.unlock();
            }
        }

    };

} // namespace betterstd



#endif //BETTERSTDLIB_THREADPOOL_HH
