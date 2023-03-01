#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace LibraryDesktopTests
{
	TEST_CLASS(SListTests)
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

		const Foo expectedFoo{7};
		const Foo additionalFoo{8}; 
		
		TEST_METHOD(Constructor)
		{
			{
				SList<Foo> list;
				Assert::AreEqual(std::size_t{ 0 }, list.Size());
#ifdef _DEBUG
				Assert::ExpectException<std::runtime_error>([&list] {auto item = list.Front(); item; });
				Assert::ExpectException<std::runtime_error>([&list] {auto item = list.Back(); item; });
#endif
			}

			{
				SList<Foo> list{ expectedFoo, expectedFoo, expectedFoo };
				Assert::AreEqual(std::size_t{ 3 }, list.Size());

				for (auto& item : list)
				{
					Assert::AreEqual(expectedFoo, item);
				}
			}
		}
		TEST_METHOD(PushFront)
		{
			SList<Foo> list;
			list.PushFront(additionalFoo);
			list.PushFront(std::move(Foo{expectedFoo}));
			list.PushFront(expectedFoo);
			Assert::AreEqual(std::size_t{ 3 }, list.Size());

			Assert::AreEqual(additionalFoo, list.Back());
			Assert::AreEqual(expectedFoo, list.At(1));
			Assert::AreEqual(expectedFoo, list.Front());
		}

		TEST_METHOD(PushBack)
		{
			SList<Foo> list;
			list.PushBack(additionalFoo);
			list.PushBack(std::move(Foo{expectedFoo}));
			list.PushBack(expectedFoo);
			Assert::AreEqual(std::size_t{ 3 }, list.Size());
			Assert::AreEqual(additionalFoo, list.Front());
			Assert::AreEqual(expectedFoo, list.At(1));
			Assert::AreEqual(expectedFoo, list.Back());
		}
		
		TEST_METHOD(EmplaceBack)
		{
			SList<Foo> list;
			list.EmplaceBack(0);
			Assert::AreEqual(Foo{ 0 }, list.Front());

			list.PushBack(std::move(Foo{}));

		}

		TEST_METHOD(MoveSemantics)
		{
			{
				SList<Foo> list;
				list.EmplaceBack(expectedFoo);
				list.EmplaceBack(expectedFoo);
				list.EmplaceBack(expectedFoo);

				SList<Foo> movedList{ std::move(list) };
				Assert::AreEqual(std::size_t{ 3 }, movedList.Size());
				for (auto& item : movedList)
				{
					Assert::AreEqual(expectedFoo, item);
				}
			}

			{
				SList<Foo> list;
				list.EmplaceBack(expectedFoo);
				list.EmplaceBack(expectedFoo);
				list.EmplaceBack(expectedFoo);

				SList<Foo> movedList;
				movedList = std::move(list);
				Assert::AreEqual(std::size_t{ 3 }, movedList.Size());
				for (auto& item : movedList)
				{
					Assert::AreEqual(expectedFoo, item);
				}
			}
		}
		TEST_METHOD(Front)
		{
			SList<Foo> list;
#ifdef _DEBUG
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = list.Front(); item; });
#endif

			list.PushFront(expectedFoo);
			Assert::AreEqual(expectedFoo, list.Front());
			Assert::AreEqual(expectedFoo, list.Back());

			list.PushFront(additionalFoo);
			Assert::AreEqual(additionalFoo, list.Front());
			Assert::AreEqual(expectedFoo, list.Back());
		}

		TEST_METHOD(Back)
		{
			SList<Foo> list;
#ifdef _DEBUG
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = list.Back(); item; });
#endif
			list.PushBack(expectedFoo);
			Assert::AreEqual(expectedFoo, list.Back());
			Assert::AreEqual(expectedFoo, list.Front());

			list.PushBack(additionalFoo);
			Assert::AreEqual(expectedFoo, list.Front());
			Assert::AreEqual(additionalFoo, list.Back());
		}

		TEST_METHOD(IsEmpty)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			list.PushBack(expectedFoo);
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);
			list.PopFront();
			Assert::AreEqual(std::size_t{ 1 }, list.Size());
			Assert::AreEqual(additionalFoo, list.Front());
			Assert::AreEqual(additionalFoo, list.Back());
			list.PopFront();
			Assert::AreEqual(std::size_t{ 0 }, list.Size());
			list.PopFront();
			Assert::AreEqual(std::size_t{ 0 }, list.Size());
		}

		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);
			list.PopBack();
			Assert::AreEqual(std::size_t{ 1 }, list.Size());
			Assert::AreEqual(expectedFoo, list.Front());
			Assert::AreEqual(expectedFoo, list.Back());
			list.PopBack();
			Assert::AreEqual(std::size_t{ 0 }, list.Size());
			list.PopBack();
			Assert::AreEqual(std::size_t{ 0 }, list.Size());
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.Clear();
			Assert::AreEqual(std::size_t{ 0 }, list.Size());

