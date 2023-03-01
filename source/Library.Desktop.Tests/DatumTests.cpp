#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "Foo.h"
#include "Bar.h"
#include "Datum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace LibraryDesktopTests
{
	TEST_CLASS(DatumTests)
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
			{
				Datum datum;
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::Unknown };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::Integer };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Integer == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Float == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::String == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Vector == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Matrix == datum.Type());
			}

			{
				Datum datum{ Datum::DatumTypes::Pointer };
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
				Assert::AreEqual(true, Datum::DatumTypes::Pointer == datum.Type());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				{
					Datum datum{ Datum::DatumTypes::Integer };
					datum.PushBack(1);
					datum.PushBack(2);
					datum.PushBack(3);

					Datum movedDatum{ std::move(datum) };

					Assert::AreEqual(1, movedDatum.FrontAsInt());
					Assert::AreEqual(2, movedDatum.GetAsInt(1));
					Assert::AreEqual(3, movedDatum.BackAsInt());
				}

				{
					Datum datum{ Datum::DatumTypes::Integer };
					datum.PushBack(1);
					datum.PushBack(2);
					datum.PushBack(3);

					Datum movedDatum{};

					movedDatum = std::move(datum);

					Assert::AreEqual(1, movedDatum.FrontAsInt());
					Assert::AreEqual(2, movedDatum.GetAsInt(1));
					Assert::AreEqual(3, movedDatum.BackAsInt());
				}
			}
			{
				{
					Datum datum{ Datum::DatumTypes::Float };
					datum.PushBack(1.2f);
					datum.PushBack(1.3f);
					datum.PushBack(1.4f);

					Datum movedDatum{ std::move(datum) };

					Assert::AreEqual(1.2f, movedDatum.FrontAsFloat());
					Assert::AreEqual(1.3f, movedDatum.GetAsFloat(1));
					Assert::AreEqual(1.4f, movedDatum.BackAsFloat());
				}

				{
					Datum datum{ Datum::DatumTypes::Float };
					datum.PushBack(1.2f);
					datum.PushBack(1.3f);
					datum.PushBack(1.4f);

					Datum movedDatum{};

					movedDatum = std::move(datum);

					Assert::AreEqual(1.2f, movedDatum.FrontAsFloat());
					Assert::AreEqual(1.3f, movedDatum.GetAsFloat(1));
					Assert::AreEqual(1.4f, movedDatum.BackAsFloat());
				}
			}

			{
				{
					Datum datum{ Datum::DatumTypes::String };
					datum.PushBack(std::string{ "String1" });
					datum.PushBack(std::string{ "String2" });
					datum.PushBack(std::string{ "String3" });

					Datum movedDatum{ std::move(datum) };

					Assert::AreEqual(std::string{ "String1" }, movedDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String2" }, movedDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String3" }, movedDatum.BackAsString());
				}

				{
					Datum datum{ Datum::DatumTypes::String };
					datum.PushBack(std::string{ "String1" });
					datum.PushBack(std::string{ "String2" });
					datum.PushBack(std::string{ "String3" });

					Datum movedDatum{};

					movedDatum = std::move(datum);

					Assert::AreEqual(std::string{ "String1" }, movedDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String2" }, movedDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String3" }, movedDatum.BackAsString());
				}
			}


			{
				{
					Datum datum{ Datum::DatumTypes::Vector };
					datum.PushBack(glm::vec4{ 0,0,0,0 });
					datum.PushBack(glm::vec4{ 1,1,1,1 });
					datum.PushBack(glm::vec4{ 2,2,2,2 });

					Datum movedDatum{ std::move(datum) };

					Assert::AreEqual(glm::vec4{ 0,0,0,0 }, movedDatum.FrontAsVector());
					Assert::AreEqual(glm::vec4{ 1,1,1,1 }, movedDatum.GetAsVector(1));
					Assert::AreEqual(glm::vec4{ 2,2,2,2 }, movedDatum.BackAsVector());
				}

				{
					Datum datum{ Datum::DatumTypes::Vector };
					datum.PushBack(glm::vec4{ 0,0,0,0 });
					datum.PushBack(glm::vec4{ 1,1,1,1 });
					datum.PushBack(glm::vec4{ 2,2,2,2 });

					Datum movedDatum{};

					movedDatum = std::move(datum);

					Assert::AreEqual(glm::vec4{ 0,0,0,0 }, movedDatum.FrontAsVector());
					Assert::AreEqual(glm::vec4{ 1,1,1,1 }, movedDatum.GetAsVector(1));
					Assert::AreEqual(glm::vec4{ 2,2,2,2 }, movedDatum.BackAsVector());
				}
			}

			{
				{
					Datum datum{ Datum::DatumTypes::Matrix };
					datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
					datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
					datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });

					Datum movedDatum{ std::move(datum) };

					Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, movedDatum.FrontAsMatrix());
					Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, movedDatum.GetAsMatrix(1));
					Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, movedDatum.BackAsMatrix());
				}

				{
					Datum datum{ Datum::DatumTypes::Matrix };
					datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
					datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
					datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });

					Datum movedDatum{};

					movedDatum = std::move(datum);

					Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, movedDatum.FrontAsMatrix());
					Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, movedDatum.GetAsMatrix(1));
					Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, movedDatum.BackAsMatrix());
				}
			}

			{
				{
					Foo foo{};
					Foo otherFoo{ 1 };
					Foo anotherFoo{ 2 };
					RTTI* fooPointer{ &foo };
					RTTI* fooPointer2{ &otherFoo };
					RTTI* fooPointer3{ &anotherFoo };
					Datum datum{ Datum::DatumTypes::Pointer };
					datum.PushBack(fooPointer);
					datum.PushBack(fooPointer2);
					datum.PushBack(fooPointer3);

					Datum movedDatum{ std::move(datum) };

					Assert::AreEqual(true, fooPointer == movedDatum.FrontAsRTTI());
					Assert::AreEqual(true, fooPointer2 == movedDatum.GetAsRTTI(1));
					Assert::AreEqual(true, fooPointer3 == movedDatum.BackAsRTTI());
				}

				{
					Foo foo{};
					Foo otherFoo{ 1 };
					Foo anotherFoo{ 2 };
					RTTI* fooPointer{ &foo };
					RTTI* fooPointer2{ &otherFoo };
					RTTI* fooPointer3{ &anotherFoo };
					Datum datum{ Datum::DatumTypes::Pointer };
					datum.PushBack(fooPointer);
					datum.PushBack(fooPointer2);
					datum.PushBack(fooPointer3);

					Datum movedDatum{};

					movedDatum = std::move(datum);

					Assert::AreEqual(true, fooPointer == movedDatum.FrontAsRTTI());
					Assert::AreEqual(true, fooPointer2 == movedDatum.GetAsRTTI(1));
					Assert::AreEqual(true, fooPointer3 == movedDatum.BackAsRTTI());
				}
			}
		}

		TEST_METHOD(ExternalStorage)
		{
			{
				int array[] = { 1,2,3 };
				Datum datum{ Datum::DatumTypes::Integer };
				datum.SetStorage(array, 3);
				Assert::AreEqual(array[0], datum.FrontAsInt());
				Assert::AreEqual(array[1], datum.GetAsInt(1));
				Assert::AreEqual(array[2], datum.BackAsInt());

				{
					Datum copiedDatum{ datum };
					Assert::AreEqual(array[0], copiedDatum.FrontAsInt());
					Assert::AreEqual(array[1], copiedDatum.GetAsInt(1));
					Assert::AreEqual(array[2], copiedDatum.BackAsInt());

					Assert::AreEqual(array[0], datum.FrontAsInt());
					Assert::AreEqual(array[1], datum.GetAsInt(1));
					Assert::AreEqual(array[2], datum.BackAsInt());

					Assert::AreEqual(datum.Size(), copiedDatum.Size());
				}

				{
					int otherArray[] = { 4,5,6 };
					Datum copiedDatum{ Datum::DatumTypes::Integer };
					copiedDatum.SetStorage(otherArray, 3);

					Assert::AreEqual(otherArray[0], copiedDatum.FrontAsInt());
					Assert::AreEqual(otherArray[1], copiedDatum.GetAsInt(1));
					Assert::AreEqual(otherArray[2], copiedDatum.BackAsInt());

					copiedDatum = datum;

					Assert::AreEqual(array[0], copiedDatum.FrontAsInt());
					Assert::AreEqual(array[1], copiedDatum.GetAsInt(1));
					Assert::AreEqual(array[2], copiedDatum.BackAsInt());
				}

				{
					Datum assignedDatum{ Datum::DatumTypes::Integer };
					assignedDatum.PushBack(5);
					assignedDatum.PushBack(6);
					assignedDatum.PushBack(7);

					assignedDatum = datum;

					Assert::AreEqual(array[0], assignedDatum.FrontAsInt());
					Assert::AreEqual(array[1], assignedDatum.GetAsInt(1));
					Assert::AreEqual(array[2], assignedDatum.BackAsInt());
				}

				{
					Datum copiedDatum{ datum };

					Datum otherDatum{ Datum::DatumTypes::Integer };
					otherDatum.PushBack(5);
					otherDatum.PushBack(6);
					otherDatum.PushBack(7);

					copiedDatum = otherDatum;

					Assert::AreEqual(5, otherDatum.FrontAsInt());
					Assert::AreEqual(6, otherDatum.GetAsInt(1));
					Assert::AreEqual(7, otherDatum.BackAsInt());
				}

				{
					Datum movedDatum{ std::move(datum) };
					Assert::AreEqual(array[0], movedDatum.FrontAsInt());
					Assert::AreEqual(array[1], movedDatum.GetAsInt(1));
					Assert::AreEqual(array[2], movedDatum.BackAsInt());
				}
			}

			{
				float array[] = { 1.2f, 1.3f, 1.4f };
				Datum datum{ Datum::DatumTypes::Float };
				datum.SetStorage(array, 3);
				Assert::AreEqual(array[0], datum.FrontAsFloat());
				Assert::AreEqual(array[1], datum.GetAsFloat(1));
				Assert::AreEqual(array[2], datum.BackAsFloat());

				{
					Datum copiedDatum{ datum };
					Assert::AreEqual(array[0], copiedDatum.FrontAsFloat());
					Assert::AreEqual(array[1], copiedDatum.GetAsFloat(1));
					Assert::AreEqual(array[2], copiedDatum.BackAsFloat());

					Assert::AreEqual(array[0], datum.FrontAsFloat());
					Assert::AreEqual(array[1], datum.GetAsFloat(1));
					Assert::AreEqual(array[2], datum.BackAsFloat());

					Assert::AreEqual(datum.Size(), copiedDatum.Size());
				}

				{
					Datum assignedDatum{ Datum::DatumTypes::String };
					assignedDatum.PushBack(std::string{ "String1" });
					assignedDatum.PushBack(std::string{ "String2" });
					assignedDatum.PushBack(std::string{ "String3" });

					assignedDatum = datum;

					Assert::AreEqual(array[0], assignedDatum.FrontAsFloat());
					Assert::AreEqual(array[1], assignedDatum.GetAsFloat(1));
					Assert::AreEqual(array[2], assignedDatum.BackAsFloat());
				}

				{
					Datum copiedDatum{ datum };

					Datum otherDatum{ Datum::DatumTypes::Float };
					otherDatum.PushBack(5.0f);
					otherDatum.PushBack(6.0f);
					otherDatum.PushBack(7.0f);

					copiedDatum = otherDatum;

					Assert::AreEqual(5.0f, otherDatum.FrontAsFloat());
					Assert::AreEqual(6.0f, otherDatum.GetAsFloat(1));
					Assert::AreEqual(7.0f, otherDatum.BackAsFloat());
				}

				{
					Datum movedDatum{ std::move(datum) };
					Assert::AreEqual(array[0], movedDatum.FrontAsFloat());
					Assert::AreEqual(array[1], movedDatum.GetAsFloat(1));
					Assert::AreEqual(array[2], movedDatum.BackAsFloat());
				}
			}

			{
				std::string array[] = { "String1", "String2", "String3" };
				Datum datum{ Datum::DatumTypes::String };
				datum.SetStorage(array, 3);
				Assert::AreEqual(array[0], datum.FrontAsString());
				Assert::AreEqual(array[1], datum.GetAsString(1));
				Assert::AreEqual(array[2], datum.BackAsString());

				{
					Datum copiedDatum{ datum };
					Assert::AreEqual(array[0], copiedDatum.FrontAsString());
					Assert::AreEqual(array[1], copiedDatum.GetAsString(1));
					Assert::AreEqual(array[2], copiedDatum.BackAsString());

					Assert::AreEqual(array[0], datum.FrontAsString());
					Assert::AreEqual(array[1], datum.GetAsString(1));
					Assert::AreEqual(array[2], datum.BackAsString());

					Assert::AreEqual(datum.Size(), copiedDatum.Size());
				}

				{
					Datum assignedDatum{ Datum::DatumTypes::String };
					assignedDatum.PushBack(std::string{ "String1" });
					assignedDatum.PushBack(std::string{ "String2" });
					assignedDatum.PushBack(std::string{ "String3" });

					assignedDatum = datum;

					Assert::AreEqual(array[0], assignedDatum.FrontAsString());
					Assert::AreEqual(array[1], assignedDatum.GetAsString(1));
					Assert::AreEqual(array[2], assignedDatum.BackAsString());
				}

				{
					Datum copiedDatum{ datum };

					Datum otherDatum{ Datum::DatumTypes::String };
					otherDatum.PushBack(std::string{ "String4" });
					otherDatum.PushBack(std::string{ "String5" });
					otherDatum.PushBack(std::string{ "String6" });

					copiedDatum = otherDatum;

					Assert::AreEqual(std::string{"String4"}, otherDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String5" }, otherDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String6" }, otherDatum.BackAsString());
				}

				{
					Datum movedDatum{ std::move(datum) };
					Assert::AreEqual(array[0], movedDatum.FrontAsString());
					Assert::AreEqual(array[1], movedDatum.GetAsString(1));
					Assert::AreEqual(array[2], movedDatum.BackAsString());
				}
			}

			{
				glm::vec4 array[] = { {0,0,0,0}, {1,1,1,1}, {2,2,2,2} };
				Datum datum{ Datum::DatumTypes::Vector };
				datum.SetStorage(array, 3);
				Assert::AreEqual(array[0], datum.FrontAsVector());
				Assert::AreEqual(array[1], datum.GetAsVector(1));
				Assert::AreEqual(array[2], datum.BackAsVector());

				{
					Datum copiedDatum{ datum };
					Assert::AreEqual(array[0], copiedDatum.FrontAsVector());
					Assert::AreEqual(array[1], copiedDatum.GetAsVector(1));
					Assert::AreEqual(array[2], copiedDatum.BackAsVector());

					Assert::AreEqual(array[0], datum.FrontAsVector());
					Assert::AreEqual(array[1], datum.GetAsVector(1));
					Assert::AreEqual(array[2], datum.BackAsVector());

					Assert::AreEqual(datum.Size(), copiedDatum.Size());
				}

				{
					Datum assignedDatum{ Datum::DatumTypes::String };
					assignedDatum.PushBack(std::string{ "String1" });
					assignedDatum.PushBack(std::string{ "String2" });
					assignedDatum.PushBack(std::string{ "String3" });

					assignedDatum = datum;

					Assert::AreEqual(array[0], assignedDatum.FrontAsVector());
					Assert::AreEqual(array[1], assignedDatum.GetAsVector(1));
					Assert::AreEqual(array[2], assignedDatum.BackAsVector());
				}

				{
					Datum copiedDatum{ datum };

					Datum otherDatum{ Datum::DatumTypes::String };
					otherDatum.PushBack(std::string{ "String4" });
					otherDatum.PushBack(std::string{ "String5" });
					otherDatum.PushBack(std::string{ "String6" });

					copiedDatum = otherDatum;

					Assert::AreEqual(std::string{ "String4" }, otherDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String5" }, otherDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String6" }, otherDatum.BackAsString());
				}

				{
					Datum movedDatum{ std::move(datum) };
					Assert::AreEqual(array[0], movedDatum.FrontAsVector());
					Assert::AreEqual(array[1], movedDatum.GetAsVector(1));
					Assert::AreEqual(array[2], movedDatum.BackAsVector());
				}
			}

			{
				glm::mat4 array[] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.SetStorage(array, 3);
				Assert::AreEqual(array[0], datum.FrontAsMatrix());
				Assert::AreEqual(array[1], datum.GetAsMatrix(1));
				Assert::AreEqual(array[2], datum.BackAsMatrix());

				{
					Datum copiedDatum{ datum };
					Assert::AreEqual(array[0], copiedDatum.FrontAsMatrix());
					Assert::AreEqual(array[1], copiedDatum.GetAsMatrix(1));
					Assert::AreEqual(array[2], copiedDatum.BackAsMatrix());

					Assert::AreEqual(array[0], datum.FrontAsMatrix());
					Assert::AreEqual(array[1], datum.GetAsMatrix(1));
					Assert::AreEqual(array[2], datum.BackAsMatrix());

					Assert::AreEqual(datum.Size(), copiedDatum.Size());
				}

				{
					Datum assignedDatum{ Datum::DatumTypes::String };
					assignedDatum.PushBack(std::string{ "String1" });
					assignedDatum.PushBack(std::string{ "String2" });
					assignedDatum.PushBack(std::string{ "String3" });

					assignedDatum = datum;

					Assert::AreEqual(array[0], assignedDatum.FrontAsMatrix());
					Assert::AreEqual(array[1], assignedDatum.GetAsMatrix(1));
					Assert::AreEqual(array[2], assignedDatum.BackAsMatrix());
				}

				{
					Datum copiedDatum{ datum };

					Datum otherDatum{ Datum::DatumTypes::String };
					otherDatum.PushBack(std::string{ "String4" });
					otherDatum.PushBack(std::string{ "String5" });
					otherDatum.PushBack(std::string{ "String6" });

					copiedDatum = otherDatum;

					Assert::AreEqual(std::string{ "String4" }, otherDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String5" }, otherDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String6" }, otherDatum.BackAsString());
				}

				{
					Datum movedDatum{ std::move(datum) };
					Assert::AreEqual(array[0], movedDatum.FrontAsMatrix());
					Assert::AreEqual(array[1], movedDatum.GetAsMatrix());
					Assert::AreEqual(array[2], movedDatum.BackAsMatrix());
				}
			}

			{
				Foo foo{};
				Foo foo2{ 1 };
				Foo foo3{ 2 };
				RTTI* array[] = {&foo, &foo2, &foo3};
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.SetStorage(array, 3);
				Assert::AreEqual(true, array[0] == datum.FrontAsRTTI());
				Assert::AreEqual(true, array[1] == datum.GetAsRTTI(1));
				Assert::AreEqual(true, array[2] == datum.BackAsRTTI());

				{
					Datum copiedDatum{ datum };
					Assert::AreEqual(true, array[0] == copiedDatum.FrontAsRTTI());
					Assert::AreEqual(true, array[1] == copiedDatum.GetAsRTTI(1));
					Assert::AreEqual(true, array[2] == copiedDatum.BackAsRTTI());

					Assert::AreEqual(true, array[0] == datum.FrontAsRTTI());
					Assert::AreEqual(true, array[1] == datum.GetAsRTTI(1));
					Assert::AreEqual(true, array[2] == datum.BackAsRTTI());

					Assert::AreEqual(datum.Size(), copiedDatum.Size());
				}

				{
					Datum assignedDatum{ Datum::DatumTypes::String };
					assignedDatum.PushBack(std::string{ "String1" });
					assignedDatum.PushBack(std::string{ "String2" });
					assignedDatum.PushBack(std::string{ "String3" });

					assignedDatum = datum;

					Assert::AreEqual(true, array[0] == assignedDatum.FrontAsRTTI());
					Assert::AreEqual(true, array[1] == assignedDatum.GetAsRTTI(1));
					Assert::AreEqual(true, array[2] == assignedDatum.BackAsRTTI());
				}

				{
					Datum copiedDatum{ datum };

					Datum otherDatum{ Datum::DatumTypes::String };
					otherDatum.PushBack(std::string{ "String4" });
					otherDatum.PushBack(std::string{ "String5" });
					otherDatum.PushBack(std::string{ "String6" });

					copiedDatum = otherDatum;

					Assert::AreEqual(std::string{ "String4" }, otherDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String5" }, otherDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String6" }, otherDatum.BackAsString());
				}

				{
					Datum movedDatum{ std::move(datum) };
					Assert::AreEqual(true, array[0] == movedDatum.FrontAsRTTI());
					Assert::AreEqual(true, array[1] == movedDatum.GetAsRTTI(1));
					Assert::AreEqual(true, array[2] == movedDatum.BackAsRTTI());
				}
			}
		}

		TEST_METHOD(Assignment)
		{

			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				{
					Datum copiedDatum{ Datum::DatumTypes::Float };
					copiedDatum.PushBack(5.0f);
					copiedDatum.PushBack(5.0f);
					copiedDatum.PushBack(5.0f);

					copiedDatum = datum;
					Assert::AreEqual(1, copiedDatum.FrontAsInt());
					Assert::AreEqual(2, copiedDatum.GetAsInt(1));
					Assert::AreEqual(3, copiedDatum.BackAsInt());
				}

				{
					Datum assignedDatum{};
					assignedDatum = 2;
					Assert::AreEqual(2, assignedDatum.FrontAsInt());
					Assert::AreEqual(2, assignedDatum.BackAsInt());
				}
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				{
					Datum copiedDatum{ Datum::DatumTypes::Integer };
					copiedDatum.PushBack(1);
					copiedDatum.PushBack(2);
					copiedDatum.PushBack(3);

					copiedDatum = datum;
					Assert::AreEqual(1.2f, copiedDatum.FrontAsFloat());
					Assert::AreEqual(1.3f, copiedDatum.GetAsFloat(1));
					Assert::AreEqual(1.4f, copiedDatum.BackAsFloat());
				}

				{
					Datum assignedDatum{};
					assignedDatum = 3.7f;
					Assert::AreEqual(3.7f, assignedDatum.FrontAsFloat());
					Assert::AreEqual(3.7f, assignedDatum.BackAsFloat());
				}
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				{
					Datum copiedDatum{ Datum::DatumTypes::Integer };
					copiedDatum.PushBack(1);
					copiedDatum.PushBack(2);
					copiedDatum.PushBack(3);

					copiedDatum = datum;
					Assert::AreEqual(std::string{ "String1" }, copiedDatum.FrontAsString());
					Assert::AreEqual(std::string{ "String2" }, copiedDatum.GetAsString(1));
					Assert::AreEqual(std::string{ "String3" }, copiedDatum.BackAsString());
				}

				{
					Datum assignedDatum{};

					assignedDatum = "NewString";
					Assert::AreEqual(std::string{ "NewString" }, assignedDatum.FrontAsString());
					Assert::AreEqual(std::string{ "NewString" }, assignedDatum.BackAsString());
				}
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				Datum copiedDatum{ Datum::DatumTypes::String };
				copiedDatum.PushBack(std::string{ "String" });
				copiedDatum.PushBack(std::string{ "String" });
				copiedDatum.PushBack(std::string{ "String" });


				copiedDatum = datum;
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, copiedDatum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 1,1,1,1 }, copiedDatum.GetAsVector(1));
				Assert::AreEqual(glm::vec4{ 2,2,2,2 }, copiedDatum.BackAsVector());
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				{
					Datum copiedDatum{ Datum::DatumTypes::Integer };
					copiedDatum.PushBack(1);
					copiedDatum.PushBack(2);
					copiedDatum.PushBack(3);

					copiedDatum = datum;
					Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, copiedDatum.FrontAsMatrix());
					Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, copiedDatum.GetAsMatrix(1));
					Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, copiedDatum.BackAsMatrix());
				}

				{
					Datum assignedDatum{};

					assignedDatum = glm::mat4{ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 };
					Assert::AreEqual(glm::mat4{ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 }, assignedDatum.FrontAsMatrix());
					Assert::AreEqual(glm::mat4{ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 }, assignedDatum.BackAsMatrix());
				}
			}

			{
				Foo foo{};
				Foo foo2{ 1 };
				Foo foo3{ 2 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &foo2 };
				RTTI* fooPointer3{ &foo3 };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				{
					Datum copiedDatum{ Datum::DatumTypes::Integer };
					copiedDatum.PushBack(1);
					copiedDatum.PushBack(2);
					copiedDatum.PushBack(3);

					copiedDatum = datum;
					Assert::AreEqual(true, fooPointer == copiedDatum.FrontAsRTTI());
					Assert::AreEqual(true, fooPointer2 == copiedDatum.GetAsRTTI(1));
					Assert::AreEqual(true, fooPointer3 == copiedDatum.BackAsRTTI());
				}

				{
					Foo foo4{};
					RTTI* fooPointer4{ &foo4 };

					Datum assignedDatum{};

					assignedDatum = fooPointer4;
					Assert::AreEqual(true, fooPointer4 == assignedDatum.FrontAsRTTI());
					Assert::AreEqual(true, fooPointer4 == assignedDatum.BackAsRTTI());
				}
			}
		}

		TEST_METHOD(SetType)
		{
			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::Integer);
				Assert::IsTrue(Datum::DatumTypes::Integer == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::Float);
				Assert::IsTrue(Datum::DatumTypes::Float == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::String);
				Assert::IsTrue(Datum::DatumTypes::String == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::Vector);
				Assert::IsTrue(Datum::DatumTypes::Vector == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::Matrix);
				Assert::IsTrue(Datum::DatumTypes::Matrix == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::Pointer);
				Assert::IsTrue(Datum::DatumTypes::Pointer == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}

			{
				Datum datum;
				datum.SetType(Datum::DatumTypes::Integer);
				datum.SetType(Datum::DatumTypes::Unknown);
				Assert::IsTrue(Datum::DatumTypes::Integer == datum.Type());
				Assert::IsFalse(Datum::DatumTypes::Unknown == datum.Type());
			}
		}

		TEST_METHOD(PushBack)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				Assert::AreEqual(std::size_t{ 3 }, datum.Capacity());

				const Datum constDatum{ datum };
				Assert::AreEqual(1, constDatum.FrontAsInt());
				Assert::AreEqual(2, constDatum.GetAsInt(1));
				Assert::AreEqual(3, constDatum.BackAsInt());
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				Assert::AreEqual(std::size_t{ 3 }, datum.Capacity());

				const Datum constDatum{ datum };
				Assert::AreEqual(1.2f, constDatum.FrontAsFloat());
				Assert::AreEqual(1.3f, constDatum.GetAsFloat(1));
				Assert::AreEqual(1.4f, constDatum.BackAsFloat());
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				Assert::AreEqual(std::size_t{ 3 }, datum.Capacity());

				const Datum constDatum{ datum };
				Assert::AreEqual(std::string{ "String1" }, constDatum.FrontAsString());
				Assert::AreEqual(std::string{ "String2" }, constDatum.GetAsString(1));
				Assert::AreEqual(std::string{ "String3" }, constDatum.BackAsString());

				std::string string{ "String4" };
				datum.PushBack(string);

				Assert::AreEqual(std::size_t{ 4 }, datum.Size());
				Assert::AreEqual(std::string{ "String1" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String2" }, datum.GetAsString(1));
				Assert::AreEqual(std::string{ "String3" }, datum.GetAsString(2));
				Assert::AreEqual(std::string{ "String4" }, datum.BackAsString());
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				Assert::AreEqual(std::size_t{ 3 }, datum.Capacity());

				const Datum constDatum{ datum };
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, constDatum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 1,1,1,1 }, constDatum.GetAsVector(1));
				Assert::AreEqual(glm::vec4{ 2,2,2,2 }, constDatum.BackAsVector());

				glm::vec4 vec{ 1,2,3,4 };
				datum.PushBack(vec);

				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 1,1,1,1 }, datum.GetAsVector(1));
				Assert::AreEqual(glm::vec4{ 2,2,2,2 }, datum.GetAsVector(2));
				Assert::AreEqual(vec, datum.BackAsVector());

			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				Assert::AreEqual(std::size_t{ 3 }, datum.Capacity());

				const Datum constDatum{ datum };
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, constDatum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, constDatum.GetAsMatrix(1));
				Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, constDatum.BackAsMatrix());

				glm::mat4 matrix{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
				datum.PushBack(matrix);

				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, datum.GetAsMatrix(1));
				Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, datum.GetAsMatrix(2));
				Assert::AreEqual(matrix, datum.BackAsMatrix());
			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());
				Assert::AreEqual(std::size_t{ 3 }, datum.Capacity());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, fooPointer == constDatum.FrontAsRTTI());
				Assert::AreEqual(true, fooPointer2 == constDatum.GetAsRTTI(1));
				Assert::AreEqual(true, fooPointer3 == constDatum.BackAsRTTI());
			}
		}

		TEST_METHOD(Resize)
		{

			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(1, datum.FrontAsInt());
				Assert::AreEqual(2, datum.BackAsInt());

				datum.Resize(1);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(1, datum.FrontAsInt());
				Assert::AreEqual(1, datum.BackAsInt());

				datum.Resize(0);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PushBack(3);
				datum.PushBack(4);

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(3, datum.FrontAsInt());
				Assert::AreEqual(4, datum.BackAsInt());
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(2.7f);

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(1.2f, datum.FrontAsFloat());
				Assert::AreEqual(2.7f, datum.BackAsFloat());

				datum.Resize(1);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(1.2f, datum.FrontAsFloat());
				Assert::AreEqual(1.2f, datum.BackAsFloat());

				datum.Resize(0);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PushBack(3.2f);
				datum.PushBack(4.7f);

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(3.2f, datum.FrontAsFloat());
				Assert::AreEqual(4.7f, datum.BackAsFloat());
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(std::string{ "String1" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String2" }, datum.BackAsString());

				datum.Resize(1);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(std::string{ "String1" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String1" }, datum.BackAsString());

				datum.Resize(0);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PushBack(std::string{ "String3" });
				datum.PushBack(std::string{ "String4" });

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(std::string{ "String3" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String4" }, datum.BackAsString());
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 1,1,1,1 }, datum.BackAsVector());

				datum.Resize(1);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.BackAsVector());

				datum.Resize(0);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PushBack(glm::vec4{ 2,2,2,2 });
				datum.PushBack(glm::vec4{ 3,3,3,3 });

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::vec4{ 2,2,2,2 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 3,3,3,3 }, datum.BackAsVector());
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, datum.BackAsMatrix());

				datum.Resize(1);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.BackAsMatrix());

				datum.Resize(0);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				datum.PushBack(glm::mat4{ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 });

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 }, datum.BackAsMatrix());
			}

			{

				Foo foo1{ 1 };
				Foo foo2{ 2 };
				Foo foo3{ 3 };
				Foo foo4{ 4 };

				RTTI* fooPointer1{ &foo1 };
				RTTI* fooPointer2{ &foo2 };
				RTTI* fooPointer3{ &foo3 };
				RTTI* fooPointer4{ &foo4 };

				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(&foo1);
				datum.PushBack(&foo2);

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::IsTrue(fooPointer1 == datum.FrontAsRTTI());
				Assert::IsTrue(fooPointer2 == datum.BackAsRTTI());

				datum.Resize(1);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::IsTrue(fooPointer1 == datum.FrontAsRTTI());
				Assert::IsTrue(fooPointer1 == datum.BackAsRTTI());

				datum.Resize(0);
				Assert::AreEqual(datum.Size(), datum.Capacity());

				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PushBack(fooPointer3);
				datum.PushBack(fooPointer4);

				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::IsTrue(fooPointer3 == datum.FrontAsRTTI());
				Assert::IsTrue(fooPointer4 == datum.BackAsRTTI());
			}
		}

		TEST_METHOD(Get)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				Assert::AreEqual(1, datum.GetAsInt(0));
				Assert::AreEqual(2, datum.GetAsInt(1));
				Assert::AreEqual(3, datum.GetAsInt(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);
				Assert::AreEqual(1.2f, datum.GetAsFloat(0));
				Assert::AreEqual(1.3f, datum.GetAsFloat(1));
				Assert::AreEqual(1.4f, datum.GetAsFloat(2));
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });
				Assert::AreEqual(std::string{ "String1" }, datum.GetAsString(0));
				Assert::AreEqual(std::string{ "String2" }, datum.GetAsString(1));
				Assert::AreEqual(std::string{ "String3" }, datum.GetAsString(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{0,0,0,0});
				datum.PushBack(glm::vec4{1,1,1,1});
				datum.PushBack(glm::vec4{2,2,2,2});
				Assert::IsTrue(glm::vec4{ 0,0,0,0 } == datum.GetAsVector(0));
				Assert::IsTrue(glm::vec4{ 1,1,1,1 } == datum.GetAsVector(1));
				Assert::IsTrue(glm::vec4{ 2,2,2,2 } == datum.GetAsVector(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0});
				datum.PushBack(glm::mat4{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1});
				datum.PushBack(glm::mat4{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2});
				Assert::IsTrue(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } == datum.GetAsMatrix(0));
				Assert::IsTrue(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } == datum.GetAsMatrix(1));
				Assert::IsTrue(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 } == datum.GetAsMatrix(2));
			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				Assert::IsTrue(fooPointer == datum.GetAsRTTI(0));
				Assert::IsTrue(fooPointer2 == datum.GetAsRTTI(1));
				Assert::IsTrue(fooPointer3 == datum.GetAsRTTI(2));

			}
		}

		TEST_METHOD(Comparison)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, datum == constDatum);
				Assert::AreEqual(false, datum != constDatum);

				Datum assignedDatum{};
				assignedDatum = 7;
				Assert::IsTrue(7 == assignedDatum);
				Assert::IsTrue(9 != assignedDatum);
				Assert::IsFalse(7 != assignedDatum);
				Assert::IsFalse(9 == assignedDatum);

				Assert::IsTrue(assignedDatum == 7);
				Assert::IsTrue(assignedDatum != 9);
				Assert::IsFalse(assignedDatum != 7);
				Assert::IsFalse(assignedDatum == 9);
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, datum == constDatum);
				Assert::AreEqual(false, datum != constDatum);

				Datum assignedDatum{};
				assignedDatum = 7.0f;
				Assert::IsTrue(7.0f == assignedDatum);
				Assert::IsTrue(7.1f != assignedDatum);
				Assert::IsFalse(7.0f != assignedDatum);
				Assert::IsFalse(7.1f == assignedDatum);

				Assert::IsTrue(assignedDatum == 7.0f);
				Assert::IsTrue(assignedDatum != 7.1f);
				Assert::IsFalse(assignedDatum != 7.0f);
				Assert::IsFalse(assignedDatum == 7.1f);
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, datum == constDatum);
				Assert::AreEqual(false, datum != constDatum);

				Datum otherDatum{ Datum::DatumTypes::String };
				otherDatum.PushBack(std::string{ "String4" });
				otherDatum.PushBack(std::string{ "String5" });
				otherDatum.PushBack(std::string{ "String6" });
				Assert::IsFalse(datum == otherDatum);
				Assert::IsTrue(datum != otherDatum);


				Datum assignedDatum{};
				assignedDatum = "String";
				Assert::IsTrue("String" == assignedDatum);
				Assert::IsFalse("String" != assignedDatum);
				Assert::IsFalse("String2" == assignedDatum);
				Assert::IsTrue("String2" != assignedDatum);

				Assert::IsTrue(assignedDatum == "String");
				Assert::IsFalse(assignedDatum != "String");
				Assert::IsFalse(assignedDatum == "String2");
				Assert::IsTrue(assignedDatum != "String2");
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, datum == constDatum);
				Assert::AreEqual(false, datum != constDatum);

				Datum assignedDatum{};
				assignedDatum = glm::vec4{ 0,0,0,0 };
				Assert::IsTrue(glm::vec4{ 0,0,0,0 } == assignedDatum);
				Assert::IsTrue(glm::vec4{ 1,1,1,1 } != assignedDatum);
				Assert::IsFalse(glm::vec4{ 0,0,0,0 } != assignedDatum);
				Assert::IsFalse(glm::vec4{ 1,1,1,1 } == assignedDatum);

				Assert::IsTrue(assignedDatum == glm::vec4{ 0,0,0,0 });
				Assert::IsTrue(assignedDatum != glm::vec4{ 1,1,1,1 });
				Assert::IsFalse(assignedDatum != glm::vec4{ 0,0,0,0 });
				Assert::IsFalse(assignedDatum == glm::vec4{ 1,1,1,1 });
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, datum == constDatum);
				Assert::AreEqual(false, datum != constDatum);

				Datum assignedDatum{};
				assignedDatum = glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
				Assert::IsTrue(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } == assignedDatum);
				Assert::IsTrue(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } != assignedDatum);
				Assert::IsFalse(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } != assignedDatum);
				Assert::IsFalse(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } == assignedDatum);

				Assert::IsTrue(assignedDatum == glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				Assert::IsTrue(assignedDatum != glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				Assert::IsFalse(assignedDatum != glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				Assert::IsFalse(assignedDatum == glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });

				glm::mat4 a{};
			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };

				Foo foo1{ 4 };
				Foo foo2{ 5 };
				Foo foo3{ 6 };

				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				const Datum constDatum{ datum };
				Assert::AreEqual(true, datum == constDatum);
				Assert::AreEqual(false, datum != constDatum);

				Datum differentDatum{ Datum::DatumTypes::Pointer };
				differentDatum.PushBack(&foo1);
				differentDatum.PushBack(&foo2);
				differentDatum.PushBack(&foo3);

				Assert::IsTrue(datum != differentDatum);
				Assert::IsFalse(datum == differentDatum);

				Datum assignedDatum{};
				assignedDatum = &foo;
				Assert::IsTrue(&foo == assignedDatum);
				Assert::IsTrue(&otherFoo != assignedDatum);
				Assert::IsFalse(&foo != assignedDatum);
				Assert::IsFalse(&otherFoo == assignedDatum);

				Assert::IsTrue(assignedDatum == &foo);
				Assert::IsTrue(assignedDatum != &otherFoo);
				Assert::IsFalse(assignedDatum != &foo);
				Assert::IsFalse(assignedDatum == &otherFoo);
			}

			{
				Datum datum{};
				Datum otherDatum{};
				Assert::IsTrue(datum == otherDatum);
				Assert::IsFalse(datum != otherDatum);
			}
		}

		TEST_METHOD(Set)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				datum.Set(7, 2);
				Assert::AreEqual(7, datum.GetAsInt(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);
				datum.Set(2.7f, 2);
				Assert::AreEqual(2.7f, datum.GetAsFloat(2));
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });
				datum.Set("NewString", 2);
				Assert::AreEqual(std::string{ "NewString" }, datum.GetAsString(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });
				datum.Set(glm::vec4{ 1,2,3,4 }, 2);
				Assert::IsTrue(glm::vec4{ 1,2,3,4 } == datum.GetAsVector(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				datum.Set(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, 2);
				Assert::IsTrue(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 } == datum.GetAsMatrix(2));
			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };
				Foo newFoo{ 3 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				datum.Set(&newFoo, 2);
				Assert::IsTrue(&newFoo == datum.GetAsRTTI(2));
			}
		}

		TEST_METHOD(Remove)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				datum.Remove(2);
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(1, datum.FrontAsInt());
				Assert::AreEqual(3, datum.BackAsInt());
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.0f);
				datum.PushBack(2.0f);
				datum.PushBack(3.0f);
				datum.Remove(2.0f);
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(1.0f, datum.FrontAsFloat());
				Assert::AreEqual(3.0f, datum.BackAsFloat());
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String" });
				datum.PushBack(std::string{ "AnotherString" });
				datum.PushBack(std::string{ "ThirdString" });
				datum.Remove("AnotherString");
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(std::string{ "String" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "ThirdString" }, datum.BackAsString());
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });
				datum.Remove(glm::vec4{ 1,1,1,1 });
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 2,2,2,2 }, datum.BackAsVector());
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				datum.Remove(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }, datum.BackAsMatrix());
			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				datum.Remove(fooPointer2);
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(true, fooPointer == datum.FrontAsRTTI());
				Assert::AreEqual(true, fooPointer3 == datum.BackAsRTTI());
			}


		}

		TEST_METHOD(RemoveAt)
		{
			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String" });
				datum.PushBack(std::string{ "AnotherString" });
				datum.PushBack(std::string{ "ThirdString" });
				datum.RemoveAt(1);
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(std::string{ "String" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "ThirdString" }, datum.GetAsString(1));
			}
		}

		TEST_METHOD(PopBack)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(1, datum.FrontAsInt());
				Assert::AreEqual(2, datum.BackAsInt());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(1, datum.FrontAsInt());
				Assert::AreEqual(1, datum.BackAsInt());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(1.2f, datum.FrontAsFloat());
				Assert::AreEqual(1.3f, datum.BackAsFloat());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(1.2f, datum.FrontAsFloat());
				Assert::AreEqual(1.2f, datum.BackAsFloat());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(std::string{ "String1" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String2" }, datum.BackAsString());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(std::string{ "String1" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String1" }, datum.BackAsString());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 1,1,1,1 }, datum.BackAsVector());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.BackAsVector());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, datum.BackAsMatrix());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, datum.BackAsMatrix());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);
				Assert::AreEqual(std::size_t{ 3 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 2 }, datum.Size());
				Assert::AreEqual(true, fooPointer == datum.FrontAsRTTI());
				Assert::AreEqual(true, fooPointer2 == datum.BackAsRTTI());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 1 }, datum.Size());
				Assert::AreEqual(true, fooPointer == datum.FrontAsRTTI());
				Assert::AreEqual(true, fooPointer == datum.BackAsRTTI());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());

				datum.PopBack();
				Assert::AreEqual(std::size_t{ 0 }, datum.Size());
			}
		}

		TEST_METHOD(ToString)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.PushBack(1);
				datum.PushBack(2);
				datum.PushBack(3);

				Assert::AreEqual(std::string{ "1" }, datum.ToString());
				Assert::AreEqual(std::string{ "2" }, datum.ToString(1));
				Assert::AreEqual(std::string{ "3" }, datum.ToString(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.PushBack(1.2f);
				datum.PushBack(1.3f);
				datum.PushBack(1.4f);

				Assert::AreEqual(std::string{ "1.200000" }, datum.ToString());
				Assert::AreEqual(std::string{ "1.300000" }, datum.ToString(1));
				Assert::AreEqual(std::string{ "1.400000" }, datum.ToString(2));
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.PushBack(std::string{ "String1" });
				datum.PushBack(std::string{ "String2" });
				datum.PushBack(std::string{ "String3" });

				Assert::AreEqual(datum.FrontAsString(), datum.ToString());
				Assert::AreEqual(datum.GetAsString(1), datum.ToString(1));
				Assert::AreEqual(datum.BackAsString(), datum.ToString(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.PushBack(glm::vec4{ 0,0,0,0 });
				datum.PushBack(glm::vec4{ 1,1,1,1 });
				datum.PushBack(glm::vec4{ 2,2,2,2 });

				Assert::AreEqual(std::string{"vec4(0.000000, 0.000000, 0.000000, 0.000000)"}, datum.ToString());
				Assert::AreEqual(std::string{"vec4(1.000000, 1.000000, 1.000000, 1.000000)"}, datum.ToString(1));
				Assert::AreEqual(std::string{"vec4(2.000000, 2.000000, 2.000000, 2.000000)"}, datum.ToString(2));
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.PushBack(glm::mat4{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
				datum.PushBack(glm::mat4{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 });
				datum.PushBack(glm::mat4{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 });

				Assert::AreEqual(std::string{"mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000))"}, datum.ToString());
				Assert::AreEqual(std::string{"mat4x4((1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000))"}, datum.ToString(1));
				Assert::AreEqual(std::string{"mat4x4((2.000000, 2.000000, 2.000000, 2.000000), (2.000000, 2.000000, 2.000000, 2.000000), (2.000000, 2.000000, 2.000000, 2.000000), (2.000000, 2.000000, 2.000000, 2.000000))"}, datum.ToString(2));

			}

			{
				Foo foo{};
				Foo otherFoo{ 1 };
				Foo anotherFoo{ 2 };
				RTTI* fooPointer{ &foo };
				RTTI* fooPointer2{ &otherFoo };
				RTTI* fooPointer3{ &anotherFoo };
				Datum datum{ Datum::DatumTypes::Pointer };
				datum.PushBack(fooPointer);
				datum.PushBack(fooPointer2);
				datum.PushBack(fooPointer3);

				Assert::AreEqual(std::string{"0"}, datum.ToString());
				Assert::AreEqual(std::string{"1"}, datum.ToString(1));
				Assert::AreEqual(std::string{"2"}, datum.ToString(2));
			}
		}

		TEST_METHOD(FromString)
		{
			{
				Datum datum{ Datum::DatumTypes::Integer };
				datum.Resize(3);
				datum.SetFromString("1");
				datum.SetFromString("2", 1);
				datum.SetFromString("3", 2);
				Assert::AreEqual(1, datum.FrontAsInt());
				Assert::AreEqual(2, datum.GetAsInt(1));
				Assert::AreEqual(3, datum.BackAsInt());
			}

			{
				Datum datum{ Datum::DatumTypes::Float };
				datum.Resize(3);
				datum.SetFromString("1.0");
				datum.SetFromString("2.7",1);
				datum.SetFromString("2.72",2);
				Assert::AreEqual(1.0f, datum.FrontAsFloat());
				Assert::AreEqual(2.7f, datum.GetAsFloat(1));
				Assert::AreEqual(2.72f, datum.BackAsFloat());
			}

			{
				Datum datum{ Datum::DatumTypes::String };
				datum.Resize(3);
				datum.SetFromString("String1");
				datum.SetFromString("String2",1);
				datum.SetFromString("String3",2);
				Assert::AreEqual(std::string{ "String1" }, datum.FrontAsString());
				Assert::AreEqual(std::string{ "String2" }, datum.GetAsString(1));
				Assert::AreEqual(std::string{ "String3" }, datum.BackAsString());
			}

			{
				Datum datum{ Datum::DatumTypes::Vector };
				datum.Resize(3);
				datum.SetFromString("vec4(0,0,0,0)");
				datum.SetFromString("vec4(1,1,1,1)",1);
				datum.SetFromString("vec4(2,2,2,2)",2);
				Assert::AreEqual(glm::vec4{ 0,0,0,0 }, datum.FrontAsVector());
				Assert::AreEqual(glm::vec4{ 1,1,1,1 }, datum.GetAsVector(1));
				Assert::AreEqual(glm::vec4{ 2,2,2,2 }, datum.BackAsVector());
			}

			{
				Datum datum{ Datum::DatumTypes::Matrix };
				datum.Resize(3);
				datum.SetFromString("mat4x4((0,0,0,0), (0,0,0,0), (0,0,0,0), (0,0,0,0))");
				datum.SetFromString("mat4x4((1,1,1,1), (1,1,1,1), (1,1,1,1), (1,1,1,1))",1);
				datum.SetFromString("mat4x4((2,2,2,2), (2,2,2,2), (2,2,2,2), (2,2,2,2))",2);
				Assert::AreEqual(glm::mat4{ glm::vec4{ 0,0,0,0 }, glm::vec4{ 0,0,0,0 }, glm::vec4{ 0,0,0,0 }, glm::vec4{ 0,0,0,0 } }, datum.FrontAsMatrix());
				Assert::AreEqual(glm::mat4{ glm::vec4{ 1,1,1,1 }, glm::vec4{ 1,1,1,1 }, glm::vec4{ 1,1,1,1 }, glm::vec4{ 1,1,1,1 } }, datum.GetAsMatrix(1));
				Assert::AreEqual(glm::mat4{ glm::vec4{ 2,2,2,2 }, glm::vec4{ 2,2,2,2 }, glm::vec4{ 2,2,2,2 }, glm::vec4{ 2,2,2,2 } }, datum.BackAsMatrix());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}