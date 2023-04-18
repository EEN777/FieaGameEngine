#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include <execution>
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
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"
#include "Event.h"
#include "EventQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(EventTests)
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

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();

			Event<int> event{ 100 };
			Assert::AreEqual(100, event.Message());

			std::function<void(int)> func1 = std::bind(&Monster::IncreaseHealth, &monster, std::placeholders::_1);
			Event<int>::Subscriber subscriber1{ func1 };

			Event<int>::Subscribe(subscriber1);
			event.Deliver();

			Assert::AreEqual(200, monster._health);

			Event<int>::Unsubscribe(subscriber1);
			event.Deliver();

			Assert::AreEqual(200, monster._health);

			Event<int>::Subscribe(subscriber1);
			event.Deliver();

			Assert::AreEqual(300, monster._health);

			Event<int>::UnsubscribeAll();
			event.Deliver();
		}

		TEST_METHOD(EventQueues)
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

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();

			Event<int> event{ 100 };

			std::function<void(int)> func1 = std::bind(&Monster::IncreaseHealth, &monster, std::placeholders::_1);
			Event<int>::Subscriber subscriber1{ func1 };

			Event<int>::Subscribe(subscriber1);

			std::chrono::milliseconds delay{ 600 };

			EventQueue eventQueue{};

			Assert::AreEqual(100, monster._health);
			eventQueue.Enqueue(std::make_shared<Event<int>>( 100 ), gameTime, delay);
			Assert::AreEqual(std::size_t{ 1 }, eventQueue.Size());
			Assert::IsFalse(eventQueue.IsEmpty());
			gameTime.SetCurrentTime(gameTime.CurrentTime() + delay);
			eventQueue.Update(gameTime);
			Assert::IsTrue(eventQueue.IsEmpty());
			Assert::AreEqual(200, monster._health);

			eventQueue.Enqueue(std::make_shared<Event<int>>(100), gameTime, delay);
			eventQueue.Clear();
			eventQueue.Update(gameTime);
			Assert::AreEqual(200, monster._health);

			std::function<void(bool)> func2 = [&eventQueue, &gameTime](bool addEvent) { if (addEvent == true) { eventQueue.Enqueue(std::make_shared<Event<int>>(100), gameTime, std::chrono::milliseconds{ 0 }); }  };

			Event<bool>::Subscriber subscriber2{ func2 };
			Event<bool>::Subscribe(subscriber2);

			eventQueue.Enqueue(std::make_shared<Event<bool>>(true), gameTime, std::chrono::milliseconds{ 0 });
			eventQueue.Update(gameTime);

			Assert::AreEqual(200, monster._health);

			eventQueue.Update(gameTime);

			Assert::AreEqual(300, monster._health);

			Event<int>::UnsubscribeAll();
			Event<bool>::UnsubscribeAll();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
