#ifndef UTILS_HPP
#define UTILS_HPP

#include <variant>

template<typename T, typename E>
class Result {
public:
	// Delete default constructor
	Result() = delete;

	// Static methods to create results
	static Result Ok(T value) { return Result(value); };
	static Result Error(E error) { return Result(error); };

	[[nodiscard]] T ok() { return std::get<T>(_value); }
	[[nodiscard]] E error() { return std::get<E>(_value); }

	[[nodiscard]] bool is_ok() const { return std::holds_alternative<T>(_value); }
	[[nodiscard]] bool is_error() const { return !is_ok(); }

private:
	std::variant<T, E> _value;

	static_assert(!std::is_same<T, E>::value, "T and E cannot be the same type.");

	// Private constructors for Ok and Error cases
	explicit Result(T value) : _value(value) {}
	explicit Result(E error) : _value(error) {}
};

template<typename E>
class Error {
public:
	Error(E value) : value(value) {};
	E value;
};

#endif //UTILS_HPP
