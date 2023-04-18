#include "Factory.h"

#pragma once

namespace FieaGameEngine
{
	template <typename T>
	inline const Factory<T>* Factory<T>::Find(const std::string& className)
	{
		auto position = _factories.Find(className);
		if (position != _factories.end())
		{
			return position->second.get();
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	inline std::unique_ptr<T> Factory<T>::Create(const std::string& className)
	{
		auto factory = Find(className);
		if (factory != nullptr)
		{
			return factory->Create();
		}
		else
		{
			return std::unique_ptr<T>{ nullptr };
		}
	}

	template<typename T>
	inline void Factory<T>::Clear()
	{
		_factories.Clear();
	}

	template<typename T>
	inline void Factory<T>::Add(std::unique_ptr<const Factory> concreteFactory)
	{
		if (!_factories.ContainsKey(concreteFactory->ClassName()))
		{
			_factories.Emplace(concreteFactory->ClassName(), std::move(concreteFactory));
		}
	}

	template<typename T>
	inline void Factory<T>::Remove(const std::string& className)
	{
		_factories.Remove(className);
	}
}