#ifdef _DEBUG
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = list.Front(); item; });
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = list.Back(); item; });
#endif
		}

		TEST_METHOD(At)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);
			list.PushBack(expectedFoo);
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);
			Assert::AreEqual(additionalFoo, list.At(5));
			Assert::ExpectException<std::runtime_error>([&list]() {auto& element = list.At(6); element; });
			list.Clear();
			Assert::ExpectException<std::runtime_error>([&list]() {auto& element = list.At(0); element; });
		}

		TEST_METHOD(CopyConstructor)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);

			SList<Foo> copiedList{ list };
			Assert::AreEqual(expectedFoo, copiedList.Front());
			Assert::AreEqual(expectedFoo, copiedList.At(1));
			Assert::AreEqual(additionalFoo, copiedList.Back());
			Assert::AreEqual(std::size_t{ 3 }, list.Size());

			Assert::IsTrue(std::equal(list.begin(), list.end(), copiedList.begin()));
		}

		TEST_METHOD(CopyAssignment)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);

			SList<Foo> copiedList;

			copiedList = list;
			Assert::AreEqual(copiedList.Front(), list.Front());
			Assert::AreEqual(copiedList.At(1), list.At(1));
			Assert::AreEqual(copiedList.Back(), list.Back());

			SList<Foo> anotherList;
			anotherList.PushBack(expectedFoo);
			anotherList.PushBack(expectedFoo);
			anotherList.PushBack(additionalFoo);

			Assert::AreEqual(anotherList.Front(), list.Front());
			Assert::AreEqual(anotherList.At(1), list.At(1));
			Assert::AreEqual(anotherList.Back(), list.Back());

			Assert::IsTrue(std::equal(list.begin(), list.end(), anotherList.begin()));
			Assert::IsTrue(std::equal(list.begin(), list.end(), anotherList.begin()));
			Assert::IsTrue(std::equal(list.begin(), list.end(), copiedList.begin()));
			Assert::IsTrue(std::equal(copiedList.begin(), copiedList.end(), anotherList.begin()));

		}

		TEST_METHOD(begin_and_end)
		{
			SList<Foo> list;
			Assert::AreEqual(list.begin(), list.end());
			Assert::AreEqual(list.cbegin(), list.cend());
			list.PushBack(expectedFoo);
			Assert::AreNotEqual(list.begin(), list.end());
			Assert::AreNotEqual(list.cbegin(), list.cend());

			list.PushBack(additionalFoo);
			Assert::AreEqual(expectedFoo, *list.begin());
			Assert::AreEqual(expectedFoo, *list.cbegin());
			Assert::AreNotEqual(list.begin(), list.end());
			Assert::AreNotEqual(list.cbegin(), list.cend());

#ifdef _DEBUG
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = *list.end(); item; });
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = *list.cend(); item; });
#endif
		}

		TEST_METHOD(IteratorSameness)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);

			SList<Foo> otherList;
			otherList.PushBack(expectedFoo);
			otherList.PushBack(additionalFoo);

			const SList<Foo> constList{otherList};

			Assert::AreNotEqual(list.begin(), otherList.begin());
			Assert::AreNotEqual(list.cbegin(), constList.cbegin());
			Assert::AreNotEqual(list.cbegin(), constList.cbegin());

			Assert::AreNotEqual(list.begin(),otherList.begin());
			Assert::AreNotEqual(list.cbegin(), constList.begin());
			Assert::AreNotEqual(list.cbegin(), constList.cbegin());

			Assert::AreNotEqual(list.end(),otherList.end());
			Assert::AreNotEqual(list.cend(), constList.end());
			Assert::AreNotEqual(list.cend(), constList.cend());
			Assert::AreNotEqual(list.end(), otherList.end());
			Assert::AreNotEqual(list.cend(), constList.end());
			Assert::AreNotEqual(list.cend(), constList.cend());

			const SList<Foo>::Iterator a{list, nullptr};
			const SList<Foo>::Iterator b{list, nullptr};

			Assert::AreEqual(a,b);

			const SList<Foo>::ConstIterator c{ constList, nullptr };
			const SList<Foo>::ConstIterator d{ constList, nullptr };

			Assert::AreEqual(c,d);

			const SList<Foo>::ConstIterator e{ list, nullptr };
			const SList<Foo>::ConstIterator f{ a };

			Assert::AreEqual(e,f);
		}

		TEST_METHOD(Dereference)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);

			SList<Foo> otherList;
			otherList.PushBack(expectedFoo);
			otherList.PushBack(additionalFoo);

			Assert::AreEqual(*list.cbegin(), *otherList.cbegin());
			Assert::AreEqual(*list.begin(), *otherList.begin());

			const SList<Foo> constList{ otherList };

			Assert::AreEqual(*list.cbegin(), *constList.cbegin());
			Assert::AreEqual(*list.begin(), *constList.begin());

