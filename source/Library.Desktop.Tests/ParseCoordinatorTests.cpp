#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "fstream"
#include "Foo.h"
#include "Stack.h"
#include "JsonParseCoordinator.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(ParseCoordinatorTests)
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

		TEST_METHOD(Construction)
		{
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());
		}

		TEST_METHOD(IntegerParsing)
		{
			std::string inputString = R"({ "integer": 100 })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());
		}

		TEST_METHOD(Clone)
		{
			std::string inputString = R"({ "integer": 100 })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

			auto clonedCoordinator = parseCoordinator.Clone();

			clonedCoordinator->Initialize();

			clonedCoordinator->DeserializeObject(inputString);

			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(clonedCoordinator->GetWrapper().get());
			Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				std::string inputString = R"({ "integer": 100 })";

				std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator{ wrapper };
				parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

				auto movedCoordinator{ std::move(parseCoordinator) };

				movedCoordinator.Initialize();

				movedCoordinator.DeserializeObject(inputString);

				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());
			}

			{
				std::string inputString = R"({ "integer": 100 })";

				std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator{ wrapper };
				parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

				auto movedCoordinator = std::move(parseCoordinator);

				movedCoordinator.Initialize();

				movedCoordinator.DeserializeObject(inputString);

				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());
			}
		}

		TEST_METHOD(SetWrapper)
		{
			std::string inputString = R"({ "integer": 100 })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> anotherWrapper = std::make_shared<JsonTestParseHelper::Wrapper>();

			parseCoordinator.SetWrapper(anotherWrapper);

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(anotherWrapper.get());
			Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());
		}

		TEST_METHOD(RemoveHelper)
		{
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			auto helper = std::make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(helper);
			parseCoordinator.RemoveHelper(helper);

			std::string inputString = R"({ "integer": 100 })";

			parseCoordinator.Initialize();

			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::IsTrue(rawWrapper->_scope.IsEmpty());

		}

		TEST_METHOD(IntegerArrayParsing)
		{
			std::string inputString = R"({ "integer": [100, 200, 300] })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };

			Assert::IsTrue(wrapper == parseCoordinator.GetWrapper());
			Assert::IsTrue(wrapper->GetJsonParseCoordinator() == &parseCoordinator);

			auto helper = std::make_shared<JsonTestParseHelper>();
			Assert::IsFalse(helper->InitializeCalled);
			parseCoordinator.AddHelper(helper);
			Assert::IsTrue(helper->InitializeCalled);

			parseCoordinator.Initialize();

			Assert::AreEqual(std::size_t{ 0 }, helper->StartHandlerCount);
			Assert::AreEqual(std::size_t{ 0 }, helper->EndHandlerCount);

			Assert::IsFalse(helper->CleanupCalled);

			parseCoordinator.DeserializeObject(inputString);
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(100, rawWrapper->_scope["integer"].GetAsInt());
			Assert::AreEqual(200, rawWrapper->_scope["integer"].GetAsInt(1));
			Assert::AreEqual(300, rawWrapper->_scope["integer"].GetAsInt(2));

			Assert::AreEqual(std::size_t{ 1 }, rawWrapper->MaxDepth);
			Assert::AreEqual(std::size_t{ 0 }, rawWrapper->Depth());
			
			Assert::AreEqual(std::size_t{ 4 }, helper->StartHandlerCount);
			Assert::AreEqual(std::size_t{ 4 }, helper->EndHandlerCount);

			Assert::IsTrue(helper->CleanupCalled);
		}

		TEST_METHOD(TableHelper)
		{
			const std::string filename = "TestFile.json";
			const std::string inputString =
				R"({"Scope Enemy": {
						"Health": 100,
						"Position": [2.7, 3.2, 7.3, 0.1],
						"Name": "Bob",
						"Aliases": [ "Billy", "Bobby", "Rob" ],
						"Location": " vec4(1.2, 3.2, 4.7, 0.2) ",
						"PreviousLocations": " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) ",
						"Scope Enemy": {
							"Health": 150,
							"Position": [0.1, 0.2, 0.3, 0.4],
							"Name": "Ricky",
							"Aliases": [ "Rick", "Rude", "Rob" ],
							"Location": " vec4(3.2, 5.2, 7.7, 3.2) ",
							"PreviousLocations": " mat4x4((1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2), (1.2, 3.2, 4.7, 0.2)) "
							}
					}})";
			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());

			outputFile << inputString;
			outputFile.close();

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };

			Assert::IsTrue(wrapper == parseCoordinator.GetWrapper());
			Assert::IsTrue(wrapper->GetJsonParseCoordinator() == &parseCoordinator);

			auto helper = std::make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
            assert(rawWrapper != nullptr);
			Scope& enemy = *rawWrapper->_scope;
			Assert::AreEqual(100, enemy["Health"].GetAsInt());
			Assert::AreEqual(2.7f, enemy["Position"].GetAsFloat());
			Assert::AreEqual(3.2f, enemy["Position"].GetAsFloat(1));
			Assert::AreEqual(7.3f, enemy["Position"].GetAsFloat(2));
			Assert::AreEqual(0.1f, enemy["Position"].GetAsFloat(3));
			Assert::AreEqual("Bob"s, enemy["Name"].GetAsString());
			Assert::AreEqual("Billy"s, enemy["Aliases"].GetAsString());
			Assert::AreEqual("Bobby"s, enemy["Aliases"].GetAsString(1));
			Assert::AreEqual("Rob"s, enemy["Aliases"].GetAsString(2));
			Assert::AreEqual(glm::vec4{ 1.2, 3.2, 4.7, 0.2 }, enemy["Location"].GetAsVector());
			Assert::AreEqual(glm::mat4{ 1.2, 3.2, 4.7, 0.2, 1.2, 3.2, 4.7, 0.2, 1.2, 3.2, 4.7, 0.2, 1.2, 3.2, 4.7, 0.2 }, enemy["PreviousLocations"].GetAsMatrix());
			Scope& nestedEnemy = *enemy["Enemy"].GetAsScope();
			Assert::AreEqual(150, nestedEnemy["Health"].GetAsInt());
			Assert::AreEqual(0.1f, nestedEnemy["Position"].GetAsFloat());
			Assert::AreEqual(0.2f, nestedEnemy["Position"].GetAsFloat(1));
			Assert::AreEqual(0.3f, nestedEnemy["Position"].GetAsFloat(2));
			Assert::AreEqual(0.4f, nestedEnemy["Position"].GetAsFloat(3));
			Assert::AreEqual("Ricky"s, nestedEnemy["Name"].GetAsString());
			Assert::AreEqual("Rick"s, nestedEnemy["Aliases"].GetAsString());
			Assert::AreEqual("Rude"s, nestedEnemy["Aliases"].GetAsString(1));
			Assert::AreEqual("Rob"s, nestedEnemy["Aliases"].GetAsString(2));
			Assert::AreEqual(glm::vec4{ 3.2, 5.2, 7.7, 3.2 }, nestedEnemy["Location"].GetAsVector());
			Assert::AreEqual(glm::mat4{ 1.2, 3.2, 4.7, 0.2, 1.2, 3.2, 4.7, 0.2, 1.2, 3.2, 4.7, 0.2, 1.2, 3.2, 4.7, 0.2 }, nestedEnemy["PreviousLocations"].GetAsMatrix());
		}

		TEST_METHOD(TableHelperObjectArrays)
		{
			const std::string filename = "TestFile.json";
			const std::string inputString =
				R"({"Attacks": [{ "Attack1" : { "Primary": "Punch", "Damage": 15 }}, { "Attack2" : { "Primary": "Kick", "Damage": 25}}]})";
			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());

			outputFile << inputString;
			outputFile.close();

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };

			Assert::IsTrue(wrapper == parseCoordinator.GetWrapper());
			Assert::IsTrue(wrapper->GetJsonParseCoordinator() == &parseCoordinator);

			auto helper = std::make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());

			auto& datum = rawWrapper->_scope->Append("Attacks");
			Assert::AreEqual(std::size_t{ 2 }, datum.Size());

			Scope* attack1 = datum.GetAsScope();
			{
				auto& firstObject = attack1->Append("Primary");

				Assert::AreEqual("Punch"s, firstObject.GetAsString());

				auto& secondObject = attack1->Append("Damage");

				Assert::AreEqual(15, secondObject.GetAsInt());
			}

			Scope* attack2 = datum.GetAsScope(1);
			{
				auto& firstObject = attack2->Append("Primary");

				Assert::AreEqual("Kick"s, firstObject.GetAsString());

				auto& secondObject = attack2->Append("Damage");

				Assert::AreEqual(25, secondObject.GetAsInt());
			}

		}

	private:
		inline static _CrtMemState _startMemState;
	};
}