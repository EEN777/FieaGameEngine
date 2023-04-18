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
#include "GameObject.h"
#include "Monster.h"
#include "ActionList.h"
#include "ActionListSwitch.h"
#include "ActionIncrement.h"
#include "ExpressionParser.h"
#include "ActionExpression.h"
#include <any>
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(ActionTests)
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
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			_typeManager->AddSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());
			_typeManager->AddSignature(ActionListSwitch::TypeIdClass(), ActionListSwitch::Signatures());

			Factory<Scope>::Add(std::make_unique<const ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionListSwitchFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\ActionTest.json)"s;

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Monster& monster = *rawWrapper->_scope->As<Monster>();
			Datum* health = monster.Search("Health");
			Assert::IsNotNull(health);
			Datum* actions = monster.Find("Actions"s);
			Assert::IsNotNull(actions);
			Scope* actionList = actions->GetAsScope();
			Assert::IsNotNull(actionList);
			actions = actionList->Find("Actions");
			Assert::IsNotNull(actions);
			Scope* actionIncrement = actions->GetAsScope();
			Assert::IsNotNull(actionIncrement);

			Action* action = static_cast<Action*>(actionIncrement);
			Assert::AreEqual(action->Name(), "Increment"s);
			action->SetName("NewName"s);
			Assert::AreEqual(action->Name(), "NewName"s);

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

		TEST_METHOD(ActionListSwitch)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			_typeManager->AddSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());
			_typeManager->AddSignature(ActionListSwitch::TypeIdClass(), ActionListSwitch::Signatures());

			Factory<Scope>::Add(std::make_unique<const ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionListSwitchFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\ActionTest.json)"s;

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Monster& monster = *rawWrapper->_scope->As<Monster>();

			Assert::AreEqual(100, monster._health);

			monster.Update(gameTime);

			Assert::AreEqual(135, monster._health); 

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

		TEST_METHOD(ExpressionParsing)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			_typeManager->AddSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());
			_typeManager->AddSignature(ActionListSwitch::TypeIdClass(), ActionListSwitch::Signatures());

			Factory<Scope>::Add(std::make_unique<const ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionListSwitchFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\ActionTest.json)"s;

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Monster& monster = *rawWrapper->_scope->As<Monster>();
			Datum* health = monster.Search("Health");
			Assert::IsNotNull(health);
			Datum* actions = monster.Find("Actions"s);
			Assert::IsNotNull(actions);
			Scope* actionList = actions->GetAsScope();
			Assert::IsNotNull(actionList);
			actions = actionList->Find("Actions");
			Assert::IsNotNull(actions);
			Scope* actionIncrement = actions->GetAsScope();
			Assert::IsNotNull(actionIncrement);

			ExpressionParser expressionParser;

			expressionParser.ParseExpression("4 + 4 * 2 / ( 1 - 5 )", static_cast<Action*>(actionIncrement)); //4 + 4 * 2 / ( 1 - 5 )
			{
				auto output = expressionParser.Output();
				Assert::AreEqual("4"s, output[0]._token);
				Assert::IsTrue(Datum::DatumTypes::Integer == output[0]._type);
				Assert::AreEqual(4, output[0].GetAsInt(*static_cast<Action*>(actionIncrement)));
				Assert::AreEqual("4"s, output[1]._token);
				Assert::AreEqual(4, output[1].GetAsInt(*static_cast<Action*>(actionIncrement)));
				Assert::IsTrue(Datum::DatumTypes::Integer == output[1]._type);
				Assert::AreEqual("2"s, output[2]._token);
				Assert::AreEqual(2, output[2].GetAsInt(*static_cast<Action*>(actionIncrement)));
				Assert::IsTrue(Datum::DatumTypes::Integer == output[2]._type);
				Assert::AreEqual("*"s, output[3]._token);
				Assert::AreEqual("1"s, output[4]._token);
				Assert::AreEqual(1, output[4].GetAsInt(*static_cast<Action*>(actionIncrement)));
				Assert::IsTrue(Datum::DatumTypes::Integer == output[4]._type);
				Assert::AreEqual("5"s, output[5]._token);
				Assert::AreEqual(5, output[5].GetAsInt(*static_cast<Action*>(actionIncrement)));
				Assert::IsTrue(Datum::DatumTypes::Integer == output[5]._type);
				Assert::AreEqual("-"s, output[6]._token);
				Assert::AreEqual("/"s, output[7]._token);
				Assert::AreEqual("+"s, output[8]._token);
			}

			expressionParser.Reset();

			expressionParser.ParseExpression("A_s = ( 3 + 5 - ( 2 + 3 ) )", static_cast<Action*>(actionIncrement));
			{
				auto output = expressionParser.Output();
				std::string rpn{};
				for (auto& token : output)
				{
					rpn += token._token;
				}
				Assert::AreEqual("A35+23+-="s, rpn);

			}

			expressionParser.Reset();

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();

			expressionParser.ParseExpression("A_s = ( 3.2_f * 5.7_f - ( 2.2_f / 3.0_f ) )", static_cast<Action*>(actionIncrement));
			{
				auto output = expressionParser.Output();
				std::string rpn{};
				for (auto& token : output)
				{
					rpn += token._token;
				}
				Assert::AreEqual("A3.25.7*2.23.0/-="s, rpn);

			}

			expressionParser.Reset();

			expressionParser.ParseExpression("4 + 4 * 2 / ! ( 1 > 5 )", static_cast<Action*>(actionIncrement));
			{
				auto output = expressionParser.Output();
				std::string rpn{};
				for (auto& token : output)
				{
					rpn += token._token;
				}
				Assert::AreEqual("442*15>!/+"s, rpn);
			}
		}

		TEST_METHOD(ActionExpressions)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			_typeManager->AddSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());
			_typeManager->AddSignature(ActionListSwitch::TypeIdClass(), ActionListSwitch::Signatures());
			_typeManager->AddSignature(ActionExpression::TypeIdClass(), ActionExpression::Signatures());

			Factory<Scope>::Add(std::make_unique<const ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionListSwitchFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionExpressionFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\ActionExpressionTest.json)"s;

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Monster& monster = *rawWrapper->_scope->As<Monster>();

			monster.Update(gameTime);


			Assert::AreEqual(2, monster._health);

			Assert::AreEqual("CatDog"s, monster._species);

			auto action = monster.CreateAction("ActionExpression");
			auto actionExpression = static_cast<ActionExpression*>(action);
			Datum& _expression = actionExpression->Append("Expression");
			_expression = "Health = ( Health * 100 )";
			actionExpression->Reparse();

			monster.Update(gameTime);

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();

			Assert::AreEqual(200, monster._health);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}