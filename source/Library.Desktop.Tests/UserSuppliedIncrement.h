#pragma once

#include <cstddef>
template <typename T>
struct UserIncrement final
{
	std::size_t operator()(std::size_t capacity, std::size_t size);
};

template<typename T>
inline std::size_t UserIncrement<T>::operator()(std::size_t capacity, std::size_t size)
{
	size;
	return capacity + 10;
}