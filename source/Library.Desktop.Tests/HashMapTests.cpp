#include "pch.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include <gsl/gsl>
#include "HashMap.h"
#include "Foo.h"
#include "Bar.h"
#include "DefaultHash.h"
#include "UserSuppliedHash.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

bool BarEquality(const Bar& lhs, const Bar& rhs)
{
	return lhs.Data() == rhs.Data();
}

namespace LibraryDesktopTests
{
	TEST_CLASS(HashMapTests)
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
				HashMap<int, Foo> map{};
				map.Insert(std::pair<const int, Foo>(7, Foo{}));
				Assert::AreEqual(Foo{},map.At(7));
				Assert::IsTrue(map.ContainsKey(7));
				map.Clear();
				Assert::IsFalse(map.ContainsKey(7));
				map.Insert(std::pair<const int, Foo>(7, Foo{}));
				Assert::IsTrue(map.ContainsKey(7));
			}

			{
				HashMap<int, int> test{ 2000 };
			}

			{
				HashMap<int, Foo> map{};
				map.Insert(std::pair(0, Foo{1}));
				map.Insert(std::pair(1, Foo{2}));
				map.Insert(std::pair(6, Foo{3}));
				map.Insert(std::pair(60, Foo{4}));
				map.Insert(std::pair(70, Foo{5}));

				{
					const HashMap<int, Foo> constMap{ map };
				}

				{
					const HashMap<int, Foo> constMap = map;
					auto iterator{ constMap.begin() };
					Assert::AreEqual(Foo{1}, (*iterator).second);
				}
				{
					HashMap<Foo, int> fooMap{ UserHash<Foo>{} };
					fooMap.Insert(std::pair(Foo{ 1 }, 1));
					fooMap.Insert(std::pair(Foo{ 2 }, 2));
					fooMap.Insert(std::pair(Foo{ 3 }, 3));
					fooMap.Insert(std::pair(Foo{ 4 }, 4));
					fooMap.Insert(std::pair(Foo{ 5 }, 5));

					auto iterator{ fooMap.begin() };
					Assert::AreEqual(1, (*iterator).second);

				}


				{
					const HashMap<int, Foo> constMap{ std::move(map) };
					auto iterator{ constMap.begin() };
					Assert::AreEqual(Foo{1}, (*iterator).second);
				}

				{
					HashMap<Bar, Foo> barMap{ BarEquality, UserHash<Bar>{} };
					barMap.Insert(std::pair(Bar{}, Foo{}));
					barMap.Insert(std::pair(Bar{1}, Foo{1}));
					barMap.Insert(std::pair(Bar{2}, Foo{2}));
					barMap.Insert(std::pair(Bar{3}, Foo{3}));


					Assert::AreEqual(Foo{}, (*barMap.Find(Bar{})).second);
					Assert::AreEqual(Foo{ 1 }, (*barMap.Find(Bar{ 1 })).second);
					Assert::AreEqual(Foo{ 2 }, (*barMap.Find(Bar{ 2 })).second);
					Assert::AreEqual(Foo{ 3 }, (*barMap.Find(Bar{ 3 })).second);

					HashMap<Bar, Foo> barMapSized{ 27, BarEquality, UserHash<Bar>{} };
					barMapSized.Insert(std::pair(Bar{}, Foo{}));
					barMapSized.Insert(std::pair(Bar{ 1 }, Foo{ 1 }));
					barMapSized.Insert(std::pair(Bar{ 2 }, Foo{ 2 }));
					barMapSized.Insert(std::pair(Bar{ 3 }, Foo{ 3 }));

					Assert::AreEqual(Foo{}, (*barMapSized.Find(Bar{})).second);
					Assert::AreEqual(Foo{ 1 }, (*barMapSized.Find(Bar{ 1 })).second);
					Assert::AreEqual(Foo{ 2 }, (*barMapSized.Find(Bar{ 2 })).second);
					Assert::AreEqual(Foo{ 3 }, (*barMapSized.Find(Bar{ 3 })).second);

					Assert::IsTrue(barMap.GetLoadFactor() > barMapSized.GetLoadFactor());
				}


			}
		}

		TEST_METHOD(InitializerListConstruction)
		{
			HashMap<std::string, int> map{ {"String1", 1}, {"String2", 2}, {"String3", 3}, {"String4", 4}, {"String5", 5} };

			Assert::AreEqual(1, map.At("String1"));
			Assert::AreEqual(2, map.At("String2"));
			Assert::AreEqual(3, map.At("String3"));
			Assert::AreEqual(4, map.At("String4"));
			Assert::AreEqual(5, map.At("String5"));
		}

		TEST_METHOD(Hash)
		{
			{
				const int a{ 7 };
				auto hashFunctor = DefaultHash<int>{};
				Assert::AreEqual(std::size_t{ 7 }, hashFunctor(a));
			}

			{
				const char* string{ "MyString" };
				auto hashFunctor = DefaultHash<char>{};
				Assert::AreEqual(std::size_t{ 77 + 121 + 83 + 116 + 114 + 105 + 110 + 103 }, hashFunctor(string));
			}

			{
				const std::string string {"MyString"};
				auto hashFunctor = DefaultHash<std::string>{};
				Assert::AreEqual(std::size_t{ 77 + 121 + 83 + 116 + 114 + 105 + 110 + 103 }, hashFunctor(string));
			}

			{
				const Foo foo{};
				const Foo otherFoo{7};
				auto hashFunctor = UserHash<Foo>{};
				Assert::AreEqual(std::size_t{ 0 }, hashFunctor(foo));
				Assert::AreEqual(std::size_t{ 7 }, hashFunctor(otherFoo));
			}
		}

		TEST_METHOD(Iterators)
		{
			{
				HashMap<int, Foo> map{10};
				map.Insert(std::pair(0, Foo{}));
				map.Insert(std::pair(0, Foo{}));
				map.Insert(std::pair(0, Foo{}));
				map.Insert(std::pair(1, Foo{}));
				map.Insert(std::pair(6, Foo{}));
				map.Insert(std::pair(60, Foo{ 1 }));
				map.Insert(std::pair(60, Foo{ 1 }));
				map.Insert(std::pair(70, Foo{ 1 }));
				auto start = map.begin();
				Assert::AreEqual(0, (*start).first);
				Assert::AreEqual(Foo{}, (*start).second);
				++start;
				Assert::AreEqual(60, (*start).first);
				Assert::AreEqual(Foo{ 1 }, (*start).second);
				start++;
				Assert::AreEqual(70, (*start).first);
				Assert::AreEqual(Foo{ 1 }, (*start).second);
				++start;
				Assert::AreEqual(1, (*start).first);
				Assert::AreEqual(Foo{}, (*start).second);
				++start;
				Assert::AreEqual(6, (*start).first);
				Assert::AreEqual(Foo{}, (*start).second);

				Assert::IsTrue(map.Find(6) == start);

				++start;
				Assert::IsTrue(start == map.end());

				auto begin = map.begin();
				std::size_t size{};
				for (begin; begin != map.end(); ++begin)
				{
					++size;
				}

				Assert::AreEqual(std::size_t{ 5 }, size);

				map.Remove(6);

				begin = map.begin();
				std::size_t newSize{};
				for (begin; begin != map.end(); ++begin)
				{
					++newSize;
				}

				Assert::AreEqual(std::size_t{ 4 }, newSize);

				map.Remove(88);

				Assert::IsTrue(map[0] == map.At(0));

				map[88];

				newSize = 0;
				begin = map.begin();
				Assert::AreEqual(0, begin->first);
				auto constIterator{ HashMap<int, Foo>::ConstIterator(begin) };
				Assert::AreEqual(0, constIterator->first);
				for (begin; begin != map.end(); ++begin)
				{
					++newSize;
				}

				Assert::AreEqual(std::size_t{ 5 }, newSize);
				Assert::AreEqual(std::size_t{ 5 }, map.Size());
			}

			{
				HashMap<int, Foo> map{10};
				map.Insert(std::pair(1, Foo{}));
				map.Insert(std::pair(6, Foo{}));
				map.Insert(std::pair(61, Foo{ 1 }));
				map.Insert(std::pair(62, Foo{ 1 }));
				map.Insert(std::pair(73, Foo{ 1 }));

				const HashMap<int, Foo> constMap{ map };

				for (auto& item : constMap)
				{
					Assert::IsTrue(item.second == Foo{ 1 } || item.second == Foo{});
				}

				auto iterator{ constMap.begin() };
				++iterator;
				iterator++;

				Assert::AreEqual((*iterator).first, 62);
				Assert::AreEqual((*iterator).second, Foo{ 1 });
			}
		}

		TEST_METHOD(Insert)
		{
			HashMap<Bar, Foo>::PairType movedItem{ Bar{}, Foo{} };
			HashMap<Bar, Foo>::PairType item{ Bar{1}, Foo{1} };
			HashMap<Bar, Foo>::PairType otherItem{ Bar{10}, Foo{1} };

			HashMap<Bar, Foo> barMap{ 10, BarEquality, UserHash<Bar>{} };
			barMap.Insert(std::move(movedItem));
			barMap.Insert(item);
			barMap.Insert(item);
			barMap.Insert(otherItem);
			barMap.Emplace(Bar{ 7 }, Foo{ 6 });

			Assert::AreEqual(Foo{}, barMap.At(Bar{}));
			Assert::AreEqual(Foo{1}, barMap.At(Bar{1}));
			Assert::AreEqual(std::size_t{ 4 }, barMap.Size());
		}

		TEST_METHOD(Find)
		{
			HashMap<int, Foo> map{10};
			map.Insert(std::pair(7, Foo{}));
			map.Insert(std::pair(0, Foo{}));
			map.Insert(std::pair(10, Foo{}));

			Assert::AreEqual(true,map.begin() == map.Find(0));
			Assert::AreEqual(true,map.begin() != map.Find(8));
			Assert::AreEqual(true, map.end() == map.Find(8));


			const HashMap<int, Foo> constMap{ map };
			Assert::AreEqual(true,constMap.begin() == constMap.Find(0));
			Assert::AreEqual(true,constMap.cbegin() == constMap.Find(0));
			Assert::AreEqual(true,constMap.begin() != constMap.Find(8));
			Assert::AreEqual(true,constMap.end() == constMap.Find(8));
			Assert::AreEqual(true,constMap.cbegin() != constMap.Find(8));
			Assert::AreEqual(true,constMap.cend() == constMap.Find(8));
			Assert::AreEqual(true,constMap.cend() == constMap.Find(20));

			Assert::AreEqual((*constMap.Find(10)).second, (*constMap.Find(0)).second);
		}

		TEST_METHOD(Rehash)
		{
			HashMap<int, Foo> map{};
			map.Insert(std::pair(0, Foo{}));
			map.Insert(std::pair(1, Foo{}));
			map.Insert(std::pair(6, Foo{}));
			map.Insert(std::pair(60, Foo{}));
			map.Insert(std::pair(70, Foo{}));

			map.Rehash(2000);
			Assert::AreEqual(std::size_t{ 5 }, map.Size());
			map.Rehash(2);
			Assert::AreEqual(std::size_t{ 5 }, map.Size());
		}

		TEST_METHOD(At)
		{
			HashMap<int, Foo> map{};
			map.Insert(std::pair(0, Foo{1}));
			map.Insert(std::pair(1, Foo{2}));
			map.Insert(std::pair(6, Foo{3}));
			map.Insert(std::pair(60, Foo{4}));
			map.Insert(std::pair(70, Foo{5}));

			Assert::AreEqual(Foo{ 1 }, map.At(0));
			Assert::AreEqual(Foo{ 2 }, map.At(1));
			Assert::AreEqual(Foo{ 3 }, map.At(6));
			Assert::AreEqual(Foo{ 4 }, map.At(60));
			Assert::AreEqual(Foo{ 5 }, map.At(70));

			const HashMap<int, Foo> constMap{ map };

			Assert::AreEqual(Foo{ 1 }, constMap.At(0));
			Assert::AreEqual(Foo{ 2 }, constMap.At(1));
			Assert::AreEqual(Foo{ 3 }, constMap.At(6));
			Assert::AreEqual(Foo{ 4 }, constMap.At(60));
			Assert::AreEqual(Foo{ 5 }, constMap.At(70));
		}

		TEST_METHOD(BracketOperator)
		{
			HashMap<int, Foo> map{};
			map.Insert(std::pair(0, Foo{ 1 }));
			map.Insert(std::pair(1, Foo{ 2 }));
			map.Insert(std::pair(6, Foo{ 3 }));
			map.Insert(std::pair(60, Foo{ 4 }));
			map.Insert(std::pair(2, Foo{ 4 }));
			map.Insert(std::pair(3, Foo{ 5 }));
			map.Insert(std::pair(4, Foo{ 5 }));
			map[77];
			Assert::AreEqual(std::size_t{ 8 }, map.Size());
			Assert::IsTrue(map.GetLoadFactor() > 0.7f);
		}

		TEST_METHOD(LoadFactor)
		{
			HashMap<int, Foo> map{10};
			map.Insert(std::pair(0, Foo{}));
			map.Insert(std::pair(1, Foo{}));
			map.Insert(std::pair(2, Foo{}));
			map.Insert(std::pair(3, Foo{}));
			map.Insert(std::pair(4, Foo{}));
			map.Insert(std::pair(6, Foo{}));
			map.Insert(std::pair(60, Foo{}));
			map.Insert(std::pair(70, Foo{}));

			Assert::IsTrue(map.GetLoadFactor() > 0.75f);
		}

		TEST_METHOD(StringKey)
		{
			HashMap<std::string, int> map{};
			map.Insert(std::pair("one", 1));
			map.Insert(std::pair("two", 2));
			map.Insert(std::pair("three", 3));

			Assert::AreEqual(1, map.At("one"));
			Assert::AreEqual(2, map.At("two"));
			Assert::AreEqual(3, map.At("three"));
		}

		TEST_METHOD(CStringKey)
		{
			HashMap<const char*, int> map{};
			map.Insert(std::pair("one", 1));
			map.Insert(std::pair("two", 2));
			map.Insert(std::pair("three", 3));

			Assert::AreEqual(1, map.At("one"));
			Assert::AreEqual(2, map.At("two"));
			Assert::AreEqual(3, map.At("three"));
		}

		TEST_METHOD(FooKey)
		{
			HashMap<Foo, int> map{ UserHash<Foo>{} };
			map.Insert(std::pair(Foo{ 0 }, 0));
			map.Insert(std::pair(Foo{ 1 }, 1));
			map.Insert(std::pair(Foo{ 2 }, 2));

			Assert::AreEqual(0, map.At(Foo{ 0 }));
			Assert::AreEqual(1, map.At(Foo{ 1 }));
			Assert::AreEqual(2, map.At(Foo{ 2 }));
		}

		TEST_METHOD(BarKey)
		{
			HashMap<Bar, int> map{BarEquality, UserHash<Bar>{} };
			map.Insert(std::pair(Bar{ 0 }, 0));
			map.Insert(std::pair(Bar{ 1 }, 1));
			map.Insert(std::pair(Bar{ 2 }, 2));

			Assert::AreEqual(0, map.At(Bar{ 0 }));
			Assert::AreEqual(1, map.At(Bar{ 1 }));
			Assert::AreEqual(2, map.At(Bar{ 2 }));
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}