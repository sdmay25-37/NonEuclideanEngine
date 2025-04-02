#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Resource.hpp"
#include "utils.hpp"

class SystemBase {
public:
	virtual void Invoke(entt::registry& registry, ResourceManager& resource_manager) const = 0;
	virtual ~SystemBase() = default;
};

template<typename... Args>
class System final : public SystemBase {
public:
	explicit System(std::function<void(Args...)> func) : _func(func) {}

	void Invoke(entt::registry& registry, ResourceManager& resource_manager) const override {
		std::tuple<Args...> args = std::make_tuple(GetArg<Args>(registry, resource_manager)...);
		std::apply(_func, args);
	}

private:
	std::function<void(Args...)> _func;

	template<typename T>
	auto GetArg(entt::registry& registry, ResourceManager& resource_manager) const {
		if constexpr(std::is_same_v<T, entt::registry&>) {
			return std::ref(registry);
		} else {
			return resource_manager.get<typename T::value_type>();
		}
	}
};


// Voodoo template magic
template<typename F, typename Tuple, std::size_t... Is>
static auto CreateSystemImpl(F&& func, std::index_sequence<Is...>) {
	using SystemType = System<std::tuple_element_t<Is, Tuple>...>;
	auto wrappedFunc = std::function<void(std::tuple_element_t<Is, Tuple>...)>(
		std::forward<F>(func)
	);
	return std::make_unique<SystemType>(wrappedFunc);
}

template<typename F>
static auto CreateSystem(F&& func) {
	using ArgsTuple = typename FunctionTraits<F>::arguments;
	return CreateSystemImpl<F, ArgsTuple>(
		std::forward<F>(func),
		std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
	);
}



#endif //SYSTEM_HPP
