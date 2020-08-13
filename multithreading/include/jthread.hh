/**
 * @author dePaul Miller
 * This is is to provide some jthread functionality in C++11
 */

#include <thread>

#ifndef BETTERSTDLIB_JTHREAD_HH
#define BETTERSTDLIB_JTHREAD_HH

namespace betterstd {

    /**
     * jthread is a class to allow for automatic joining of threads on deletion
     */
    class jthread {
    public:

        /**
         * betterstd::jthread::id is the same as std::thread::id
         */
        typedef std::thread::id id;

        /**
         * betterstd::jthread::native_handle_type is the same as std::thread::native_handle_type
         */
        typedef std::thread::native_handle_type native_handle_type;

        /**
         * Creates a new jthread object which does not represent a thread
         */
        jthread() noexcept: _thread() {
        }

        /**
         * Creates a new jthread object from another jthread object, which transitions the
         * execution of the thread from other to this. Afterwards, other no longer represents
         * a thread of execution
         * @param other
         */
        jthread(jthread &&other) noexcept: _thread(std::move(other._thread)) {
        }

        /**
         * Initializes a thread running f with arguments args.
         * @tparam Function
         * @tparam Args
         * @param f
         * @param args
         */
        template<class Function, class... Args>
        explicit jthread(Function &&f, Args &&... args) : _thread(f, args...) {
        }

        jthread(const jthread &) = delete;

        /**
         * Joins the thread of execution if it is joinable
         */
        ~jthread() {
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
        jthread &operator=(jthread &&other) noexcept {
            this->_thread = std::move(other._thread);
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
        [[nodiscard]] jthread::id get_id() const noexcept {
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
        void swap(jthread &other) noexcept {
            _thread.swap(other._thread);
        }

        /**
         * Swaps two jthreads
         * @param lhs
         * @param rhs
         */
        friend void swap(betterstd::jthread &lhs, betterstd::jthread &rhs) noexcept;

    private:

        std::thread _thread;

    };

    /**
     * Swaps two jthreads
     * @param lhs
     * @param rhs
     */
    void swap(jthread &lhs, jthread &rhs) noexcept {
        lhs.swap(rhs);
    }

} // namespace betterstd

#endif //BETTERSTDLIB_JTHREAD_HH
