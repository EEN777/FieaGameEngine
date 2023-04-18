#pragma once

#include "CppUnitTest.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "glm/glm.hpp"
#include <glm/gtx/string_cast.hpp>
#include <codecvt>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<UnitTests::AttributedFoo>(const UnitTests::AttributedFoo& t)
	{
		auto string = t.ToString();
		return std::wstring(string.begin(), string.end());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Foo>::Iterator>(const FieaGameEngine::SList<UnitTests::Foo>::Iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<const char*>::Iterator>(const FieaGameEngine::SList<const char*>::Iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Foo>::ConstIterator>(const FieaGameEngine::SList<UnitTests::Foo>::ConstIterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<const char*>::ConstIterator>(const FieaGameEngine::SList<const char*>::ConstIterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Foo>::Iterator>(const FieaGameEngine::Vector<UnitTests::Foo>::Iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<const char*>::Iterator>(const FieaGameEngine::Vector<const char*>::Iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Foo>::ConstIterator>(const FieaGameEngine::Vector<UnitTests::Foo>::ConstIterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<const char*>::ConstIterator>(const FieaGameEngine::Vector<const char*>::ConstIterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}

		catch (const std::exception&)
		{
			value = L"end()";
		}

		return value;
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& vec)
	{
		std::wstring string = L"X: " + std::to_wstring(vec.x);
		string.append(L" Y: " + std::to_wstring(vec.y));
		string.append(L" Z: " + std::to_wstring(vec.z));
		string.append(L" W: " + std::to_wstring(vec.w));

		return string;
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& mat)
	{
		std::string string{ glm::to_string(mat) };
		return std::wstring{ string.begin(), string.end() };
	}
}