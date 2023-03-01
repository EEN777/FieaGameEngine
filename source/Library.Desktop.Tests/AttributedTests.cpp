#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "Foo.h"
#include "Bar.h"
#include "Attributed.h"
#include "TypeManager.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace LibraryDesktopTests
{
	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(TypeManager)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;
			foo;
			TypeManager::DestroyInstance();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}