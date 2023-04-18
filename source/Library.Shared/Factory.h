#pragma once

#include <memory>
#include "HashMap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A Templated class that follows the factory design pattern. Each templated version of this class serves to describe a family of possible factories to call from.
	/// </summary>
	/// <typeparam name="T">The family type of the factory.</typeparam>
	template<typename T>
	class Factory
	{
		inline static HashMap<std::string, std::unique_ptr<const Factory>> _factories{};
	public:
		/// <summary>
		/// Attempts to find a factory given a class name. Will return nullptr if not found.
		/// </summary>
		/// <param name="className">The name of the class the found factory should handle.</param>
		/// <returns>A pointer to a Factory of a given family type.</returns>
		static const Factory* Find(const std::string& className);
		/// <summary>
		/// Attempts to create an object of the given class name. Will return nullptr if not possible.
		/// </summary>
		/// <param name="className">The name of the class to insantiate.</param>
		/// <returns>A unique_ptr to the instantiated class.</returns>
		static std::unique_ptr<T> Create(const std::string& className);
		/// <summary>
		/// Clears the added factories, thus deleting them.
		/// </summary>
		static void Clear();
		/// <summary>
		/// Pure Virtual function, used to create objects of a given class in concrete factories.
		/// </summary>
		/// <returns>A unique_ptr to an instantiated object of the desired class.</returns>
		virtual std::unique_ptr<T> Create() const = 0;
		/// <summary>
		/// Pure Virtual function that is meant to return the class name of a concrete factory.
		/// </summary>
		/// <returns>The name of the class associated with the given factory.</returns>
		virtual const std::string& ClassName() const = 0;
		virtual ~Factory() = default;
		/// <summary>
		/// Adds a given concrete factory.
		/// </summary>
		/// <param name="concreteFactory">Factory to add.</param>
		static void Add(std::unique_ptr<const Factory> concreteFactory);
		/// <summary>
		/// Removes a given concrete factory.
		/// </summary>
		/// <param name="concreteFactory">Factory to remove.</param>
		static void Remove(const std::string& className);
	};
}

#include "Factory.inl"
//A Macro that handles creation of factories for a given class by overriding the base class pure virtual functions.
#define ConcreteFactory(ConcreteProductType, AbstractProductType) \
	class ConcreteProductType ## Factory final : public FieaGameEngine::Factory<AbstractProductType>		\
{																	\
	public:															\
	const std::string _className { #ConcreteProductType };			\
	std::unique_ptr<AbstractProductType> Create() const override { return std::make_unique<ConcreteProductType>();};	\
	const std::string& ClassName() const override { return _className; };	\
}
		
