#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "fstream"
#include "Foo.h"
#include "Stack.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "AttributedFoo.h"
#include "Factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(FactoryTests)
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

		TEST_METHOD(FindAndCreate)
		{
			{
				auto foundFactory = Factory<Scope>::Find("Null");
				Assert::IsNull(foundFactory);
				auto product = Factory<Scope>::Create("Null");
				Assert::IsNull(product.get());
			}

			{
				TypeManager::CreateInstance();
				auto _typeManager = TypeManager::Instance();
				_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());

				Factory<Scope>::Add(std::make_unique<const AttributedFooFactory>());

				auto foundFactory = Factory<Scope>::Find("AttributedFoo");
				Assert::AreEqual("AttributedFoo"s, foundFactory->ClassName());
				Assert::IsNotNull(foundFactory);
				auto attributedFoo = Factory<Scope>::Create("AttributedFoo");
				Assert::IsTrue(attributedFoo->Is(AttributedFoo::TypeIdClass()));
				Assert::AreEqual(0, attributedFoo->As<AttributedFoo>()->externalInteger);

				Factory<Scope>::Clear();
				TypeManager::DestroyInstance();
			}
		}

		TEST_METHOD(ParseCreation)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());

			Factory<Scope>::Add(std::make_unique<const AttributedFooFactory>());

			std::string inputString = R"({ "AttributedFoo foo": {
												"ExternalInteger" : 5,
												"ExternalFloat" : 7.2,
												"ExternalString" : "String",
												"ExternalVector" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"ExternalMatrix" : " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) ",
												"ExternalIntegerArray" : [1,2,3,4,5],
												"ExternalFloatArray" : [1.2, 1.3, 1.4, 1.5, 1.6],
												"ExternalStringArray" : ["String1", "String2", "String3", "String4", "String5"],
												"ExternalVectorArray" : [" vec4(1.2, 3.2, 4.7, 0.2) ", " vec4(1.2, 3.2, 4.7, 0.2) ", " vec4(1.2, 3.2, 4.7, 0.2) ", " vec4(1.2, 3.2, 4.7, 0.2) ", " vec4(1.2, 3.2, 4.7, 0.2) "],
												"ExternalMatrixArray" : [" mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) ", " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) ", " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) ", " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) ", " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) "],
										  } })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			AttributedFoo& attributedFoo = *rawWrapper->_scope->As<AttributedFoo>();

			Assert::AreEqual(5, attributedFoo.externalInteger);
			Assert::AreEqual(7.2f, attributedFoo.externalFloat);
			Assert::AreEqual("String"s, attributedFoo.externalString);
			Assert::AreEqual(glm::vec4{1.2, 3.2, 4.7, 0.2}, attributedFoo.externalVector);
			Assert::AreEqual(glm::mat4{ glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2) }, attributedFoo.externalMatrix);
			Assert::AreEqual(1, attributedFoo.externalIntegerArray[0]);
			Assert::AreEqual(2, attributedFoo.externalIntegerArray[1]);
			Assert::AreEqual(3, attributedFoo.externalIntegerArray[2]);
			Assert::AreEqual(4, attributedFoo.externalIntegerArray[3]);
			Assert::AreEqual(5, attributedFoo.externalIntegerArray[4]);
			Assert::AreEqual(1.2f, attributedFoo.externalFloatArray[0]);
			Assert::AreEqual(1.3f, attributedFoo.externalFloatArray[1]);
			Assert::AreEqual(1.4f, attributedFoo.externalFloatArray[2]);
			Assert::AreEqual(1.5f, attributedFoo.externalFloatArray[3]);
			Assert::AreEqual(1.6f, attributedFoo.externalFloatArray[4]);
			Assert::AreEqual("String1"s, attributedFoo.externalStringArray[0]);
			Assert::AreEqual("String2"s, attributedFoo.externalStringArray[1]);
			Assert::AreEqual("String3"s, attributedFoo.externalStringArray[2]);
			Assert::AreEqual("String4"s, attributedFoo.externalStringArray[3]);
			Assert::AreEqual("String5"s, attributedFoo.externalStringArray[4]);
			Assert::AreEqual(glm::vec4{ 1.2, 3.2, 4.7, 0.2 }, attributedFoo.externalVectorArray[0]);
			Assert::AreEqual(glm::vec4{ 1.2, 3.2, 4.7, 0.2 }, attributedFoo.externalVectorArray[1]);
			Assert::AreEqual(glm::vec4{ 1.2, 3.2, 4.7, 0.2 }, attributedFoo.externalVectorArray[2]);
			Assert::AreEqual(glm::vec4{ 1.2, 3.2, 4.7, 0.2 }, attributedFoo.externalVectorArray[3]);
			Assert::AreEqual(glm::vec4{ 1.2, 3.2, 4.7, 0.2 }, attributedFoo.externalVectorArray[4]);
			Assert::AreEqual(glm::mat4{ glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2) }, attributedFoo.externalMatrixArray[0]);
			Assert::AreEqual(glm::mat4{ glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2) }, attributedFoo.externalMatrixArray[1]);
			Assert::AreEqual(glm::mat4{ glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2) }, attributedFoo.externalMatrixArray[2]);
			Assert::AreEqual(glm::mat4{ glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2) }, attributedFoo.externalMatrixArray[3]);
			Assert::AreEqual(glm::mat4{ glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2), glm::vec4(1.2, 3.2, 4.7, 0.2) }, attributedFoo.externalMatrixArray[4]);
			attributedFoo;
			
			Factory<Scope>::Remove("AttributedFoo");
			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}