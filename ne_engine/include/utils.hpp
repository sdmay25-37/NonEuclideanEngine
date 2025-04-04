#ifndef UTILS_HPP
#define UTILS_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <variant>


template<typename T, typename E>
class Result {
public:
	// Delete default constructor
	Result() = delete;

	// Static methods to create results
	static Result Ok(T value) { return Result(std::move(value)); };
	static Result Error(E error) { return Result(std::move(error)); };

	[[nodiscard]] T ok() { return std::get<T>(std::move(_value)); }
	[[nodiscard]] E error() { return std::get<E>(std::move(_value)); }

	[[nodiscard]] bool is_ok() const { return std::holds_alternative<T>(_value); }
	[[nodiscard]] bool is_error() const { return !is_ok(); }

private:
	std::variant<T, E> _value;

	static_assert(!std::is_same<T, E>::value, "T and E cannot be the same type.");

	// Private constructors for Ok and Error cases
	explicit Result(T value) : _value(std::move(value)) {}
	explicit Result(E error) : _value(std::move(error)) {}
};

// Todo:  better error type, allow for layered messages
template<typename E>
class Error {
public:
	Error(E value) : value(value) {};
	E value;
};


class Random {
public:
	static int integer(int min, int max) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}
};

class Signal {
public:
	Signal() : _fired(false) {}

	void fire() {
		std::lock_guard lock(_mtx);
		_fired = true;
		_cv.notify_all();
	}

	void wait() {
		std::unique_lock lock(_mtx);
		_cv.wait(lock, [this] { return _fired; });
	}

private:
	std::mutex _mtx;
	std::condition_variable _cv;
	bool _fired;
};

class Synchronizer {
public:
	explicit Synchronizer(const unsigned int threads)
	: _total(threads), _waiting(0), _cycle(0) {}

	void wait() {
		std::unique_lock lock(_mtx);
		unsigned char current_cycle = _cycle;

		_waiting++;
		if(_waiting == _total) {
			_waiting = 0;
			_cycle++;
			_cv.notify_all();
		} else {
			_cv.wait(lock, [this, current_cycle] { return _cycle != current_cycle; });
		}
	}

private:
	unsigned int _total;
	unsigned int _waiting;
	unsigned char _cycle;

	std::mutex _mtx;
	std::condition_variable _cv;
};

class ThreadPool {
public:
	explicit ThreadPool(size_t threads);
	~ThreadPool();

	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;


private:
	std::vector<std::thread> _threads;
	std::queue<std::function<void()>> _tasks;

	std::mutex _mtx;
	std::condition_variable _cv;
	bool _stop;

};

inline ThreadPool::ThreadPool(const size_t threads)
	: _stop(false) {

	for(size_t i = 0; i < threads; i++) {
		_threads.emplace_back([this] {
			while(true) {
				std::function<void()> task;

				{
					std::unique_lock lock(_mtx);
					_cv.wait(lock, [this] { return _stop || !_tasks.empty(); });

					if(_stop && _tasks.empty()) return;

					task = std::move(_tasks.front());
					_tasks.pop();
				}

				task();
			}
		});
	}
}

inline ThreadPool::~ThreadPool() {
	{
		std::unique_lock lock(_mtx);
		_stop = true;
		_cv.notify_all();
	}

	for(std::thread& thread : _threads) {
		thread.join();
	}
}

template<class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&... args) -> std::future<std::result_of_t<F(Args...)>> {
	using return_type = std::result_of_t<F(Args...)>;

	auto task = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
	);

	std::future<return_type> result = task->get_future();

	{
		std::unique_lock lock(_mtx);
		_tasks.emplace([task]() { (*task)(); });
		_cv.notify_one();
	}

	return result;
}

template <class Enum, class T>
struct EnumArray : std::array<T, static_cast<std::size_t>(Enum::MAX_VALUE)> {
	using Base = std::array<T, static_cast<std::size_t>(Enum::MAX_VALUE)>;

	T& operator[](const Enum& i) {
		return Base::operator[](static_cast<std::size_t>(i));
	}

	const T& operator[](const Enum& i) const {
		return Base::operator[](static_cast<std::size_t>(i));
	}
};


template<typename F> class FunctionTraits;

template<typename Result, typename... Args>
class FunctionTraits<Result(Args...)> {
public:
	using arguments = std::tuple<Args...>;
};

template<typename Result, typename... Args>
class FunctionTraits<Result(&)(Args...)> {
public:
	using arguments = std::tuple<Args...>;
};


inline uint32_t NextPowerOfTwo(uint32_t n) {
	if(n == 0) return 0;
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	return n + 1;
}

#endif //UTILS_HPP
