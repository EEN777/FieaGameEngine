#pragma once

#include <cstddef>

template<typename T>
struct DefaultEquality final
{
	bool operator()(const T&, const T&) const;
	bool operator()(const T*, const T*) const;
};

template<typename T>
inline bool DefaultEquality<T>::operator()(const T & lhs, const T & rhs) const
{
	return lhs == rhs;
}

template <typename T>
inline bool DefaultEquality<T>::operator()(const T* lhs, const T* rhs) const
{
	return lhs == rhs;
}

template<>
inline bool DefaultEquality<char>::operator()(const char* lhs, const char* rhs) const
{
	return strcmp(lhs, rhs);
}

