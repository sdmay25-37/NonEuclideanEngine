#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

// TODO: when we want to implement multi-threading of systems, create a ResourceMut class

template<typename T>
class Resource {
public:
	using value_type = T;

	explicit Resource(T* instance) : _instance(instance) {}

	T* operator->() { return _instance; }
	T& operator*() { return *_instance; }
	const T* operator->() const { return _instance; }
	const T& operator*() const { return *_instance; }

private:
	T* _instance;
};

class ResourceManager {
public:
	template<typename T, typename... Args>
	void insert(Args&&... args) {
		_resources[std::type_index(typeid(T))] = std::make_unique<T>(std::forward<Args>(args)...);
		_insertion_order.emplace_back(typeid(T));
	}

	// TODO: return optional
	template<typename T>
	Resource<T> get() {
		return Resource<T>(static_cast<T*>(_resources[std::type_index(typeid(T))].get()));
	}

	~ResourceManager() {
		for(auto it = _insertion_order.rbegin(); it != _insertion_order.rend(); ++it) {
			_resources.erase(*it);
		}
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> _resources;
	std::vector<std::type_index> _insertion_order;

};

#endif //RESOURCE_HPP
