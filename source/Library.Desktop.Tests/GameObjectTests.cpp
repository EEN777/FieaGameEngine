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
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(GameObjectTests)
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
			_typeManager->AddSignature(GameObject::TypeIdClass(), GameObject::Signatures());

			Factory<Scope>::Add(std::make_unique<const GameObjectFactory>());

			GameTime gameTime{};

			std::string inputString = R"({ "GameObject foo": {
												"Name" : "foo",
												"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Children" : [ { "GameObject SubObject" : { "Name" : "TestGameObject" } }]
										  } })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			GameObject& gameObject = *rawWrapper->_scope->As<GameObject>();
			Assert::AreEqual("foo"s, gameObject._name);
			Assert::AreEqual(gameObject.ToString(), gameObject._name);
			Assert::IsTrue(gameObject.Equals(&gameObject));
			Assert::AreEqual(glm::vec4{1.2f, 3.2f, 4.7f, 0.2f}, gameObject._position);
			Assert::AreEqual(glm::vec4{1.2f, 3.2f, 4.7f, 0.2f}, gameObject._rotation);
			Assert::AreEqual(glm::vec4{1.2f, 3.2f, 4.7f, 0.2f}, gameObject._scale);

			Datum& children = gameObject["Children"];
			children;
			Assert::AreNotEqual(std::size_t{ 0 }, children.Size());
			Scope* scope = children.GetAsScope();
			Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
			Assert::AreEqual("TestGameObject"s, static_cast<GameObject*>(scope)->_name);

			gameObject.Update(gameTime);

			GameObject copiedGameObject{ gameObject };

			Assert::IsTrue(!copiedGameObject.Equals(&gameObject));

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

		TEST_METHOD(Update)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());

			Factory<Scope>::Add(std::make_unique<const GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());

			GameTime gameTime{};

			std::string inputString = R"({ "GameObject foo": {
												"Name" : "foo",
												"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Children" : [ { "Monster SubObject" : { 
																	"Name" : "TestGameObject",
																	"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Health" : 100,
																	"Damage" : 20	} }]
										  } })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			GameObject& gameObject = *rawWrapper->_scope->As<GameObject>();
			Assert::AreEqual("foo"s, gameObject._name);
			Assert::AreEqual(glm::vec4{ 1.2f, 3.2f, 4.7f, 0.2f }, gameObject._position);
			Assert::AreEqual(glm::vec4{ 1.2f, 3.2f, 4.7f, 0.2f }, gameObject._rotation);
			Assert::AreEqual(glm::vec4{ 1.2f, 3.2f, 4.7f, 0.2f }, gameObject._scale);

			Datum& children = gameObject["Children"];
			Assert::IsTrue(children.Size() > 0);
			Scope* scope = children.GetAsScope();
			Assert::IsTrue(scope->Is(GameObject::TypeIdClass()));
			Assert::IsTrue(scope->Is(Monster::TypeIdClass()));

			Monster* monster = static_cast<Monster*>(scope);
			Assert::AreEqual("TestGameObject"s, monster->_name);
			Assert::AreEqual(glm::vec4{ 1.2f, 3.2f, 4.7f, 0.2f }, monster->_position);
			Assert::AreEqual(glm::vec4{ 1.2f, 3.2f, 4.7f, 0.2f }, monster->_rotation);
			Assert::AreEqual(glm::vec4{ 1.2f, 3.2f, 4.7f, 0.2f }, monster->_scale);
			Assert::AreEqual(100, monster->_health);
			Assert::AreEqual(20, monster->_damage);

			Assert::IsFalse(monster->_hasUpdated);
			gameObject.Update(gameTime);
			Assert::IsTrue(monster->_hasUpdated);

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

		TEST_METHOD(GetChildren)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());

			Factory<Scope>::Add(std::make_unique<const GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());

			GameTime gameTime{};

			std::string inputString = R"({ "GameObject foo": {
												"Name" : "foo",
												"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Children" : [ { "Monster SubObject" : { 
																	"Name" : "Monster1",
																	"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Health" : 100,
																	"Damage" : 20	} },
																{ "Monster SubObject" : { 
																	"Name" : "Monster2",
																	"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Health" : 100,
																	"Damage" : 20	} }]
										  } })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			GameObject& gameObject = *rawWrapper->_scope->As<GameObject>();

			GameObject copiedGameObject{ gameObject };

			auto& children = gameObject.GetChildren();

			for (std::size_t index{ 0 }; index < children.Size(); ++index)
			{
				Assert::IsFalse(reinterpret_cast<Monster*>(children.GetAsScope(index))->_hasUpdated);
			}

			gameObject.Update(gameTime);

			for (std::size_t index{ 0 }; index < children.Size(); ++index)
			{
				Assert::IsTrue(reinterpret_cast<Monster*>(children.GetAsScope(index))->_hasUpdated);
			}

			auto& copiedChildren = copiedGameObject.GetChildren();

			for (std::size_t index{ 0 }; index < copiedChildren.Size(); ++index)
			{
				Assert::IsFalse(reinterpret_cast<Monster*>(copiedChildren.GetAsScope(index))->_hasUpdated);
			}

			copiedGameObject.Update(gameTime);

			for (std::size_t index{ 0 }; index < copiedChildren.Size(); ++index)
			{
				Assert::IsTrue(reinterpret_cast<Monster*>(copiedChildren.GetAsScope(index))->_hasUpdated);
			}

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

		TEST_METHOD(CreateChild)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());

			Factory<Scope>::Add(std::make_unique<const GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\Content.json)"s;

			std::string inputString = R"({ "GameObject foo": {
												"Name" : "foo",
												"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
												"Children" : [ { "Monster SubObject" : { 
																	"Name" : "Monster1",
																	"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Health" : 100,
																	"Damage" : 20	} },
																{ "Monster SubObject" : { 
																	"Name" : "Monster2",
																	"Position" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Rotation" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Scale" : " vec4(1.2, 3.2, 4.7, 0.2) ",
																	"Health" : 100,
																	"Damage" : 20	} }]
										  } })";

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObject(inputString);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			GameObject& gameObject = *rawWrapper->_scope->As<GameObject>();

			Datum& children = gameObject.GetChildren();

			Assert::AreEqual(std::size_t{ 2 }, children.Size());

			gameObject.CreateChild("Monster");

			Assert::AreEqual(std::size_t{ 3 }, children.Size());

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

		TEST_METHOD(FileDeserialization)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			_typeManager->AddSignature(Monster::TypeIdClass(), Monster::Signatures());

			Factory<Scope>::Add(std::make_unique<const GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<const MonsterFactory>());

			GameTime gameTime{};

			std::string filename = R"(Content\Content.json)"s;

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator{ wrapper };
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.Initialize();

			parseCoordinator.DeserializeObjectFromFile(filename);

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			GameObject& gameObject = *rawWrapper->_scope->As<GameObject>();

			Datum& children = gameObject.GetChildren();

			Assert::AreEqual(std::size_t{ 2 }, children.Size());

			gameObject.CreateChild("Monster");

			Assert::AreEqual(std::size_t{ 3 }, children.Size());

			Factory<Scope>::Clear();
			TypeManager::DestroyInstance();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}