#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "Foo.h"
#include "Stack.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace LibraryDesktopTests
{
	TEST_CLASS(StackTests)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor)
		{
			Stack<Foo> stack{};
			Assert::AreEqual(std::size_t{ 0 }, stack.Size());
		}

		TEST_METHOD(PushAndPop)
		{
			Stack<Foo> stack{};
			stack.Push(Foo{ 1 });
			Assert::AreEqual(Foo{ 1 }, stack.Top());
			stack.Push(Foo{ 2 });
			Assert::AreEqual(Foo{ 2 }, stack.Top());
			stack.Pop();
			Assert::AreEqual(Foo{ 1 }, stack.Top());
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}