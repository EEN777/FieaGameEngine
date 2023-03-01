#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "Foo.h"
#include "Bar.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
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

		TEST_METHOD(CopyConstructor)
		{
			{
				Scope scope{};
				scope.AppendScope("Scope");
				scope.Find("Scope")->GetAsScope()->AppendScope("NestedScope");

				Scope copiedScope{ scope };
				Assert::AreEqual(scope.Size(), copiedScope.Size());

				Assert::IsNotNull(scope.Find("Scope"));
				Assert::IsNotNull(copiedScope.Find("Scope"));
				Assert::IsNotNull((copiedScope.Find("Scope"))->GetAsScope());
				Assert::IsNotNull((copiedScope.Find("Scope"))->GetAsScope()->Find("NestedScope"));
			}

			{
				Scope scope{};
				Scope& firstScope = scope.AppendScope("FirstScope");
				Scope& secondScope = scope.AppendScope("SecondScope");
				Scope& thirdScope = scope.AppendScope("ThirdScope");
				Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
				Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
				Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
				Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

				firstScopeInteger = 100;
				secondNestedScopeStrings.PushBack(std::string{ "Throw" });
				secondNestedScopeStrings.PushBack(std::string{ "Punch" });
				secondNestedScopeStrings.PushBack(std::string{ "Kick" });
				thirdScopeInteger = 80;

				{
					Scope copiedScope{ scope };
					Assert::AreEqual(scope.Size(), copiedScope.Size());

					Datum* datum = copiedScope.Find("FirstScope");
					Assert::IsNotNull(datum);
					Assert::IsNotNull(datum->GetAsScope()->Find("Integer"));
					Datum* innerDatum = datum->GetAsScope()->Find("Integer");
					Assert::AreEqual(100, innerDatum->GetAsInt());

					datum = copiedScope.Find("SecondScope");
					Assert::IsNotNull(datum);
					Assert::IsNotNull(datum->GetAsScope()->Find("SecondScopeNestedScope"));
					datum = datum->GetAsScope()->Find("SecondScopeNestedScope");
					innerDatum = datum->GetAsScope()->Find("Strings");
					Assert::AreEqual(std::string{ "Throw" }, innerDatum->GetAsString());
					Assert::AreEqual(std::string{ "Punch" }, innerDatum->GetAsString(1));
					Assert::AreEqual(std::string{ "Kick" }, innerDatum->GetAsString(2));

					datum = copiedScope.Find("ThirdScope");
					Assert::IsNotNull(datum);
					Assert::IsNotNull(datum->GetAsScope()->Find("Integer"));
					innerDatum = datum->GetAsScope()->Find("Integer");
					Assert::AreEqual(80, innerDatum->GetAsInt());
				}
			}

			{
				Scope scope{};
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello, World!";
				Datum& bDatum = scope.Append(b);
				bDatum = glm::mat4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(std::size_t{ 3 }, scope.Size());

				Datum* cDatum = scope.FindContainedScope(&cScope).first;
				Assert::IsNotNull(cDatum);

				Scope copiedScope = scope;
				Assert::IsFalse(copiedScope.IsEmpty());
				Assert::AreEqual(std::size_t{ 3 }, copiedScope.Size());
				Datum* foundDatum = copiedScope.Find(a);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&aDatum == foundDatum);
				Assert::IsTrue(aDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(aDatum.GetAsString(), foundDatum->GetAsString());
				Assert::AreEqual(aDatum.GetAsString(), foundDatum->GetAsString());

				foundDatum = copiedScope.Find(b);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&bDatum == foundDatum);
				Assert::IsTrue(bDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());
				Assert::AreEqual(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());

				foundDatum = copiedScope.Find(c);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(cDatum == foundDatum);
				Assert::IsTrue(cDatum->Type() == foundDatum->Type());

				Assert::IsTrue(scope[0] == copiedScope[0]);
				Assert::IsTrue(scope[1] == copiedScope[1]);
				Assert::IsTrue(scope[2] == copiedScope[2]);
			}
		}

		TEST_METHOD(CopyAssignment)
		{
			{
				Scope scope{};
				scope.AppendScope("Scope");
				scope.Find("Scope")->GetAsScope()->AppendScope("NestedScope");

				Scope copiedScope{};
				copiedScope = scope;

				Assert::AreEqual(scope.Size(), copiedScope.Size());
				Assert::IsTrue(scope == copiedScope);

				Assert::IsNotNull(scope.Find("Scope"));
				Assert::IsNotNull(copiedScope.Find("Scope"));
				Assert::IsNotNull((copiedScope.Find("Scope"))->GetAsScope());
				Assert::IsNotNull((copiedScope.Find("Scope"))->GetAsScope()->Find("NestedScope"));

				Scope otherScope{};
				Scope& firstScope = scope.AppendScope("FirstScope");
				Scope& secondScope = scope.AppendScope("SecondScope");
				Scope& thirdScope = scope.AppendScope("ThirdScope");
				Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
				Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
				Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
				Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

				firstScopeInteger = 100;
				secondNestedScopeStrings.PushBack(std::string{ "Throw" });
				secondNestedScopeStrings.PushBack(std::string{ "Punch" });
				secondNestedScopeStrings.PushBack(std::string{ "Kick" });
				thirdScopeInteger = 80;

				scope = otherScope;
				Assert::IsTrue(scope == otherScope);
			}

			{
				Scope scope{};
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello, World!";
				Datum& bDatum = scope.Append(b);
				bDatum = glm::mat4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(std::size_t{ 3 }, scope.Size());

				Datum* cDatum = scope.FindContainedScope(&cScope).first;
				Assert::IsNotNull(cDatum);

				Scope copiedScope{};
				copiedScope.Append("Integers", Datum::DatumTypes::Integer);
				copiedScope.Append("Floats", Datum::DatumTypes::Float);
				Scope& nestedScope = copiedScope.AppendScope("NestedScope");

				{
					Scope copiedNestedScope{};
					copiedNestedScope = nestedScope;
					Assert::IsTrue(copiedNestedScope == nestedScope);
				}

				Datum& nestedScopeDatum = nestedScope.Append("Strings", Datum::DatumTypes::String);
				nestedScopeDatum = "This is a string!";
				copiedScope = scope;
				Assert::AreEqual(std::size_t{ 3 }, copiedScope.Size());
				Datum* foundDatum = copiedScope.Find(a);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&aDatum == foundDatum);
				Assert::IsTrue(aDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(aDatum.GetAsString(), foundDatum->GetAsString());
				Assert::AreEqual(aDatum.GetAsString(), foundDatum->GetAsString());

				foundDatum = copiedScope.Find(b);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&bDatum == foundDatum);
				Assert::IsTrue(bDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());
				Assert::AreEqual(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());

				foundDatum = copiedScope.Find(c);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(cDatum == foundDatum);
				Assert::IsTrue(cDatum->Type() == foundDatum->Type());

				Assert::IsTrue(scope[0] == copiedScope[0]);
				Assert::IsTrue(scope[1] == copiedScope[1]);
				Assert::IsTrue(scope[2] == copiedScope[2]);
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			{
				Scope scope{};
				scope.AppendScope("Scope");
				scope.Find("Scope")->GetAsScope()->AppendScope("NestedScope");

				Scope copiedScope{ scope };

				Scope otherScope{ std::move(copiedScope) };
				Assert::IsNotNull(otherScope.Find("Scope"));
				Assert::IsNotNull((otherScope.Find("Scope"))->GetAsScope());

				Assert::IsTrue(scope == otherScope);

				Scope emancipatedScope{ std::move(*(otherScope.Find("Scope"))->GetAsScope()) };
				Assert::IsNotNull(emancipatedScope.Find("NestedScope"));

				Assert::AreEqual(std::size_t{ 1 }, emancipatedScope.Size());
				Assert::AreEqual(std::size_t{ 0 }, emancipatedScope.Find("NestedScope")->GetAsScope()->Size());
			}

			{
				Scope scope{};
				Scope& firstScope = scope.AppendScope("FirstScope");
				Scope& secondScope = scope.AppendScope("SecondScope");
				Scope& thirdScope = scope.AppendScope("ThirdScope");
				Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
				Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
				Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
				Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

				firstScopeInteger = 100;
				secondNestedScopeStrings.PushBack(std::string{ "Throw" });
				secondNestedScopeStrings.PushBack(std::string{ "Punch" });
				secondNestedScopeStrings.PushBack(std::string{ "Kick" });
				thirdScopeInteger = 80;

				Scope copiedScope{ secondScopeNestedScope };
				Scope movedScope{ std::move(secondScopeNestedScope) };
				Assert::IsTrue(copiedScope == movedScope);
			}

			{
				Scope scope{};
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello, World!";
				Datum& bDatum = scope.Append(b);
				bDatum = glm::mat4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(std::size_t{ 3 }, scope.Size());

				Datum* cDatum = scope.FindContainedScope(&cScope).first;
				Assert::IsNotNull(cDatum);

				Scope duplicateScope{ scope };
				Scope copiedScope{ std::move(duplicateScope) };

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(std::size_t{ 0 }, duplicateScope.Size());
				Assert::IsTrue(duplicateScope.IsEmpty());
#pragma warning(pop)

				Assert::AreEqual(std::size_t{ 3 }, copiedScope.Size());
				Datum* foundDatum = copiedScope.Find(a);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&aDatum == foundDatum);
				Assert::IsTrue(aDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(aDatum.GetAsString(), foundDatum->GetAsString());
				Assert::AreEqual(aDatum.GetAsString(), foundDatum->GetAsString());

				foundDatum = copiedScope.Find(b);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&bDatum == foundDatum);
				Assert::IsTrue(bDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());
				Assert::AreEqual(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());

				foundDatum = copiedScope.Find(c);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(cDatum == foundDatum);
				Assert::IsTrue(cDatum->Type() == foundDatum->Type());

				Assert::IsTrue(scope[0] == copiedScope[0]);
				Assert::IsTrue(scope[1] == copiedScope[1]);
				Assert::IsTrue(scope[2] == copiedScope[2]);
			}
		}

		TEST_METHOD(MoveAssignment)
		{
			{
				Scope scope{};
				scope.AppendScope("Scope");
				scope.Find("Scope")->GetAsScope()->AppendScope("NestedScope");

				Scope copiedScope{ scope };

				Scope otherScope{};
				otherScope = std::move(copiedScope);
				Assert::IsNotNull(otherScope.Find("Scope"));
				Assert::IsNotNull((otherScope.Find("Scope"))->GetAsScope());

				Assert::IsTrue(scope == otherScope);

				Scope emancipatedScope{};
				emancipatedScope = std::move(*(otherScope.Find("Scope"))->GetAsScope());
				Assert::IsNotNull(emancipatedScope.Find("NestedScope"));

				Assert::AreEqual(std::size_t{ 1 }, emancipatedScope.Size());
				Assert::AreEqual(std::size_t{ 0 }, emancipatedScope.Find("NestedScope")->GetAsScope()->Size());
			}

			{
				Scope scope{};
				Scope& firstScope = scope.AppendScope("FirstScope");
				Scope& secondScope = scope.AppendScope("SecondScope");
				Scope& thirdScope = scope.AppendScope("ThirdScope");
				Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
				Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
				Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
				Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

				firstScopeInteger = 100;
				secondNestedScopeStrings.PushBack(std::string{ "Throw" });
				secondNestedScopeStrings.PushBack(std::string{ "Punch" });
				secondNestedScopeStrings.PushBack(std::string{ "Kick" });
				thirdScopeInteger = 80;

				{
					Scope copiedScope{ scope };

					Scope otherScope{};

					otherScope = std::move(copiedScope);

					Datum* datum = otherScope.Find("FirstScope");
					Assert::IsNotNull(datum);
					Assert::IsNotNull(datum->GetAsScope()->Find("Integer"));
					Datum* innerDatum = datum->GetAsScope()->Find("Integer");
					Assert::AreEqual(100, innerDatum->GetAsInt());

					datum = otherScope.Find("SecondScope");
					Assert::IsNotNull(datum);
					Assert::IsNotNull(datum->GetAsScope()->Find("SecondScopeNestedScope"));
					datum = datum->GetAsScope()->Find("SecondScopeNestedScope");
					innerDatum = datum->GetAsScope()->Find("Strings");
					Assert::AreEqual(std::string{ "Throw" }, innerDatum->GetAsString());
					Assert::AreEqual(std::string{ "Punch" }, innerDatum->GetAsString(1));
					Assert::AreEqual(std::string{ "Kick" }, innerDatum->GetAsString(2));

					datum = otherScope.Find("ThirdScope");
					Assert::IsNotNull(datum);
					Assert::IsNotNull(datum->GetAsScope()->Find("Integer"));
					innerDatum = datum->GetAsScope()->Find("Integer");
					Assert::AreEqual(80, innerDatum->GetAsInt());
				}
			}

			{
				Scope scope{};
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello, World!";
				Datum& bDatum = scope.Append(b);
				bDatum = glm::mat4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(std::size_t{ 3 }, scope.Size());

				Datum* cDatum = scope.FindContainedScope(&cScope).first;
				Assert::IsNotNull(cDatum);

				Scope copiedScope{};
				copiedScope.Append("Integers", Datum::DatumTypes::Integer);
				copiedScope.Append("Floats", Datum::DatumTypes::Float);
				Scope& nestedScope = copiedScope.AppendScope("NestedScope");


				Datum& nestedScopeDatum = nestedScope.Append("Strings", Datum::DatumTypes::String);
				nestedScopeDatum = "This is a string!";

				Scope originalScope{ copiedScope };
				Scope duplicateScope{ scope };
				copiedScope = std::move(duplicateScope);
				Assert::AreEqual(std::size_t{ 3 }, copiedScope.Size());
				Datum* foundDatum = copiedScope.Find(a);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&aDatum == foundDatum);
				Assert::IsTrue(aDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(aDatum.GetAsString(), foundDatum->GetAsString());
				Assert::AreEqual(aDatum.GetAsString(), foundDatum->GetAsString());

				foundDatum = copiedScope.Find(b);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(&bDatum == foundDatum);
				Assert::IsTrue(bDatum.Type() == foundDatum->Type());
				Assert::AreNotSame(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());
				Assert::AreEqual(bDatum.GetAsMatrix(), foundDatum->GetAsMatrix());

				foundDatum = copiedScope.Find(c);
				Assert::IsNotNull(foundDatum);
				Assert::IsFalse(cDatum == foundDatum);
				Assert::IsTrue(cDatum->Type() == foundDatum->Type());

				Assert::IsTrue(scope[0] == copiedScope[0]);
				Assert::IsTrue(scope[1] == copiedScope[1]);
				Assert::IsTrue(scope[2] == copiedScope[2]);

				Datum* tempDatum = originalScope.Find("NestedScope");
				Assert::IsNotNull(tempDatum);
				Scope* nestedScopePtr = tempDatum->GetAsScope();
				Scope copiedNestedScope{ *nestedScopePtr };
				Scope movedNestedScope{};
				movedNestedScope = std::move(*nestedScopePtr);
				Assert::IsTrue(movedNestedScope == copiedNestedScope);
				
			}
		}

		TEST_METHOD(Equality)
		{
			{
				Scope scope{};
				scope.AppendScope("Scope");
				scope.Find("Scope")->GetAsScope()->AppendScope("NestedScope");

				Scope copiedScope{ scope };
				Assert::AreEqual(scope.Size(), copiedScope.Size());

				Assert::IsNotNull(scope.Find("Scope"));
				Assert::IsNotNull(copiedScope.Find("Scope"));
				Assert::IsNotNull((copiedScope.Find("Scope"))->GetAsScope());
				Assert::IsNotNull((copiedScope.Find("Scope"))->GetAsScope()->Find("NestedScope"));

				Scope emptyScope{};

				Assert::IsTrue(scope == copiedScope);
				Assert::IsTrue(scope != emptyScope);
			}

			{
				Scope scope{};
				Scope& firstScope = scope.AppendScope("FirstScope");
				Scope& secondScope = scope.AppendScope("SecondScope");
				Scope& thirdScope = scope.AppendScope("ThirdScope");
				Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
				Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
				Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
				Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

				firstScopeInteger = 100;
				secondNestedScopeStrings.PushBack(std::string{ "Throw" });
				secondNestedScopeStrings.PushBack(std::string{ "Punch" });
				secondNestedScopeStrings.PushBack(std::string{ "Kick" });
				thirdScopeInteger = 80;

				Scope copiedScope{ scope };
				Scope emptyScope{};

				Assert::IsTrue(scope == copiedScope);
				Assert::IsTrue(scope != emptyScope);
			}

			{
				Scope scope{3};
				Scope& firstScope = scope.AppendScope("FirstScope");
				Scope& secondScope = scope.AppendScope("SecondScope");
				Scope& thirdScope = scope.AppendScope("ThirdScope");
				Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
				Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
				Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
				Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

				firstScopeInteger = 100;
				secondNestedScopeStrings.PushBack(std::string{ "Throw" });
				secondNestedScopeStrings.PushBack(std::string{ "Punch" });
				secondNestedScopeStrings.PushBack(std::string{ "Kick" });
				thirdScopeInteger = 80;

				Scope otherScope{};

				Assert::IsTrue(scope != otherScope);
				Assert::IsFalse(scope == otherScope);
				Assert::IsTrue(scope == scope);
				Assert::IsFalse(scope != scope);

				Scope& otherFirstScope = otherScope.AppendScope("FirstScope");
				otherScope.AppendScope("SecondScope");
				otherScope.AppendScope("ThirdScope");
				Datum& otherFirstScopeInteger = otherFirstScope["Integer"];
				otherFirstScopeInteger = 70;

				Assert::IsFalse(otherScope == scope);
				Assert::IsTrue(otherScope != scope);
			}

			{
				Foo foo{ 7 };
				Foo* fooPtr = &foo;

				Scope scope{};
				Assert::IsFalse(scope.Equals(fooPtr));
			}
		}

		TEST_METHOD(Append)
		{
			Scope scope{};
			scope.Append(std::string{ "Health" });
			scope.Append(std::string{ "Health" });
			Assert::AreEqual(true, Datum{} == scope.Append(std::string{ "Health" }));
		}

		TEST_METHOD(Find)
		{
			Scope scope{};
			scope.Append(std::string{ "Health" });
			scope.Append(std::string{ "DPS" });
			scope.Append(std::string{ "Stamina" });
			Assert::IsNotNull(scope.Find("Health"));
			Assert::IsNotNull(scope.Find("DPS"));
			Assert::IsNotNull(scope.Find("Stamina"));
			Assert::IsNull(scope.Find("Invalid"));
		}

		TEST_METHOD(Search)
		{
			Scope scope{};
			Scope& firstScope = scope.AppendScope("FirstScope");
			Scope& secondScope = scope.AppendScope("SecondScope");
			Scope& thirdScope = scope.AppendScope("ThirdScope");
			Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
			Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
			Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
			Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

			firstScopeInteger = 100;
			secondNestedScopeStrings.PushBack(std::string{ "Throw" });
			secondNestedScopeStrings.PushBack(std::string{ "Punch" });
			secondNestedScopeStrings.PushBack(std::string{ "Kick" });
			thirdScopeInteger = 80;

			{
				Assert::IsNotNull(secondScopeNestedScope.Search("ThirdScope"));

				Scope* foundScope;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNotNull(secondScopeNestedScope.Search("ThirdScope", foundScopeRef));

				Assert::IsNotNull(foundScope->Find("ThirdScope"));
				Assert::IsTrue(&scope == foundScope);
			}

			{
				Scope copiedScope(secondScopeNestedScope);

				Assert::IsNull(copiedScope.Search("ThirdScope"));

				Scope* foundScope;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNull(copiedScope.Search("ThirdScope", foundScopeRef));
				Assert::IsNull(copiedScope.GetParent());

			}

			{
				Scope temp{ scope };
				Scope copiedScope(std::move(temp));

				Assert::IsNotNull(copiedScope.Search("ThirdScope"));

				Scope* foundScope;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNotNull(copiedScope.Search("ThirdScope", foundScopeRef));
				Assert::IsNull(copiedScope.GetParent());
			}

			{
				Scope temp{ scope };
				Scope copiedScope{};

				copiedScope = std::move(temp);

				Assert::IsNotNull(copiedScope.Search("ThirdScope"));

				Scope* foundScope;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNotNull(copiedScope.Search("ThirdScope", foundScopeRef));
				Assert::IsNull(copiedScope.GetParent());
			}

			{
				Scope temp{ secondScopeNestedScope };
				Scope copiedScope{};

				copiedScope = std::move(temp);

				Assert::IsNull(copiedScope.Search("ThirdScope"));
				Assert::IsNotNull(copiedScope.Search("Strings"));

				Scope* foundScope = nullptr;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNull(copiedScope.Search("ThirdScope", foundScopeRef));
				Assert::IsNull(foundScope);
				Assert::IsNotNull(copiedScope.Search("Strings", foundScopeRef));
				Assert::IsNotNull(foundScope);
				Assert::IsTrue(foundScope == &copiedScope);
				Assert::IsNull(copiedScope.GetParent());
				Assert::IsNull(foundScope->GetParent());
			}

			{
				Scope temp{ secondScopeNestedScope };
				Scope copiedScope{ std::move(temp) };

				Assert::IsNull(copiedScope.Search("ThirdScope"));
				Assert::IsNotNull(copiedScope.Search("Strings"));

				Scope* foundScope = nullptr;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNull(copiedScope.Search("ThirdScope", foundScopeRef));
				Assert::IsNull(foundScope);
				Assert::IsNotNull(copiedScope.Search("Strings", foundScopeRef));
				Assert::IsNotNull(foundScope);
				Assert::IsTrue(foundScope == &copiedScope);
				Assert::IsNull(copiedScope.GetParent());
				Assert::IsNull(foundScope->GetParent());
			}

			{
				Scope copiedScope{};

				copiedScope = std::move(secondScopeNestedScope);

				Assert::IsNull(copiedScope.Search("ThirdScope"));
				Assert::IsNotNull(copiedScope.Search("Strings"));

				Scope* foundScope = nullptr;

				Scope*& foundScopeRef = foundScope;

				Assert::IsNull(copiedScope.Search("ThirdScope", foundScopeRef));
				Assert::IsNull(foundScope);
				Assert::IsNotNull(copiedScope.Search("Strings", foundScopeRef));
				Assert::IsNotNull(foundScope);
				Assert::IsTrue(foundScope == &copiedScope);
				Assert::IsNull(copiedScope.GetParent());
				Assert::IsNull(foundScope->GetParent());
			}
		}

		TEST_METHOD(FindContainedScope)
		{
			Scope scope{};
			Scope& firstScope = scope.AppendScope("FirstScope");
			Scope& secondScope = scope.AppendScope("SecondScope");
			Scope& thirdScope = scope.AppendScope("ThirdScope");
			Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
			Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
			Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
			Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

			firstScopeInteger = 100;
			secondNestedScopeStrings.PushBack(std::string{ "Throw" });
			secondNestedScopeStrings.PushBack(std::string{ "Punch" });
			secondNestedScopeStrings.PushBack(std::string{ "Kick" });
			thirdScopeInteger = 80;
			{
				std::pair<Datum*, std::size_t> pair = scope.FindContainedScope(&firstScope);
				Assert::IsNotNull(pair.first);
				Assert::IsNotNull(pair.first->GetAsScope()->Find("Integer"));
				Datum* datum = pair.first->GetAsScope()->Find("Integer");
				Assert::AreEqual(100, datum->GetAsInt());

				pair = secondScope.FindContainedScope(&secondScopeNestedScope);
				Assert::IsNotNull(pair.first);
				Assert::IsNotNull(pair.first->GetAsScope()->Find("Strings"));
				datum = pair.first->GetAsScope()->Find("Strings");
				Assert::AreEqual(std::string{ "Throw" }, datum->GetAsString());
				Assert::AreEqual(std::string{ "Punch" }, datum->GetAsString(1));
				Assert::AreEqual(std::string{ "Kick" }, datum->GetAsString(2));

				pair = scope.FindContainedScope(&thirdScope);
				Assert::IsNotNull(pair.first);
				Assert::IsNotNull(pair.first->GetAsScope()->Find("Integer"));
				datum = pair.first->GetAsScope()->Find("Integer");
				Assert::AreEqual(80, datum->GetAsInt());
			}

			{
				Scope copiedScope{ scope };
				Assert::AreEqual(scope.Size(), copiedScope.Size());

				Datum* datum = copiedScope.Find("FirstScope");
				Assert::IsNotNull(datum);
				Assert::IsNotNull(datum->GetAsScope()->Find("Integer"));
				Datum* innerDatum = datum->GetAsScope()->Find("Integer");
				Assert::AreEqual(100, innerDatum->GetAsInt());

				datum = copiedScope.Find("SecondScope");
				Assert::IsNotNull(datum);
				Assert::IsNotNull(datum->GetAsScope()->Find("SecondScopeNestedScope"));
				datum = datum->GetAsScope()->Find("SecondScopeNestedScope");
				innerDatum = datum->GetAsScope()->Find("Strings");
				Assert::AreEqual(std::string{ "Throw" }, innerDatum->GetAsString());
				Assert::AreEqual(std::string{ "Punch" }, innerDatum->GetAsString(1));
				Assert::AreEqual(std::string{ "Kick" }, innerDatum->GetAsString(2));

				datum = copiedScope.Find("ThirdScope");
				Assert::IsNotNull(datum);
				Assert::IsNotNull(datum->GetAsScope()->Find("Integer"));
				innerDatum = datum->GetAsScope()->Find("Integer");
				Assert::AreEqual(80, innerDatum->GetAsInt());
			}

		}

		TEST_METHOD(AppendScope)
		{
			Scope scope{};
			scope.AppendScope("Scope");
			Assert::IsTrue(scope.Find("Scope")->GetAsScope()->GetParent() == &scope);
		}

		TEST_METHOD(Orphan)
		{
			Scope scope{};
			scope.AppendScope("Scope");
			Assert::IsTrue(scope.Find("Scope")->GetAsScope()->GetParent() == &scope);
			Scope* nestedScope = scope.Find("Scope")->GetAsScope();
			Scope* capturedScope = nestedScope->Orphan();
			delete capturedScope;
		}

		TEST_METHOD(Adopt)
		{
			Scope scope{};
			scope.AppendScope("Scope");
			Assert::IsTrue(scope.Find("Scope")->GetAsScope()->GetParent() == &scope);
			Scope* nestedScope = scope.Find("Scope")->GetAsScope();
			Scope* capturedScope = nestedScope->Orphan();
			Scope* A = new Scope{};
			Scope newParent{};
			newParent.Adopt(*A, "AdoptedScope");
			newParent.Adopt(*capturedScope, "AdoptedScope");
		}

		TEST_METHOD(Ancestry)
		{
			Scope scope{};
			Scope& firstScope = scope.AppendScope("FirstScope");
			Scope& secondScope = scope.AppendScope("SecondScope");
			Scope& thirdScope = scope.AppendScope("ThirdScope");
			Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
			Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
			Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
			Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

			firstScopeInteger = 100;
			secondNestedScopeStrings.PushBack(std::string{ "Throw" });
			secondNestedScopeStrings.PushBack(std::string{ "Punch" });
			secondNestedScopeStrings.PushBack(std::string{ "Kick" });
			thirdScopeInteger = 80;

			Scope otherScope{};

			Assert::IsTrue(firstScope.IsDescendantOf(&scope));
			Assert::IsFalse(firstScope.IsDescendantOf(&thirdScope));
			Assert::IsTrue(scope.IsAncestorOf(&firstScope));
			Assert::IsFalse(scope.IsAncestorOf(&otherScope));
		}

		TEST_METHOD(ToString)
		{
			const std::string nonNested{ "Scope" };
			const std::string nested{ "NestedScope" };

			Scope scope{ 3 };
			Scope& firstScope = scope.AppendScope("FirstScope");
			Scope& secondScope = scope.AppendScope("SecondScope");
			Scope& thirdScope = scope.AppendScope("ThirdScope");
			Datum& firstScopeInteger = firstScope.Append("Integer", Datum::DatumTypes::Integer);
			Scope& secondScopeNestedScope = secondScope.AppendScope("SecondScopeNestedScope");
			Datum& thirdScopeInteger = thirdScope.Append("Integer", Datum::DatumTypes::Integer);
			Datum& secondNestedScopeStrings = secondScopeNestedScope.Append("Strings", Datum::DatumTypes::String);

			firstScopeInteger = 100;
			secondNestedScopeStrings.PushBack(std::string{ "Throw" });
			secondNestedScopeStrings.PushBack(std::string{ "Punch" });
			secondNestedScopeStrings.PushBack(std::string{ "Kick" });
			thirdScopeInteger = 80;

			Assert::AreEqual(scope.ToString(), nonNested);
			Assert::AreEqual(firstScope.ToString(), nested);
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}