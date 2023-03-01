#pragma once

#include <cstddef>

namespace FieaGameEngine
{
	template <typename T>
	struct DefaultIncrement final
	{
		std::size_t operator()(std::size_t capacity, std::size_t size) const;
		std::size_t operator()(const T&) const;
	};

	template<typename T>
	inline std::size_t DefaultIncrement<T>::operator()(std::size_t capacity, std::size_t size) const
	{
		size;
		return capacity + 5;
	}
	template<typename T>
	inline std::size_t DefaultIncrement<T>::operator()(const T& t) const
	{
		return std::size_t(7);
	}
	template<>
	inline std::size_t DefaultIncrement<int>::operator()(const int& t) const
	{
		return static_cast<std::size_t>(t);
	}

}