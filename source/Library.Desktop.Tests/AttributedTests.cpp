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
using namespace std::string_literals;

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

			Assert::AreSame(foo, *reinterpret_cast<AttributedFoo*>(foo.Find("this")->GetAsRTTI()));

			foo.Find("ExternalInteger")->Set(7);
			Assert::AreEqual(7, foo.externalInteger);
			foo.Find("ExternalFloat")->Set(2.7f); 
			Assert::AreEqual(2.7f, foo.externalFloat);
			foo.Find("ExternalString")->Set("String");
			Assert::AreEqual("String"s, foo.externalString);
			foo.Find("ExternalVector")->Set(glm::vec4{});
			Assert::AreEqual(glm::vec4{}, foo.externalVector);
			foo.Find("ExternalMatrix")->Set(glm::mat4{});
			Assert::AreEqual(glm::mat4{}, foo.externalMatrix);
			foo.Find("ExternalIntegerArray")->Set(0);
			foo.Find("ExternalIntegerArray")->Set(1,1);
			foo.Find("ExternalIntegerArray")->Set(2,2);
			foo.Find("ExternalIntegerArray")->Set(3,3);
			foo.Find("ExternalIntegerArray")->Set(4,4);
			Assert::AreEqual(0, foo.externalIntegerArray[0]);
			Assert::AreEqual(1, foo.externalIntegerArray[1]);
			Assert::AreEqual(2, foo.externalIntegerArray[2]);
			Assert::AreEqual(3, foo.externalIntegerArray[3]);
			Assert::AreEqual(4, foo.externalIntegerArray[4]);
			foo.Find("ExternalFloatArray")->Set(0.0f);
			foo.Find("ExternalFloatArray")->Set(0.1f,1);
			foo.Find("ExternalFloatArray")->Set(0.2f,2);
			foo.Find("ExternalFloatArray")->Set(0.3f,3);
			foo.Find("ExternalFloatArray")->Set(0.4f,4);
			Assert::AreEqual(0.0f, foo.externalFloatArray[0]);
			Assert::AreEqual(0.1f, foo.externalFloatArray[1]);
			Assert::AreEqual(0.2f, foo.externalFloatArray[2]);
			Assert::AreEqual(0.3f, foo.externalFloatArray[3]);
			Assert::AreEqual(0.4f, foo.externalFloatArray[4]);
			foo.Find("ExternalStringArray")->Set("String0"s);
			foo.Find("ExternalStringArray")->Set("String1"s,1);
			foo.Find("ExternalStringArray")->Set("String2"s,2);
			foo.Find("ExternalStringArray")->Set("String3"s,3);
			foo.Find("ExternalStringArray")->Set("String4"s,4);
			Assert::AreEqual("String0"s, foo.externalStringArray[0]);
			Assert::AreEqual("String1"s, foo.externalStringArray[1]);
			Assert::AreEqual("String2"s, foo.externalStringArray[2]);
			Assert::AreEqual("String3"s, foo.externalStringArray[3]);
			Assert::AreEqual("String4"s, foo.externalStringArray[4]);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 });
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 },1);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 },2);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 },3);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 },4);
			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, foo.externalVectorArray[0]);
			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, foo.externalVectorArray[1]);
			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, foo.externalVectorArray[2]);
			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, foo.externalVectorArray[3]);
			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, foo.externalVectorArray[4]);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 },1);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 },2);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 },3);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 },4);
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, foo.externalMatrixArray[0]);
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, foo.externalMatrixArray[1]);
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, foo.externalMatrixArray[2]);
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, foo.externalMatrixArray[3]);
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, foo.externalMatrixArray[4]);
			Assert::IsNotNull(foo.Find("NestedScope"));
			Assert::IsNotNull(foo.Find("NestedScopeArray"));

			Scope* nestedScope = new Scope{};

			Scope* nestedScope1 = new Scope{};
			Scope* nestedScope2 = new Scope{};
			Scope* nestedScope3 = new Scope{};
			Scope* nestedScope4 = new Scope{};
			Scope* nestedScope5 = new Scope{};

			foo.Adopt(*nestedScope, "NestedScope");

			foo.Adopt(*nestedScope1, "NestedScopeArray");
			foo.Adopt(*nestedScope2, "NestedScopeArray");
			foo.Adopt(*nestedScope3, "NestedScopeArray");
			foo.Adopt(*nestedScope4, "NestedScopeArray");
			foo.Adopt(*nestedScope5, "NestedScopeArray");

			TypeManager::DestroyInstance();
		}

		TEST_METHOD(CopyConstructor)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;

			Assert::AreSame(foo, *reinterpret_cast<AttributedFoo*>(foo.Find("this")->GetAsRTTI()));

			foo.Find("ExternalInteger")->Set(7);
			foo.Find("ExternalFloat")->Set(2.7f);
			foo.Find("ExternalString")->Set("String");
			foo.Find("ExternalVector")->Set(glm::vec4{});
			foo.Find("ExternalMatrix")->Set(glm::mat4{});
			foo.Find("ExternalIntegerArray")->Set(0);
			foo.Find("ExternalIntegerArray")->Set(1, 1);
			foo.Find("ExternalIntegerArray")->Set(2, 2);
			foo.Find("ExternalIntegerArray")->Set(3, 3);
			foo.Find("ExternalIntegerArray")->Set(4, 4);
			foo.Find("ExternalFloatArray")->Set(0.0f);
			foo.Find("ExternalFloatArray")->Set(0.1f, 1);
			foo.Find("ExternalFloatArray")->Set(0.2f, 2);
			foo.Find("ExternalFloatArray")->Set(0.3f, 3);
			foo.Find("ExternalFloatArray")->Set(0.4f, 4);
			foo.Find("ExternalStringArray")->Set("String0"s);
			foo.Find("ExternalStringArray")->Set("String1"s, 1);
			foo.Find("ExternalStringArray")->Set("String2"s, 2);
			foo.Find("ExternalStringArray")->Set("String3"s, 3);
			foo.Find("ExternalStringArray")->Set("String4"s, 4);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 });
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 1);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 2);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 3);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 4);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 1);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 2);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 3);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 4);

			Scope* nestedScope = new Scope{};

			Scope* nestedAttributedFoo = new AttributedFoo{ foo };

			nestedScope->Adopt(*nestedAttributedFoo, "Foo");
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(nestedScope->Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope duplicateScope{ *nestedScope };
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(duplicateScope.Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope* nestedScope1 = new Scope{};
			Scope* nestedScope2 = new Scope{};
			Scope* nestedScope3 = new Scope{};
			Scope* nestedScope4 = new Scope{};
			Scope* nestedScope5 = new Scope{};

			foo.Adopt(*nestedScope, "NestedScope");

			foo.Adopt(*nestedScope1, "NestedScopeArray");
			foo.Adopt(*nestedScope2, "NestedScopeArray");
			foo.Adopt(*nestedScope3, "NestedScopeArray");
			foo.Adopt(*nestedScope4, "NestedScopeArray");
			foo.Adopt(*nestedScope5, "NestedScopeArray");

			AttributedFoo copiedFoo{ foo };
			Assert::AreSame(*reinterpret_cast<AttributedFoo*>(copiedFoo.Find("this"s)->GetAsRTTI()), copiedFoo);
			Assert::IsNotNull(copiedFoo.Find("ExternalInteger"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloat"));
			Assert::IsNotNull(copiedFoo.Find("ExternalString"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVector"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrix"));
			Assert::IsNotNull(copiedFoo.Find("ExternalIntegerArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloatArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalStringArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVectorArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrixArray"));
			Assert::IsNotNull(copiedFoo.Find("NestedScope"));
			Assert::IsNotNull(copiedFoo.Find("NestedScopeArray"));

			Assert::AreEqual(foo.externalInteger, copiedFoo.externalInteger);

			copiedFoo.Find("ExternalInteger")->Set(0);

			Assert::AreNotEqual(foo.externalInteger, copiedFoo.externalInteger);

			Assert::AreEqual(foo.Size(), copiedFoo.Size());

			TypeManager::DestroyInstance();
		}

		TEST_METHOD(CopyAssignment)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;

			Assert::AreSame(foo, *reinterpret_cast<AttributedFoo*>(foo.Find("this")->GetAsRTTI()));

			foo.Find("ExternalInteger")->Set(7);
			foo.Find("ExternalFloat")->Set(2.7f);
			foo.Find("ExternalString")->Set("String");
			foo.Find("ExternalVector")->Set(glm::vec4{});
			foo.Find("ExternalMatrix")->Set(glm::mat4{});
			foo.Find("ExternalIntegerArray")->Set(0);
			foo.Find("ExternalIntegerArray")->Set(1, 1);
			foo.Find("ExternalIntegerArray")->Set(2, 2);
			foo.Find("ExternalIntegerArray")->Set(3, 3);
			foo.Find("ExternalIntegerArray")->Set(4, 4);
			foo.Find("ExternalFloatArray")->Set(0.0f);
			foo.Find("ExternalFloatArray")->Set(0.1f, 1);
			foo.Find("ExternalFloatArray")->Set(0.2f, 2);
			foo.Find("ExternalFloatArray")->Set(0.3f, 3);
			foo.Find("ExternalFloatArray")->Set(0.4f, 4);
			foo.Find("ExternalStringArray")->Set("String0"s);
			foo.Find("ExternalStringArray")->Set("String1"s, 1);
			foo.Find("ExternalStringArray")->Set("String2"s, 2);
			foo.Find("ExternalStringArray")->Set("String3"s, 3);
			foo.Find("ExternalStringArray")->Set("String4"s, 4);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 });
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 1);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 2);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 3);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 4);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 1);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 2);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 3);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 4);

			Scope* nestedScope = new Scope{};

			Scope* nestedAttributedFoo = new AttributedFoo{foo};

			nestedScope->Adopt(*nestedAttributedFoo, "Foo");
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(nestedScope->Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope duplicateScope{ *nestedScope };
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(duplicateScope.Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope* nestedScope1 = new Scope{};
			Scope* nestedScope2 = new Scope{};
			Scope* nestedScope3 = new Scope{};
			Scope* nestedScope4 = new Scope{};
			Scope* nestedScope5 = new Scope{};

			foo.Adopt(*nestedScope, "NestedScope");

			foo.Adopt(*nestedScope1, "NestedScopeArray");
			foo.Adopt(*nestedScope2, "NestedScopeArray");
			foo.Adopt(*nestedScope3, "NestedScopeArray");
			foo.Adopt(*nestedScope4, "NestedScopeArray");
			foo.Adopt(*nestedScope5, "NestedScopeArray");

			AttributedFoo copiedFoo{};
			copiedFoo = foo;
			Assert::IsTrue(copiedFoo.Find("this"s)->GetAsRTTI() == &copiedFoo);
			Assert::IsNotNull(copiedFoo.Find("ExternalInteger"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloat"));
			Assert::IsNotNull(copiedFoo.Find("ExternalString"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVector"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrix"));
			Assert::IsNotNull(copiedFoo.Find("ExternalIntegerArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloatArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalStringArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVectorArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrixArray"));
			Assert::IsNotNull(copiedFoo.Find("NestedScope"));
			Assert::IsNotNull(copiedFoo.Find("NestedScopeArray"));

			Assert::IsTrue(foo.externalInteger == copiedFoo.externalInteger);

			copiedFoo.Find("ExternalInteger")->Set(0);

			Assert::IsFalse(foo.externalInteger == copiedFoo.externalInteger);

			Assert::AreEqual(foo.Size(), copiedFoo.Size());

			TypeManager::DestroyInstance();
		}

		TEST_METHOD(MoveConstructor)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;

			Assert::AreSame(foo, *reinterpret_cast<AttributedFoo*>(foo.Find("this")->GetAsRTTI()));

			foo.Find("ExternalInteger")->Set(7);
			foo.Find("ExternalFloat")->Set(2.7f);
			foo.Find("ExternalString")->Set("String");
			foo.Find("ExternalVector")->Set(glm::vec4{});
			foo.Find("ExternalMatrix")->Set(glm::mat4{});
			foo.Find("ExternalIntegerArray")->Set(0);
			foo.Find("ExternalIntegerArray")->Set(1, 1);
			foo.Find("ExternalIntegerArray")->Set(2, 2);
			foo.Find("ExternalIntegerArray")->Set(3, 3);
			foo.Find("ExternalIntegerArray")->Set(4, 4);
			foo.Find("ExternalFloatArray")->Set(0.0f);
			foo.Find("ExternalFloatArray")->Set(0.1f, 1);
			foo.Find("ExternalFloatArray")->Set(0.2f, 2);
			foo.Find("ExternalFloatArray")->Set(0.3f, 3);
			foo.Find("ExternalFloatArray")->Set(0.4f, 4);
			foo.Find("ExternalStringArray")->Set("String0"s);
			foo.Find("ExternalStringArray")->Set("String1"s, 1);
			foo.Find("ExternalStringArray")->Set("String2"s, 2);
			foo.Find("ExternalStringArray")->Set("String3"s, 3);
			foo.Find("ExternalStringArray")->Set("String4"s, 4);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 });
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 1);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 2);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 3);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 4);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 1);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 2);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 3);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 4);

			Scope* nestedScope = new Scope{};

			Scope* nestedAttributedFoo = new AttributedFoo{ foo };

			nestedScope->Adopt(*nestedAttributedFoo, "Foo");
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(nestedScope->Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope duplicateScope{ *nestedScope };
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(duplicateScope.Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope* nestedScope1 = new Scope{};
			Scope* nestedScope2 = new Scope{};
			Scope* nestedScope3 = new Scope{};
			Scope* nestedScope4 = new Scope{};
			Scope* nestedScope5 = new Scope{};

			foo.Adopt(*nestedScope, "NestedScope");

			foo.Adopt(*nestedScope1, "NestedScopeArray");
			foo.Adopt(*nestedScope2, "NestedScopeArray");
			foo.Adopt(*nestedScope3, "NestedScopeArray");
			foo.Adopt(*nestedScope4, "NestedScopeArray");
			foo.Adopt(*nestedScope5, "NestedScopeArray");

			AttributedFoo copiedFoo{std::move(foo)};
			Assert::IsTrue(copiedFoo.Find("this"s)->GetAsRTTI() == &copiedFoo);
			Assert::IsNotNull(copiedFoo.Find("ExternalInteger"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloat"));
			Assert::IsNotNull(copiedFoo.Find("ExternalString"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVector"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrix"));
			Assert::IsNotNull(copiedFoo.Find("ExternalIntegerArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloatArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalStringArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVectorArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrixArray"));
			Assert::IsNotNull(copiedFoo.Find("NestedScope"));
			Assert::IsNotNull(copiedFoo.Find("NestedScopeArray"));

			Assert::IsTrue(7 == copiedFoo.externalInteger);

			copiedFoo.Find("ExternalInteger")->Set(0);

			Assert::IsTrue(0 == copiedFoo.externalInteger);

			TypeManager::DestroyInstance();
		}

		TEST_METHOD(MoveAssignment)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;

			Assert::AreSame(foo, *reinterpret_cast<AttributedFoo*>(foo.Find("this")->GetAsRTTI()));

			foo.Find("ExternalInteger")->Set(7);
			foo.Find("ExternalFloat")->Set(2.7f);
			foo.Find("ExternalString")->Set("String");
			foo.Find("ExternalVector")->Set(glm::vec4{});
			foo.Find("ExternalMatrix")->Set(glm::mat4{});
			foo.Find("ExternalIntegerArray")->Set(0);
			foo.Find("ExternalIntegerArray")->Set(1, 1);
			foo.Find("ExternalIntegerArray")->Set(2, 2);
			foo.Find("ExternalIntegerArray")->Set(3, 3);
			foo.Find("ExternalIntegerArray")->Set(4, 4);
			foo.Find("ExternalFloatArray")->Set(0.0f);
			foo.Find("ExternalFloatArray")->Set(0.1f, 1);
			foo.Find("ExternalFloatArray")->Set(0.2f, 2);
			foo.Find("ExternalFloatArray")->Set(0.3f, 3);
			foo.Find("ExternalFloatArray")->Set(0.4f, 4);
			foo.Find("ExternalStringArray")->Set("String0"s);
			foo.Find("ExternalStringArray")->Set("String1"s, 1);
			foo.Find("ExternalStringArray")->Set("String2"s, 2);
			foo.Find("ExternalStringArray")->Set("String3"s, 3);
			foo.Find("ExternalStringArray")->Set("String4"s, 4);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 });
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 1);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 2);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 3);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 4);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 1);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 2);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 3);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 4);

			Scope* nestedScope = new Scope{};

			Scope* nestedAttributedFoo = new AttributedFoo{ foo };

			nestedScope->Adopt(*nestedAttributedFoo, "Foo");
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(nestedScope->Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope duplicateScope{ *nestedScope };
			Assert::IsTrue(reinterpret_cast<AttributedFoo*>(duplicateScope.Find("Foo")->GetAsScope())->externalInteger == 7);

			Scope* nestedScope1 = new Scope{};
			Scope* nestedScope2 = new Scope{};
			Scope* nestedScope3 = new Scope{};
			Scope* nestedScope4 = new Scope{};
			Scope* nestedScope5 = new Scope{};

			foo.Adopt(*nestedScope, "NestedScope");

			foo.Adopt(*nestedScope1, "NestedScopeArray");
			foo.Adopt(*nestedScope2, "NestedScopeArray");
			foo.Adopt(*nestedScope3, "NestedScopeArray");
			foo.Adopt(*nestedScope4, "NestedScopeArray");
			foo.Adopt(*nestedScope5, "NestedScopeArray");

			AttributedFoo copiedFoo{};
			copiedFoo = std::move(foo);
			Assert::AreSame(*reinterpret_cast<AttributedFoo*>(copiedFoo.Find("this"s)->GetAsRTTI()), copiedFoo);
			Assert::IsNotNull(copiedFoo.Find("ExternalInteger"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloat"));
			Assert::IsNotNull(copiedFoo.Find("ExternalString"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVector"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrix"));
			Assert::IsNotNull(copiedFoo.Find("ExternalIntegerArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalFloatArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalStringArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalVectorArray"));
			Assert::IsNotNull(copiedFoo.Find("ExternalMatrixArray"));
			Assert::IsNotNull(copiedFoo.Find("NestedScope"));
			Assert::IsNotNull(copiedFoo.Find("NestedScopeArray"));

			Assert::IsTrue(7 == copiedFoo.externalInteger);

			copiedFoo.Find("ExternalInteger")->Set(0);

			Assert::IsTrue(0 == copiedFoo.externalInteger);

			TypeManager::DestroyInstance();
		}

		TEST_METHOD(IsAttribute)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;

			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(foo.IsAuxillaryAttribute("AuxillaryInteger"));
			foo.AppendAuxillaryAttribute("AuxillaryInteger", Datum::DatumTypes::Integer);
			Assert::IsTrue(foo.IsAuxillaryAttribute("AuxillaryInteger"));

			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foo.IsAttribute("AuxillaryInteger"));

			TypeManager::DestroyInstance();
		}

		TEST_METHOD(Attributes)
		{
			TypeManager::CreateInstance();
			auto _typeManager = TypeManager::Instance();
			_typeManager->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo foo;

			foo.Find("ExternalInteger")->Set(7);
			foo.Find("ExternalFloat")->Set(2.7f);
			foo.Find("ExternalString")->Set("String");
			foo.Find("ExternalVector")->Set(glm::vec4{});
			foo.Find("ExternalMatrix")->Set(glm::mat4{});
			foo.Find("ExternalIntegerArray")->Set(0);
			foo.Find("ExternalIntegerArray")->Set(1, 1);
			foo.Find("ExternalIntegerArray")->Set(2, 2);
			foo.Find("ExternalIntegerArray")->Set(3, 3);
			foo.Find("ExternalIntegerArray")->Set(4, 4);
			foo.Find("ExternalFloatArray")->Set(0.0f);
			foo.Find("ExternalFloatArray")->Set(0.1f, 1);
			foo.Find("ExternalFloatArray")->Set(0.2f, 2);
			foo.Find("ExternalFloatArray")->Set(0.3f, 3);
			foo.Find("ExternalFloatArray")->Set(0.4f, 4);
			foo.Find("ExternalStringArray")->Set("String0"s);
			foo.Find("ExternalStringArray")->Set("String1"s, 1);
			foo.Find("ExternalStringArray")->Set("String2"s, 2);
			foo.Find("ExternalStringArray")->Set("String3"s, 3);
			foo.Find("ExternalStringArray")->Set("String4"s, 4);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 });
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 1);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 2);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 3);
			foo.Find("ExternalVectorArray")->Set(glm::vec4{ 1,2,3,4 }, 4);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 1);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 2);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 3);
			foo.Find("ExternalMatrixArray")->Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 4);
			foo.AppendAuxillaryAttribute("AuxillaryInteger", Datum::DatumTypes::Integer);
			foo.Find("AuxillaryInteger")->PushBack(10);

			auto indices = foo.Attributes();
			Assert::AreEqual(std::size_t{ 1 }, indices.first);
			Assert::AreEqual(std::size_t{ 14 }, indices.second);
			Assert::AreEqual(7, foo[indices.first].GetAsInt());
			Assert::AreEqual(2.7f, foo[indices.first + 1].GetAsFloat());
			Assert::AreEqual(10, foo[indices.second - 1].GetAsInt());


			indices = foo.PrescribedAttributes();
			Assert::AreEqual(std::size_t{ 1 }, indices.first);
			Assert::AreEqual(std::size_t{ 13 }, indices.second);
			Assert::AreEqual(7, foo[indices.first].GetAsInt());
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, foo[indices.second - 3].GetAsMatrix());
			Assert::IsTrue(Datum::DatumTypes::Table ==  foo[indices.second - 1].Type());

			indices = foo.AuxillaryAttributes();
			Assert::AreEqual(std::size_t{ 13 }, indices.first);
			Assert::AreEqual(std::size_t{ 14 }, indices.second);
			Assert::AreEqual(10, foo[indices.first].GetAsInt());
			Assert::AreEqual(10, foo[indices.second - 1 ].GetAsInt());

			foo.Clear();

			indices = foo.Attributes();
			Assert::AreEqual(std::size_t{ 1 }, indices.first);
			Assert::AreEqual(std::size_t{ 13 }, indices.second);

			indices = foo.PrescribedAttributes();
			Assert::AreEqual(std::size_t{ 1 }, indices.first);
			Assert::AreEqual(std::size_t{ 13 }, indices.second);

			indices = foo.AuxillaryAttributes();
			Assert::AreEqual(std::size_t{ 13 }, indices.first);
			Assert::AreEqual(std::size_t{ 13 }, indices.second);

			TypeManager::DestroyInstance();                     
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}