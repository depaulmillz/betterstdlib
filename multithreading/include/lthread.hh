/**
 * @author dePaul Miller
 * This is is to provide a type of thread that will repeat a function until it is signaled
 * to stop
 */

#include <thread>
#include <atomic>
#include <memory>
#include <functional>

#ifndef BETTERSTDLIB_LTHREAD_HH
#define BETTERSTDLIB_LTHREAD_HH

namespace betterstd {

    /**
     * lthread is a class to provide a type of thread that will repeat a function until it is signaled
     */
    class lthread {
    public:

        /**
         * betterstd::lthread::id is the same as std::thread::id
         */
        typedef std::thread::id id;

        /**
         * betterstd::lthread::native_handle_type is the same as std::thread::native_handle_type
         */
        typedef std::thread::native_handle_type native_handle_type;

        /**
         * Creates a new lthread object which does not represent a thread
         */
        lthread() noexcept: _thread(), shouldFinish(nullptr) {
        }

        /**
         * Creates a new lthread object from another lthread object, which transitions the
         * execution of the thread from other to this. Afterwards, other no longer represents
         * a thread of execution
         * @param other
         */
        lthread(lthread &&other) noexcept: _thread(std::move(other._thread)), shouldFinish(std::move(other.shouldFinish)) {
        }

        /**
         * Initializes a thread running f with arguments args.
         * @tparam Function
         * @tparam Args
         * @param f
         * @param args
         */
        template<class Function, class... Args>
        explicit lthread(Function &&f, Args &&... args) : shouldFinish(std::make_unique<std::atomic<bool>>(false)) {
            auto g = std::bind(std::forward<Function>(f), std::forward<Args>(args)...);

            _thread = std::thread([g, this]() {
                while (!this->shouldFinish->load())
                    g();
            });
        }

        lthread(const lthread &) = delete;

        /**
         * Joins the thread of execution if it is joinable
         */
        ~lthread() {
            shouldFinish->store(true);
            if (_thread.joinable()) {
                _thread.join();
            }
        }

        /**
         * Moves the execution of the thread from other to this. Other no longer
         * represents a thread of execution.
         * @param other
         * @return
         */
        lthread &operator=(lthread &&other) noexcept {
            this->_thread = std::move(other._thread);
            shouldFinish = std::move(other.shouldFinish);
            return *this;
        }

        /**
         * Returns a boolean of whether the thread is joinable or not.
         * @return
         */
        [[nodiscard]] bool joinable() const noexcept {
            return _thread.joinable();
        }

        /**
         * Returns the id associated with the thread.
         * @return
         */
        [[nodiscard]] lthread::id get_id() const noexcept {
            return _thread.get_id();
        }

        /**
         * Returns the native handle of the thread.
         * @return
         */
        [[nodiscard]] native_handle_type native_handle() {
            return _thread.native_handle();
        }

        /**
         * Returns the hardware concurrency of the system if implemented.
         * @return
         */
        [[nodiscard]] static unsigned int hardware_concurrency() noexcept {
            return std::thread::hardware_concurrency();
        }

        /**
         * Joins the thread.
         */
        void join() {
            shouldFinish->store(true);
            _thread.join();
        }

        /**
         * Detaches the thread.
         */
        void detach() {
            _thread.detach();
        }

        /**
         * Swaps this thread with other's thread.
         * @param other
         */
        void swap(lthread &other) noexcept {
            _thread.swap(other._thread);
            shouldFinish.swap(other.shouldFinish);
        }

        /**
         * Swaps two lthreads
         * @param lhs
         * @param rhs
         */
        friend void swap(betterstd::lthread &lhs, betterstd::lthread &rhs) noexcept;

    private:

        std::thread _thread;
        std::unique_ptr<std::atomic<bool>> shouldFinish;

    };

    /**
     * Swaps two lthreads
     * @param lhs
     * @param rhs
     */
    void swap(lthread &lhs, lthread &rhs) noexcept {
        lhs.swap(rhs);
    }

} // namespace betterstd


#endif //BETTERSTDLIB_LTHREAD_HH
