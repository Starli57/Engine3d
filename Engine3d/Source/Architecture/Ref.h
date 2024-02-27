#pragma once

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>; 

template<typename T>
using URef = std::unique_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr URef<T> CreateUniqueRef(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr WeakRef<T> CreateWeakRef(Args&& ... args)
{
	return std::weak_ptr<T>(std::forward<Args>(args)...);
}