#ifdef _DEBUG
			Assert::ExpectException<std::runtime_error>([&list]() {auto item = *list.end(); item; });
			Assert::ExpectException<std::runtime_error>([&constList]() {auto item = *constList.cend(); item; });
#endif

		}

		TEST_METHOD(Increment)
		{
			SList<Foo> list;
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.PushBack(expectedFoo);
			{
				auto begin = list.begin();
				++begin;
				begin++;
				Assert::AreEqual(expectedFoo, *begin);
			}

			const SList<Foo> constList{ list };
			{
				auto begin = constList.begin();
				++begin;
				begin++;
				Assert::AreEqual(expectedFoo, *begin);
			}

			{
				auto begin = constList.cbegin();
				++begin;
				begin++;
				Assert::AreEqual(expectedFoo, *begin);
			}

		}

		TEST_METHOD(Find)
		{
			SList<Foo> list;
			list.PushBack(expectedFoo);
			list.PushBack(additionalFoo);
			list.PushBack(expectedFoo);

			SList<Foo>::Iterator iterator{ list.begin() };
			++iterator;
			Assert::AreEqual(list.Find(additionalFoo), iterator);

			const SList<Foo> constList{ list };
			SList<Foo>::ConstIterator constIterator{ constList.begin() };
			++constIterator;

			Assert::AreEqual(constList.Find(additionalFoo), constIterator);
		}

		TEST_METHOD(Remove)
		{
			{
				SList<Foo> list;
				list.PushBack(expectedFoo);
				list.PushBack(additionalFoo);
				list.PushBack(expectedFoo);

				list.Remove(additionalFoo);

				Assert::AreEqual(std::size_t{ 2 }, list.Size());

				list.Remove(expectedFoo);
				Assert::AreEqual(std::size_t{ 1 }, list.Size());

				list.Remove(expectedFoo);
				Assert::AreEqual(std::size_t{ 0 }, list.Size());

				list.Remove(expectedFoo);
				Assert::AreEqual(std::size_t{ 0 }, list.Size());

				SList<Foo> otherList;
				otherList.PushBack(additionalFoo);
				otherList.PushBack(expectedFoo);

				otherList.Remove(expectedFoo);
				Assert::AreEqual(std::size_t{ 1 }, otherList.Size());
			}

			{
				SList<Foo> list;
				list.PushBack(expectedFoo);
				list.PushBack(additionalFoo);
				list.PushBack(expectedFoo);
				auto iterator = list.begin();
				list.Remove(iterator);
			}

			{
				SList<Foo> list;
				list.PushBack(expectedFoo);
				list.PushBack(additionalFoo);
				list.PushBack(expectedFoo);
				auto iterator = list.begin();
				++iterator;
				list.Remove(iterator);
			}

			{
				SList<Foo> list;
				list.PushBack(expectedFoo);
				list.PushBack(additionalFoo);
				list.PushBack(expectedFoo);
				auto iterator = list.begin();
				++iterator;
				++iterator;
				list.Remove(iterator);
			}
		}

		TEST_METHOD(InsertAfter)
		{
			SList<Foo> list;
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);

			list.InsertAfter(expectedFoo, list.begin());
			Assert::AreEqual(std::size_t{ 4 }, list.Size());
			Assert::AreEqual(expectedFoo, list.At(std::size_t{ 1 }));

			auto iterator = list.begin();
			++iterator;
			++iterator;
			++iterator;

			auto returnedIterator = list.InsertAfter(expectedFoo, iterator);
			Assert::AreEqual(std::size_t{ 5 }, list.Size());
			Assert::AreEqual(expectedFoo, list.At(std::size_t{ 4 }));
			Assert::AreEqual(expectedFoo, *returnedIterator);

			returnedIterator = list.InsertAfter(additionalFoo, returnedIterator);
			Assert::AreEqual(std::size_t{ 6 }, list.Size());
			Assert::AreEqual(additionalFoo, list.At(std::size_t{ 5 }));
			Assert::AreEqual(additionalFoo, *returnedIterator);

