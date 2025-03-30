#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include <typeindex>

// TODO: when we want to implement multi-threading of systems, create a ResourceMut class


template<typename T>
class Resource {
public:
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
	}

	// TODO: return optional
	template<typename T>
	Resource<T> get() {
		return Resource<T>(static_cast<T*>(_resources[std::type_index(typeid(T))].get()));
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> _resources;

};

#endif //RESOURCE_HPP
