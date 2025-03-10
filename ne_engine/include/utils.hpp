#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
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

#endif //UTILS_HPP
