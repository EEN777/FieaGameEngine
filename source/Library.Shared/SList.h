#pragma once

#include <cstddef>
#include <stdexcept>


namespace FieaGameEngine
{
	/// <summary>
	/// A singly-linked list that stores items of type T.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class SList final
	{

	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = value_type&&;


	private:
		/// <summary>
		/// A data structure that holds both a given item of type T and a pointer to the nest item in the SList.
		/// </summary>
		struct Node final
		{
			value_type item;
			Node* next{nullptr};

			Node() = delete;
			explicit Node(const_reference, Node* = nullptr);
			explicit Node(rvalue_reference, Node* = nullptr);
		};


		Node* _front{ nullptr };
		Node* _back{ nullptr };
		size_t _size{ 0 };

	public:
		/// <summary>
		/// An iterator that points to a given item in an SList.
		/// </summary>
		struct Iterator final
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

			friend SList;
			friend struct ConstIterator;

		private:
			const SList* _owner { nullptr };
			Node* _currentNode{ nullptr };

		public:
			/// <summary>
			/// Constructs an SList::Iterator that points to no item and belongs to no SList.
			/// </summary>
			explicit Iterator() = default;
			/// <summary>
			/// Constructs an SList::Iterator that point to a given item and is owned by a given SList.
			/// </summary>
			/// <param name="const SList& list"> The list that will own the SList::Iterator </param>
			/// <param name="Node* item"> Item the SList::Iterator will point to.</param>
			explicit Iterator(const SList&, Node* = nullptr);
			/// <summary>
			/// Moves the SList::Iterator forward by one step.
			/// </summary>
			Iterator& operator++();
			/// <summary>
			/// Moves the SList::Iterator forward by one step.
			/// </summary>
			/// <returns> Returns a copy of the unmodified SList::Iterator.</returns>
			Iterator operator++(int);
			/// <summary>
			/// Checks whether or not two SList::Iterators are equivalent by checking if the SList::Iterator objects are both owned by the same SList and point to the same item in that list.
			/// </summary>
			/// <param name="const Iterator& other"> The other SList::Iterator that the left hand operand will be compared to.</param>
			/// <returns>true or false</returns>
			bool operator!=(const Iterator&) const;
			/// <summary>
			/// Checks whether or not two SList::Iterators are equivalent by checking if the SList::Iterator objects are both owned by the same SList and point to the same item in that list.
			/// </summary>
			/// <param name="const Iterator& other"> The other SList::Iterator that the left hand operand will be compared to.</param>
			/// <returns>true or false</returns>
			bool operator==(const Iterator&) const;
			/// <summary>
			/// Gets the item pointed to by an SList::Iterator.
			/// </summary>
			/// <returns> Returns a T& </returns>
			reference operator*() const;
		};
		/// <summary>
		/// A constant iterator that points to a given item in an SList. Does not allow the pointed-to data to be mutated.
		/// </summary>
		struct ConstIterator final
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

			friend SList;

		private:
			const SList* _owner { nullptr };
			const Node* _currentNode{ nullptr };

