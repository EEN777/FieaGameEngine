#pragma once

#include <cstddef>
#include <stdexcept>
#include <functional>
#include "DefaultHash.h"
#include "DefaultEquality.h"
#include "Vector.h"
#include "SList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// An associative container that utilizes a Vector of SLists of Key-Value Pairs.
	/// </summary>
	/// <typeparam name="TKey"></typeparam>
	/// <typeparam name="TData"></typeparam>
	template <typename TKey, typename TData>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using ChainType = SList<PairType>;
		using size_type = std::size_t;
	private:
		Vector<ChainType> _map{};
		std::function<bool(const TKey&, const TKey&)> equality{ DefaultEquality<TKey>{} };
		std::function<std::size_t(const TKey&)> hashMethod{ DefaultHash<TKey>{} };
		size_type _size{ 0 };
		size_type _chainCount{ 11 };
	public:

		/// <summary>
		/// An Iterator used to traverse all elements of a given HashMap. It is a Forward Iterator.
		/// </summary>
		struct Iterator final
		{
			friend HashMap;
			friend struct ConstIterator;
		private:
			Vector<ChainType>::Iterator _chain{};
			ChainType::Iterator _chainPosition{};
			const HashMap* _owner{ nullptr };

		public:
			/// <summary>
			/// Creates a default Iterator that points to no item in the HashMap.
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Creates an Iterator that is owned by the passed HashMap and points to the first element of the first populated chain.
			/// </summary>
			/// <param name="HashMap"></param>
			Iterator(HashMap&);
			/// <summary>
			/// Increments the Iterator to the next valid item in the HashMap.
			/// </summary>
			/// <returns>Iterator</returns>
			Iterator& operator++();
			/// <summary>
			/// Increments the Iterator to the next valid item in the HashMap. Returns a copy of the HashMap before incrementing.
			/// </summary>
			/// <param name=""></param>
			/// <returns>Iterator</returns>
			Iterator operator++(int);
			/// <summary>
			/// Compares Iterators by comparing their ownerand where they are pointing to.
			/// </summary>
			/// <param name="other"></param>
			/// <returns>bool</returns>
			bool operator!=(const Iterator&) const;
			/// <summary>
			/// Compares Iterators by comparing their ownerand where they are pointing to.
			/// </summary>
			/// <param name="other"></param>
			/// <returns>bool</returns>
			bool operator==(const Iterator&) const;
			/// <summary>
			/// Returns a reference to the PairType pointed to by the Iterator.
			/// </summary>
			/// <returns>PairType</returns>
			PairType& operator*();
			/// <summary>
			/// Returns a reference to the PairType pointed to by the Iterator.
			/// </summary>
			/// <returns>PairType</returns>
			PairType* operator->();

		private:
			Iterator(HashMap&, Vector<ChainType>::Iterator);
			Iterator(HashMap&, Vector<ChainType>::Iterator, ChainType::Iterator);
		};

		/// <summary>
		/// An ConstIterator used to traverse all elements of a given HashMap. It is a Forward Iterator.
		/// </summary>
		struct ConstIterator final
		{
			friend HashMap;
		private:
			Vector<ChainType>::ConstIterator _chain{};
			ChainType::ConstIterator _chainPosition{};
			const HashMap* _owner{ nullptr };

		public:
			/// <summary>
			/// Creates a default ConstIterator that points to no item in the HashMap.
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Creates a ConstIterator that is owned by the passed HashMap and points to the first element of the first populated chain.
			/// </summary>
			/// <param name="HashMap"></param>
			ConstIterator(const HashMap&);
			/// <summary>
			/// Constructs a ConstIterator based off of the provided Iterator.
			/// </summary>
			/// <returns>Iterator</returns>
			ConstIterator(const Iterator&);
			/// <summary>
			/// Increments the ConstIterator to the next valid item in the HashMap.
			/// </summary>
			/// <returns>Iterator</returns>
			ConstIterator& operator++();
			/// <summary>
			/// Increments the ConstIterator to the next valid item in the HashMap. Returns a copy of the HashMap before incrementing.
			/// </summary>
			/// <param name="">other</param>
			/// <returns>Iterator</returns>
			ConstIterator operator++(int);
			/// <summary>
			/// Compares ConstIterators by comparing their ownerand where they are pointing to.
			/// </summary>
			/// <param name="other"></param>
			/// <returns>bool</returns>
			bool operator!=(const ConstIterator&) const;
			/// <summary>
			/// Compares ConstIterators by comparing their ownerand where they are pointing to.
			/// </summary>
			/// <param name="other"></param>
			/// <returns>bool</returns>
			bool operator==(const ConstIterator&) const;
			/// <summary>
			/// Returns a const reference to the PairType pointed to by the ConstIterator.
			/// </summary>
			/// <returns>const PairType</returns>
			const PairType& operator*();
			/// <summary>
			/// Returns a const reference to the PairType pointed to by the ConstIterator.
			/// </summary>
			/// <returns>const PairType</returns>
			const PairType* operator->();

		private:
			ConstIterator(const HashMap&, Vector<ChainType>::ConstIterator);
			ConstIterator(const HashMap&, Vector<ChainType>::ConstIterator, ChainType::ConstIterator);
		};

		/// <summary>
		/// A constructor for a HashMap that takes the Hash Functor to use when hashing keys.
		/// </summary>
		/// <param name="HashMethod"></param>
		explicit HashMap(std::function<std::size_t(const TKey&)> = DefaultHash<TKey>{});
		/// <summary>
		/// A constructor for a HashMap that takes the Hash Functor to use when hashing keys, and an equality method for comparing keys.
		/// </summary>
		/// <param name="EqualityMethod"></param>
		/// <param name="HashMethod"></param>
		explicit HashMap(std::function<bool(const TKey&, const TKey&)>, std::function<std::size_t(const TKey&)> = DefaultHash<TKey>{});
		/// <summary>
		/// A constructor for a HashMap that takes the Hash Functor to use when hashing keys, and the amount of chains to make.
		/// </summary>
		/// <param name="ChainCount"></param>
		/// <param name="HashMethod"></param>
		explicit HashMap(std::size_t, std::function<std::size_t(const TKey&)> = DefaultHash<TKey>{});
		/// <summary>
		/// A constructor for a HashMap that takes the Hash Functor to use when hashing keys, the amount of chains to make, and the equality method to use when comparing keys.
		/// </summary>
		/// <param name="ChainCount"></param>
		/// <param name="HashMethod"></param>
		explicit HashMap(std::size_t, std::function<bool(const TKey&, const TKey&)>, std::function<std::size_t(const TKey&)> = DefaultHash<TKey>{});
		HashMap(const HashMap&) = default;
		HashMap& operator=(const HashMap&) = default;
		HashMap(HashMap&&) noexcept = default;
		HashMap& operator=(HashMap&&) noexcept = default;
		~HashMap() = default;
		/// <summary>
		/// Attempts to insert the given pair within the hashmap, if the key has already been used then it returns an iterator pointing to that pair.
		/// </summary>
		/// <typeparam name="PairType reference"></typeparam>
		Iterator Insert(const PairType&);
		/// <summary>
		/// Attempts to insert the given pair within the hashmap, if the key has already been used then it returns an iterator pointing to that pair.
		/// </summary>
		/// <typeparam name="PairType rvalue"></typeparam>
		Iterator Insert(PairType&&);
		/// <summary>
		/// Takes values to construct a pair and does so, then moves it into the HashMap.
		/// </summary>
		/// <param name="Args"></param>
		template <typename...Args>
		Iterator Emplace(Args&&...);
		/// <summary>
		/// Retrieves the data at a given key.
		/// </summary>
		/// <param name="TKey">other</param>
		/// <returns>TData</returns>
		TData& At(const TKey&);
		/// <summary>
		/// Retrieves the data at a given key by const reference.
		/// </summary>
		/// <param name="TKey">other</param>
		/// <returns>TData</returns>
		const TData& At(const TKey&) const;
		/// <summary>
		/// Checks whether or not the HashMap already contains a given key.
		/// </summary>
		/// <param name="TKey"></param>
		/// <returns>TData</returns>
		bool ContainsKey(const TKey&) const;
		/// <summary>
		/// Clears all items out of the HashMap;
		/// </summary>
		void Clear();
		/// <summary>
		/// Removes the pair at the given Iterator.
		/// </summary>
		/// <param name="Iterator"></typeparam>
		void Remove(Iterator);
		/// <summary>
		/// Removes the pair associated with the given key.
		/// </summary>
		/// <param name="Iterator"></typeparam>
		void Remove(const TKey&);
		/// <summary>
		/// Returns the size of the HashMap.
		/// </summary>
		/// <returns>size</returns>
		std::size_t Size() const;
		/// <summary>
		/// Returns an Iterator pointing to the first item held in the HashMap.
		/// </summary>
		/// <returns>Iterator</returns>
		Iterator begin();
		/// <summary>
		/// Returns an Iterator pointing to the end of the HashMap.
		/// </summary>
		/// <returns>Iterator</returns>
		Iterator end();
		/// <summary>
		/// Returns an ConstIterator pointing to the first item held in the HashMap.
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an ConstIterator pointing to the end of the HashMap.
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an ConstIterator pointing to the first item held in the HashMap.
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// Returns an ConstIterator pointing to the end of the HashMap.
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator cend() const;
		/// <summary>
		/// Returns an Iterator that points to the pair associated with the given key, if not pair is found an iterator pointing to the end of the associated chain is returned.
		/// </summary>
		/// <returns>Iterator</returns>
		Iterator Find(const TKey&);
		/// <summary>
		/// Returns an ConstIterator that points to the pair associated with the given key, if not pair is found an iterator pointing to the end of the associated chain is returned.
		/// </summary>
		/// <returns>ConstIterator</returns>
		ConstIterator Find(const TKey&) const;
		/// <summary>
		/// Returns the a reference to the TData item of the associated pair and creates one if none are found.
		/// </summary>
		/// <returns>TData</returns>
		TData& operator[](const TKey&);
		/// <summary>
		/// Resizes the hashmap to have the desired number of chains.
		/// </summary>
		/// <param name="size"></param>
		void Rehash(std::size_t);
		/// <summary>
		/// Divides the size of the HashMap by the total number of chians to give a rough estimate of total usage. Not completely accurate.
		/// </summary>
		/// <returns>float</returns>
		float GetLoadFactor();

	private:
		bool ContainsKey(const TKey&, std::size_t) const;
		Iterator Find(const TKey&, std::size_t);
		ConstIterator Find(const TKey&, std::size_t) const;
	};
}

#include "HashMap.inl"