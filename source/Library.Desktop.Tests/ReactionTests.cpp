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
#include "ReactionAttributed.h"
#include "GameState.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(ReactionTests)
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
			_typeManager->AddSignature(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
			_typeManager->AddSignature(ActionEvent::TypeIdClass(), ActionEvent::Signatures());
			_typeManager->AddSignature(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());

			Factory<Scope>::Add(std::make_unique<const ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionListSwitchFactory>());
			Factory<Scope>::Add(std::make_unique<const ReactionAttributedFactory>());
			Factory<Scope>::Add(std::make_unique<const ActionEventFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\ReactionTest.json)"s;

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Monster& monster = *rawWrapper->_scope->As<Monster>();
			monster;

			Assert::AreEqual(100, monster._health);
			monster.Update(gameTime);
			GameState::Update(gameTime);

			Assert::AreEqual(100, monster._health);
			gameTime.SetCurrentTime(gameTime.CurrentTime() + std::chrono::milliseconds{ 600 });
			GameState::Update(gameTime);

			Assert::AreEqual(120, monster._health);

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}