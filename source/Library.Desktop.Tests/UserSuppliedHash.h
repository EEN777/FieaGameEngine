#pragma once

#include <cstddef>
#include "Foo.h"
#include "Bar.h"

template <typename T>
struct UserHash final
{
	std::size_t operator()(const T&);
};

template<>
inline std::size_t UserHash<UnitTests::Foo>::operator()(const UnitTests::Foo& foo)
{
	return foo.Data();
}

template<>
inline std::size_t UserHash<UnitTests::Bar>::operator()(const UnitTests::Bar& bar)
{
	return bar.Data();
}