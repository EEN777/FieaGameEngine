#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include "Vector.h"
#include "Foo.h"
#include "UserSuppliedIncrement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace LibraryDesktopTests
{
	TEST_CLASS(VectorTests)
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
		const Foo expectedFoo{ 7 };
		const Foo additionalFoo{ 8 };


		TEST_METHOD(Constructor)
		{
			{
				Vector<Foo> vector{ UserIncrement<Foo>{} };
				Assert::AreEqual(size_t{ 0 }, vector.Size());
			}

			{
				Vector<Foo> vector{ expectedFoo,additionalFoo,additionalFoo,expectedFoo };
				Assert::AreEqual(std::size_t{ 4 }, vector.Size());
				Assert::AreEqual(expectedFoo, vector.Front());
				Assert::AreEqual(additionalFoo, vector[1]);
				Assert::AreEqual(additionalFoo, vector[2]);
				Assert::AreEqual(expectedFoo, vector.Back());
			}

			{
				Vector<Foo> vector{ std::initializer_list{expectedFoo, additionalFoo, additionalFoo, expectedFoo}, UserIncrement<Foo>{} };
				vector.EmplaceBack();
				Assert::AreEqual(std::size_t{ 14 }, vector.Capacity());
				Assert::AreEqual(std::size_t{ 5 }, vector.Size());
			}

			{
				const Vector<Foo> vector{ std::initializer_list{expectedFoo, additionalFoo, additionalFoo, expectedFoo}, UserIncrement<Foo>{} };
				Assert::AreEqual(std::size_t{ 4 }, vector.Size());
				Assert::AreEqual(expectedFoo, vector.Front());
				Assert::AreEqual(additionalFoo, vector[1]);
				Assert::AreEqual(additionalFoo, vector[2]);
				Assert::AreEqual(expectedFoo, vector.Back());
			}
		}

		TEST_METHOD(PushBack)
		{
			Vector<Foo> vector;
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});
			vector.PushBack(expectedFoo);

			Assert::AreEqual(expectedFoo, vector[6]);
			Assert::AreEqual(size_t{ 7 }, vector.Size());
		}

		TEST_METHOD(PopBack)
		{
			Vector<Foo> vector;
			vector.PushBack(expectedFoo);
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});

			vector.PopBack();
			vector.PopBack();
			Assert::AreEqual(vector.Back(), expectedFoo);
			Assert::AreEqual(std::size_t{ 1 }, vector.Size());
			vector.PopBack();
			Assert::AreEqual(std::size_t{ 0 }, vector.Size());
		}

		TEST_METHOD(CopySemantics)
		{
			Vector<Foo> vector;
			vector.PushBack(additionalFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(additionalFoo);

			Vector<Foo> copiedVector{ vector };
			Vector<Foo> emptyVector{};
			emptyVector.ShrinkToFit();
			Vector<Foo> anotherVector;
			anotherVector.PushBack(expectedFoo);
			anotherVector.PushBack(expectedFoo);
			anotherVector.PushBack(expectedFoo);

			Vector<Foo> largerVector;
			largerVector.PushBack(expectedFoo);
			largerVector.PushBack(expectedFoo);
			largerVector.PushBack(expectedFoo);			
			largerVector.PushBack(expectedFoo);
			largerVector.PushBack(expectedFoo);			
			largerVector.PushBack(expectedFoo);
			largerVector.PushBack(expectedFoo);			
			largerVector.PushBack(expectedFoo);
			largerVector.PushBack(expectedFoo);			
			largerVector.PushBack(expectedFoo);
			largerVector.PushBack(expectedFoo);

			emptyVector = vector;
			anotherVector = vector;
			largerVector = vector;

			Assert::AreEqual(vector.Size(), emptyVector.Size());
			Assert::AreEqual(vector.Size(), anotherVector.Size());
			Assert::AreEqual(vector.Size(), largerVector.Size());
			Assert::AreEqual(vector.Capacity(), emptyVector.Capacity());
			Assert::AreEqual(vector.Capacity(), anotherVector.Capacity());
			Assert::AreNotEqual(vector.Capacity(), largerVector.Capacity());

			for(std::size_t position{ 0 }; position < vector.Size(); ++position)
			{
				Assert::AreEqual(vector[position], copiedVector[position]);
				Assert::AreEqual(vector[position], emptyVector[position]);
				Assert::AreEqual(vector[position], anotherVector[position]);
				Assert::AreEqual(vector[position], largerVector[position]);
			}

		}

		TEST_METHOD(ShrinkToFit)
		{
			Vector<Foo> vector;
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);

			Assert::AreEqual(std::size_t{ 15 }, vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(std::size_t{ 11 }, vector.Capacity());
		}	

		TEST_METHOD(IteratorEquality)
		{
			Vector<Foo> vector{ expectedFoo, expectedFoo, expectedFoo };
			Assert::AreNotEqual(vector.begin(), vector.end());
			Assert::AreEqual(vector.begin(), vector.begin());
			Assert::IsTrue(vector.begin() < vector.end());
			Assert::IsTrue(vector.begin() <= vector.end());
			Assert::IsTrue(vector.end() > vector.begin());
			Assert::IsTrue(vector.end() >= vector.begin());
			Assert::IsFalse(vector.end() < vector.begin());
			Assert::IsFalse(vector.end() <= vector.begin());
			Assert::IsFalse(vector.begin() > vector.end());
			Assert::IsFalse(vector.begin() >= vector.end());

			Assert::AreNotEqual(vector.cbegin(), vector.cend());
			Assert::AreNotEqual(vector.cbegin(), vector.cend());
			Assert::AreEqual(vector.cbegin(), vector.cbegin());
			Assert::IsTrue(vector.cbegin() < vector.cend());
			Assert::IsTrue(vector.cbegin() <= vector.cend());
			Assert::IsTrue(vector.cend() > vector.cbegin());
			Assert::IsTrue(vector.cend() >= vector.cbegin());
			Assert::IsFalse(vector.cend() < vector.cbegin());
			Assert::IsFalse(vector.cend() <= vector.cbegin());
			Assert::IsFalse(vector.cbegin() > vector.cend());
			Assert::IsFalse(vector.cbegin() >= vector.cend());

			Vector<Foo>::ConstIterator constIterator{vector.begin()};
			Assert::AreEqual(vector.cbegin(), constIterator);
		}

		TEST_METHOD(BracketOperator)
		{
			Vector<Foo> vector;
			vector.PushBack(additionalFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(additionalFoo);

			Assert::AreEqual(expectedFoo, vector[1]);
			Assert::AreEqual(expectedFoo, vector.begin()[1]);
			const Vector<Foo> constVector{ vector };
			Assert::AreEqual(expectedFoo, constVector[1]);
			Assert::AreEqual(expectedFoo, constVector.begin()[1]);

#ifdef USING_EXCEPTIONS
			Vector<Foo>::Iterator iterator{};
			Assert::ExpectException<std::runtime_error>([&iterator] {iterator[0]; });
			Vector<Foo>::ConstIterator constIterator{};
			Assert::ExpectException<std::runtime_error>([&constIterator] {constIterator[0]; });
#endif
		}

		TEST_METHOD(FrontAndBack)
		{
			{
				Vector<Foo> vector;
				vector.PushBack(expectedFoo);
				Assert::AreEqual(expectedFoo, vector.Front());
				Assert::AreEqual(expectedFoo, vector.Back());
				Assert::AreEqual(vector.Front(), vector.Back());

				vector.PushBack(additionalFoo);
				Assert::AreNotEqual(vector.Front(), vector.Back());
				Assert::AreEqual(additionalFoo, vector.Back());
				Assert::AreEqual(expectedFoo, vector.Front());
			}
#ifdef USING_EXCEPTIONS
			{
				Vector<Foo> vector{};
				Assert::ExpectException<std::runtime_error>([&vector] {auto& temp = vector.Front(); temp; });
				Assert::ExpectException<std::runtime_error>([&vector] {auto& temp = vector.Back(); temp; });
			}
#endif
		}

		TEST_METHOD(At)
		{
			Vector<Foo> vector;
			vector.PushBack(additionalFoo);
			vector.PushBack(additionalFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(additionalFoo);
			Assert::AreEqual(expectedFoo, vector.At(2));
#ifdef USING_EXCEPTIONS
			Assert::ExpectException<std::out_of_range>([&vector]() {auto& element = vector.At(4); element; });
#endif

			const Vector<Foo> constVector{ vector };
			Assert::AreEqual(expectedFoo, constVector.At(2));
#ifdef USING_EXCEPTIONS
			Assert::ExpectException<std::out_of_range>([&constVector]() {auto& element = constVector.At(4); element; });
#endif
		}

		TEST_METHOD(IsEmpty)
		{
			Vector<Foo> vector;
			Assert::AreEqual(true, vector.IsEmpty());

			vector.PushBack(expectedFoo);
			Assert::AreEqual(false, vector.IsEmpty());
		}

		TEST_METHOD(Capacity)
		{
			Vector<Foo> vector;
			Assert::AreEqual(size_t{ 5 }, vector.Capacity());

			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);

			Assert::AreEqual(size_t{ 10 }, vector.Capacity());
		}

		TEST_METHOD(Clear)
		{
			Vector<Foo> vector;

			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);

			vector.Clear();

			Assert::AreEqual(size_t{ 0 }, vector.Size());
			Assert::AreEqual(size_t{ 10 }, vector.Capacity());

			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);

			Assert::AreEqual(size_t{ 7 }, vector.Size());
			Assert::AreEqual(size_t{ 10 }, vector.Capacity());
		}

		TEST_METHOD(Find)
		{
			Vector<Foo> vector;
			vector.PushBack(Foo{});
			vector.PushBack(Foo{});
			vector.PushBack(expectedFoo);
			vector.PushBack(Foo{});

			const Vector<Foo> constVector{ vector };

			Assert::AreEqual(vector.At(2), *vector.Find(expectedFoo));
			Assert::AreEqual(vector.At(2), *constVector.Find(expectedFoo));
		}

		TEST_METHOD(ForLoop)
		{
			Vector<Foo> vector;
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);

			const Vector<Foo> constVector{ vector };

			for(auto position{vector.begin()}; position != vector.end(); ++position)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ vector.begin() }; position != vector.end(); position++)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ vector.cbegin() }; position != vector.cend(); ++position)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ constVector.begin() }; position != constVector.end(); ++position)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ constVector.begin() }; position != constVector.end(); position++)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ vector.end() - 1 }; position != vector.begin(); position--)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ constVector.end() - 1 }; position != constVector.begin(); --position)
			{
				Assert::AreEqual(expectedFoo, *position);
			}

			for (auto position{ constVector.end() - 1 }; position != constVector.begin(); position--)
			{
				Assert::AreEqual(expectedFoo, *position);
			}
		}

		TEST_METHOD(Remove)
		{
			Vector<Foo> vector;
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			{
				Vector<Foo> fullRemoveVector{ vector };
				auto begin{ fullRemoveVector.begin() };
				auto end{ fullRemoveVector.end() };
				fullRemoveVector.Remove(begin, end);
				Assert::AreEqual(std::size_t{ 0 }, fullRemoveVector.Size());
				Assert::AreEqual(std::size_t{ 10 }, fullRemoveVector.Capacity());
			}

			{
				Vector<Foo> partialRemoveVector{ vector };
				auto begin{ partialRemoveVector.begin() };
				auto end{ partialRemoveVector.end() };
				begin += 2;
				--end;
				partialRemoveVector.Remove(begin, end);
				Assert::AreEqual(std::size_t{ 3 }, partialRemoveVector.Size());
				Assert::AreEqual(std::size_t{ 10 }, partialRemoveVector.Capacity());
			}

			{
				Vector<Foo> failedRemoveVector{ vector };
				failedRemoveVector.Remove(failedRemoveVector.begin(), failedRemoveVector.begin());
				failedRemoveVector.Remove(failedRemoveVector.end(), failedRemoveVector.begin());
				failedRemoveVector.Remove(failedRemoveVector.begin(), vector.end());
				failedRemoveVector.Remove(failedRemoveVector.end(), failedRemoveVector.end());
				failedRemoveVector.Remove(failedRemoveVector.begin(), failedRemoveVector.end() + 1);

				Assert::AreEqual(std::size_t{ 6 }, failedRemoveVector.Size());
				Assert::AreEqual(std::size_t{ 10 }, failedRemoveVector.Capacity());
			}

			{
				Vector<Foo> valueRemoveVector{ vector };
				valueRemoveVector.PushBack(additionalFoo);
				valueRemoveVector.Remove(additionalFoo);
				Assert::AreEqual(std::size_t{ 6 }, valueRemoveVector.Size());
				Assert::AreEqual(std::size_t{ 10 }, valueRemoveVector.Capacity());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			Vector<Foo> vector;
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);
			vector.PushBack(expectedFoo);

			Vector<Foo> movedVector{ std::move(Vector<Foo>{} )};
#pragma warning(push)
#pragma warning( disable : 26800)
			Vector<Foo> newVector{std::move(vector)};
			Assert::AreEqual(std::size_t{ 0 }, vector.Size());
			Assert::AreEqual(std::size_t{ 0 }, vector.Capacity());
			Assert::AreEqual(std::size_t{ 5 }, newVector.Size());
			Assert::AreEqual(std::size_t{ 5 }, newVector.Capacity());
			Vector<Foo> assignedVector{};
			assignedVector = std::move(newVector);

			Assert::AreEqual(std::size_t{ 0 }, vector.Size());
			Assert::AreEqual(std::size_t{ 0 }, vector.Capacity());
			Assert::AreEqual(std::size_t{ 0 }, newVector.Size());
			Assert::AreEqual(std::size_t{ 0 }, newVector.Capacity());
			Assert::AreEqual(std::size_t{ 5 }, assignedVector.Size());
			Assert::AreEqual(std::size_t{ 5 }, assignedVector.Capacity());
#pragma warning(pop)
		}

		TEST_METHOD(Arithmetic)
		{
			{
				Vector<Foo> vector{ expectedFoo, additionalFoo, additionalFoo, additionalFoo, additionalFoo, additionalFoo, expectedFoo };

				auto iterator = vector.begin();
				auto newIterator = iterator + std::size_t{ 3 };
				Assert::AreEqual(additionalFoo, *newIterator);
				newIterator -= std::size_t{ 3 };
				Assert::AreEqual(expectedFoo, *newIterator);
				newIterator += std::size_t{ 3 };
				Assert::AreEqual(additionalFoo, *newIterator);
				newIterator = newIterator - std::size_t{ 3 };
				Assert::AreEqual(expectedFoo, *newIterator);
				auto otherIterator{ newIterator };
				++otherIterator;
				Assert::AreEqual(std::size_t{ 1 }, otherIterator - newIterator);
#ifdef USING_EXCEPTIONS
				iterator += 20;
				Assert::ExpectException<std::runtime_error>([&iterator] {*iterator; });
#endif
			}

			{
				const Vector<Foo> vector{ expectedFoo, additionalFoo, additionalFoo, additionalFoo, additionalFoo, additionalFoo, expectedFoo };

				auto iterator = vector.begin();
				auto newIterator = iterator + std::size_t{ 3 };
				Assert::AreEqual(additionalFoo, *newIterator);
				newIterator -= std::size_t{ 3 };
				Assert::AreEqual(expectedFoo, *newIterator);
				newIterator += std::size_t{ 3 };
				Assert::AreEqual(additionalFoo, *newIterator);
				newIterator = newIterator - std::size_t{ 3 };
				Assert::AreEqual(expectedFoo, *newIterator);
				auto otherIterator{ newIterator };
				++otherIterator;
				Assert::AreEqual(std::size_t{ 1 }, otherIterator - newIterator);
#ifdef USING_EXCEPTIONS
				iterator += 20;
				Assert::ExpectException<std::runtime_error>([&iterator] {*iterator; });
#endif
			}
#ifdef USING_EXCEPTIONS
			{
				Vector<Foo>::Iterator iterator{};
				Vector<Foo>::Iterator otherIterator{};
				Assert::ExpectException<std::runtime_error>([&iterator] {iterator + 1; });
				Assert::ExpectException<std::runtime_error>([&iterator] {iterator - 1; });
				Assert::ExpectException<std::runtime_error>([&iterator, &otherIterator] {iterator - otherIterator; });
			}

			{
				Vector<Foo>::ConstIterator iterator{};
				Vector<Foo>::ConstIterator otherIterator{};
				Assert::ExpectException<std::runtime_error>([&iterator] {iterator + 1; });
				Assert::ExpectException<std::runtime_error>([&iterator] {iterator - 1; });
				Assert::ExpectException<std::runtime_error>([&iterator, &otherIterator] {iterator - otherIterator; });
			}

			{
				Vector<Foo> vector{expectedFoo, expectedFoo};
				Vector<Foo> otherVector{expectedFoo, expectedFoo};

				Vector<Foo>::Iterator iterator{ vector };
				Vector<Foo>::Iterator otherIterator{ otherVector };
				Assert::ExpectException<std::runtime_error>([&iterator, &otherIterator] {iterator - otherIterator; });
			}
#endif

			{
				Vector<Foo> vector{ expectedFoo };
				Assert::AreEqual(std::size_t{ 0 }, vector.begin() - vector.end());
				Assert::AreEqual(std::size_t{ 0 }, vector.cbegin() - vector.cend());
			}

			{
				Vector<Foo> vector{ expectedFoo };
				auto iterator{ vector.begin() };
				++iterator;
				++iterator;
				++iterator;
				++iterator;
				Assert::AreEqual(vector.end(), iterator);
			}

			{
				Vector<Foo> vector{ expectedFoo };
				auto iterator{ vector.begin() };
				--iterator;
				--iterator;
				--iterator;
				--iterator;
				Assert::AreEqual(vector.begin(), iterator);
			}

			{
				Vector<Foo> vector{ expectedFoo };
				auto iterator{ vector.cbegin() };
				++iterator;
				++iterator;
				++iterator;
				++iterator;
				Assert::AreEqual(vector.cend(), iterator);
			}

			{
				Vector<Foo> vector{ expectedFoo };
				auto iterator{ vector.cbegin() };
				--iterator;
				--iterator;
				--iterator;
				--iterator;
				Assert::AreEqual(vector.cbegin(), iterator);
			}
		}

		TEST_METHOD(Dereference)
		{
			Vector<Foo> vector;
			vector.PushBack(expectedFoo);
			Assert::AreEqual(expectedFoo, *vector.begin());
			Assert::AreEqual(expectedFoo, *vector.cbegin());
#ifdef USING_EXCEPTIONS
			Assert::ExpectException<std::runtime_error>([&vector] {*vector.end(); });
			Assert::ExpectException<std::runtime_error>([&vector] {*vector.cend(); });
#endif
		}

		TEST_METHOD(EmplaceBack)
		{
			Vector<Foo> vector;
			vector.EmplaceBack(expectedFoo);
			vector.EmplaceBack(1);
			vector.EmplaceBack(0);
			vector.EmplaceBack(0);
			vector.EmplaceBack(0);
			vector.EmplaceBack(0);

			Assert::AreEqual(expectedFoo, vector.Front());
			Assert::AreEqual(Foo{ 1 }, vector[1]);
			Assert::AreEqual(Foo{}, vector.Back());
		}

		TEST_METHOD(IncrementFunctor)
		{
			Vector<Foo> vector{ Foo{0}, Foo{0}, Foo{0} };
			auto originalFunctor = vector.GetIncrementStrategy();
			vector.SetIncrementStrategy(UserIncrement<Foo>{});
			vector.EmplaceBack();
			Assert::AreEqual(std::size_t{ 13 }, vector.Capacity());
			vector.SetIncrementStrategy(originalFunctor);
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			vector.EmplaceBack();
			Assert::AreEqual(std::size_t{ 18 }, vector.Capacity());
		}
	private:
		inline static _CrtMemState _startMemState;

	};
}