		public:
			/// <summary>
			/// Constructs an SList::ConstIterator that points to no item and belongs to no SList.
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Constructs an SList::ConstIterator that points to a given item and is owned by a given SList..
			/// </summary>
			/// <param name="list">The List the iterator is owned by.</param>
			/// <param name="element">The element that the iterator points to.</param>
			explicit ConstIterator(const SList&, Node* = nullptr);
			/// <summary>
			/// Constructs an SList::ConstIterator that points to the same item and is owned by the same SList as the given SList::Iterator.
			/// </summary>
			/// <param name="iterator">The iterator is an Iterator that is passed in to create a ConstIterator</param>
			ConstIterator(Iterator);
			/// <summary>
			/// Moves the SList::ConstIterator forward by one step.
			/// </summary>
			ConstIterator& operator++();
			/// <summary>
			/// Moves the SList::ConstIterator forward by one step.
			/// </summary>
			/// <returns> Returns a copy of the unmodified SList::ConstIterator.</returns>
			ConstIterator operator++(int);
			/// <summary>
			/// Checks whether or not two SList::ConstIterator objects are equivalent by checking if the SList::ConstIterator objects are both owned by the same SList and point to the same item in that list.
			/// </summary>
			/// <param name="const ConstIterator& other"> The other SList::ConstIterator that the left hand operand will be compared to.</param>
			/// <returns>true or false</returns>
			bool operator!=(const ConstIterator&) const;
			/// <summary>
			/// Checks whether or not two SList::ConstIterator objects are equivalent by checking if the SList::ConstIterator objects are both owned by the same SList and point to the same item in that list.
			/// </summary>
			/// <param name="const ConstIterator& other"> The other SList::Iterator that the left hand operand will be compared to.</param>
			/// <returns>true or false</returns>
			bool operator==(const ConstIterator&) const;
			/// <summary>
			/// Gets the item pointed to by an SList::ConstIterator. The item cannot be modified. 
			/// </summary>
			/// <returns> Returns a const T& </returns>
			const_reference operator*() const;
		};

	public:

		explicit SList() = default;
		explicit SList(std::initializer_list<value_type>);
		explicit SList(const SList&);
		explicit SList(SList&&) noexcept;
		SList& operator=(const SList&);
		SList& operator=(SList&&) noexcept;
		~SList();
		/// <summary>
		/// Returns the current number of elements in the list.
		/// </summary>
		/// <returns> std::size_t size </returns>
		[[nodiscard]] size_type Size() const;
		/// <summary>
		/// Pushes the given item to the back of the list.
		/// </summary>
		/// <param name="const T& itemToAdd"> This is the item that will be added to the back of the list. Passed by reference.</param>
		Iterator PushBack(const_reference);
		Iterator PushBack(rvalue_reference);
		template <typename...Args>
		Iterator EmplaceBack(Args&&...);
		/// <summary>
		/// Pushes the given item to the front of the list.
		/// </summary>
		/// <param name="const T& itemToAdd"> This is the item that will be added to the front of the list. Passed by reference.</param>
		void PushFront(const_reference);
		void PushFront(rvalue_reference);
		template <typename...Args>
		void EmplaceFront(Args&&...);
		/// <summary>
		/// Removes the item located at the front of the list.
		/// </summary>
		void PopFront();
		/// <summary>
		/// Removes the item located at the back of the list.
		/// </summary>
		void PopBack();
		/// <summary>
		/// Removes all elements from the list and frees all memory that has been allocated by the list.
		/// </summary>
		void Clear();
		/// <summary>
		/// Finds a given item in the list and removes it.
		/// </summary>
		/// <param name="const T& itemToRemove"> item that will be removed if found in list.</param>
		void Remove(const_reference);
		/// <summary>
		/// Inserts a given item, after a given item the provided iterator points to, in the list.
		/// </summary>
		/// <param name="const T& itemToAdd">The item that will be added to the list.</param>
		/// <param name="SList::Iterator position"> The Iterator that points to the element the item will be added after.</param>
		/// <returns>Returns an SList::Iterator pointing to the newly inserted element.</returns>
		void Remove(Iterator);
		Iterator InsertAfter(const_reference, Iterator);
		/// <summary>
		/// Checks whether or not the list is empty.
		/// </summary>
		/// <returns>Returns true or false.</returns>
		[[nodiscard]] bool IsEmpty() const;
		/// <summary>
		/// Gets the item at the front of the list.
		/// </summary>
		/// <returns>Returns a T&.</returns>
		[[nodiscard]] reference Front() const;
		/// <summary>
		/// Gets the item at the back of the list.
		/// </summary>
		/// <returns> Returns a T&. </returns>
		[[nodiscard]] reference Back() const;
		/// <summary>
		/// Gets an item at a given position in the list.
		/// </summary>
		/// <param name="std::size_t position"> The position in the list to fetch the item from.</param>
		/// <returns> Returns a T& </returns>
		[[nodiscard]] reference At(size_type) const;
		/// <summary>
		/// Returns an SList::Iterator pointing to the beginning of the list. This method can only be called by non-const SList objects.
		/// </summary>
		/// <returns> Returns an SList::Iterator pointing to the beginning of the list.</returns>
		[[nodiscard]] Iterator begin();
		/// <summary>
		/// Returns an SList::Iterator pointing to the end of the list. This method can only be called by non-const SList objects.
		/// </summary>
		/// <returns> Returns an SList::Iterator pointing to the end of the list.</returns>
		[[nodiscard]] Iterator end();
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the beginning of the list. This method can only be called by const SList objects.
		/// </summary>
		/// <returns> Returns an SList::ConstIterator pointing to the beginning of the list.</returns>
		[[nodiscard]] ConstIterator begin() const;
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the end of the list. This method can only be called by const SList objects.
		/// </summary>
		/// <returns> Returns an SList::ConstIterator pointing to the end of the list.</returns>
		[[nodiscard]] ConstIterator end() const;
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the beginning of the list. This method can be called by const or non-const SList objects.
		/// </summary>
		/// <returns> Returns an SList::ConstIterator pointing to the beginning of the list.</returns>
		[[nodiscard]] ConstIterator cbegin() const;
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the end of the list. This method can be called by const or non-const SList objects.
		/// </summary>
		/// <returns> Returns an SList::ConstIterator pointing to the end of the list.</returns>
		[[nodiscard]] ConstIterator cend() const;
		/// <summary>
		/// Finds a given item in the list and returns an SList::Iterator pointing to it. This method can only be used by non-const SList objects.
		/// </summary>
		/// <param name="const T& itemToFind"> Item to look for in the list.</param>
		/// <returns>Returns SList::Iterator pointing to the given item.</returns>
		[[nodiscard]] Iterator Find(const_reference);
		/// <summary>
		/// Finds a given item in the list and returns an SList::ConstIterator pointing to it. This method can only be used by const SList objects.
		/// </summary>
		/// <param name="const T& itemToFind"> Item to look for in the list.</param>
		/// <returns> Returns SList::ConstIterator pointing to the given item. </returns>
		[[nodiscard]] ConstIterator Find(const_reference) const;
	};
}

#include "SList.inl" 