#ifdef _DEBUG
			SList<Foo> otherList;
			SList<Foo>::Iterator otherIterator = otherList.begin();
			Assert::ExpectException<std::runtime_error>([&list, otherIterator] { auto iterator = list.InsertAfter(Foo{ 7 }, otherIterator); });
#endif
		}

		TEST_METHOD(ForLoop)
		{
			SList<Foo> list;
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			list.PushBack(additionalFoo);
			{
				int numberOfElements{};
				for (auto iterator = list.begin(); iterator != list.end(); iterator++)
				{
					++numberOfElements;
					Assert::AreEqual(additionalFoo, *iterator);
				}
				Assert::AreEqual(3, numberOfElements);
			}

			{
				int numberOfElements{};
				for (auto iterator = list.begin(); iterator != list.end(); ++iterator)
				{
					++numberOfElements;
					Assert::AreEqual(additionalFoo, *iterator);
				}
				Assert::AreEqual(3, numberOfElements);
			}

			{
				int numberOfElements{};
				for (auto& foo : list)
				{
					++numberOfElements;
					Assert::AreEqual(additionalFoo, foo);
				}
				Assert::AreEqual(3, numberOfElements);
			}

			const SList<Foo> constList{ list };

			{
				int numberOfElements{};
				for (auto iterator = constList.begin(); iterator != constList.end(); iterator++)
				{
					++numberOfElements;
					Assert::AreEqual(additionalFoo, *iterator);
				}
				Assert::AreEqual(3, numberOfElements);
			}

			{
				int numberOfElements{};
				for (auto iterator = constList.begin(); iterator != constList.end(); ++iterator)
				{
					++numberOfElements;
					Assert::AreEqual(additionalFoo, *iterator);
				}
				Assert::AreEqual(3, numberOfElements);
			}

			{
				int numberOfElements{};
				for (auto& foo : constList)
				{
					++numberOfElements;
					Assert::AreEqual(additionalFoo, foo);
				}
				Assert::AreEqual(3, numberOfElements);
			}
		}

		private:
			inline static _CrtMemState _startMemState;
	};
}
