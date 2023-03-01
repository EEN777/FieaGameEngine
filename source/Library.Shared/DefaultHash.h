#pragma once

#include <cstddef>
#include <cassert>

namespace FieaGameEngine
{
	template <typename T>
	struct DefaultHash final
	{
		std::size_t operator()(const T&) const;
		std::size_t operator()(const T*) const;
	};

	template<typename T>
	inline std::size_t DefaultHash<T>::operator()(const T& t) const
	{
		const auto hashPrime{ 7 };
		std::size_t hash{};
		const std::byte* data = reinterpret_cast<const std::byte*>(&t);
		auto size = sizeof(T);
		for (std::size_t index{ 0 }; index < size; ++index)
		{
			hash += (static_cast<std::size_t>(static_cast<std::size_t>(data[index]) * hashPrime));
		}
		return hash;
	}

	template<>
	inline std::size_t DefaultHash<int>::operator()(const int& t) const
	{
		return static_cast<std::size_t>(t);
	}

	template<>
	inline std::size_t DefaultHash<char>::operator()(const char* t) const
	{
		assert(t != nullptr);
		std::size_t total{};
		const char* temp = t;
		while (*t != '\0')
		{
			total += static_cast<std::size_t>(*t);
			++t;
		}
		t = temp;
		return total;
	}

	template<>
	inline std::size_t DefaultHash<std::string>::operator()(const std::string& t) const
	{
		std::size_t total{};
		for (auto& character : t)
		{
			total += static_cast<std::size_t>(character);
		}
		return total;
	}
 }