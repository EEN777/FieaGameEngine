#pragma once

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <functional>
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A container that holds its members in a dynamic contiguous array.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Vector final
	{
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = T&&;

	private:
		value_type* _vec;
		size_type _capacity{ 5 };
		size_type _size{ 0 };
		std::function<std::size_t(std::size_t, std::size_t)> incrementFunctor{ DefaultIncrement<value_type> {} };

	public:

		/// <summary>
		/// An Iterator that points to an item in a vector.
		/// </summary>
		struct Iterator final
		{
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

			friend Vector;
			friend struct ConstIterator;
		private:
			Vector* _owner{ nullptr };
			value_type* _position{ nullptr };

		public:
			/// <summary>
			/// Creates a default Iterator that has no owner and points to nullptr.
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Constructs an Iterator given a Vector to declare as its owner, and a value to point at within said vector.
			/// </summary>
			/// <param name="vector"></param>
			/// <param name="position"></param>
			Iterator(Vector&, value_type* = nullptr);
			/// <summary>
			/// Moves the position of the Iterator to its next position in contiguous memory.
			/// </summary>
			/// <returns>Iterator reference</returns>
			Iterator& operator++();
			/// <summary>
			/// Moves the position of the Iterator to its next position in contiguous memory and returns the original Iterator.
			/// </summary>
			/// <returns>Iterator</returns>
			Iterator operator++(int);
			/// <summary>
			/// Moves the position of the Iterator to its previous position in contiguous memory.
			/// </summary>
			/// <returns>Iterator reference</returns>
			Iterator& operator--();
			/// <summary>
			/// Moves the position of the Iterator to its previous position in contiguous memory and returns the original Iterator.
			/// </summary>
			/// <returns>Iterator</returns>
			Iterator operator--(int);
			/// <summary>
			/// Compares Iterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const Iterator reference other"></param>
			/// <returns>bool</returns>
			bool operator!=(const Iterator&) const;
			/// <summary>
			/// Compares Iterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const Iterator reference other"></param>
			/// <returns>bool</returns>
			bool operator==(const Iterator&) const;
			/// <summary>
			/// Compares Iterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const Iterator reference other"></param>
			/// <returns>bool</returns>
			bool operator<(const Iterator&) const;
			/// <summary>
			/// Compares Iterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const Iterator reference other"></param>
			/// <returns>bool</returns>
			bool operator<=(const Iterator&) const;
			/// <summary>
			/// Compares Iterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const Iterator reference other"></param>
			/// <returns>bool</returns>
			bool operator>(const Iterator&) const;
			/// <summary>
			/// Compares Iterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const Iterator reference other"></param>
			/// <returns>bool</returns>
			bool operator>=(const Iterator&) const;
			/// <summary>
			/// Dereferences the position in the vector that the Iterator points to. Checks bounds.
			/// </summary>
			/// <returns>value_type reference</returns>
			reference operator*() const;
			/// <summary>
			/// Returns a reference to the item at the given index in the Iterator's owning vector. Checks for null, but does no bounds checking.
			/// </summary>
			/// <param name="position"></param>
			/// <returns>value_type reference</returns>
			reference operator[](std::size_t) const;
			/// <summary>
			/// returns an iterator a given offset forward in the vector from the passed Iterator. Does not bounds check.
			/// </summary>
			/// <param name="position"></param>
			/// <param name="offset"></param>
			/// <returns>Iterator</returns>
			friend Iterator operator+(const Iterator& position, size_type offset)
			{
#ifdef USING_EXCEPTIONS
				if (position._owner == nullptr)
				{
					throw std::runtime_error("Not a valid Iterator.");
				}
#endif
				return Iterator(*position._owner, position._position + offset);
			}
			/// <summary>
			/// Returns the distance between two iterators.
			/// </summary>
			/// <param name="lhs"></param>
			/// <param name="rhs"></param>
			/// <returns>size_t</returns>
			friend size_type operator-(const Iterator& lhs, const Iterator& rhs)
			{
#ifdef USING_EXCEPTIONS
				if (lhs._owner == nullptr || lhs._owner != rhs._owner)
				{
					throw std::runtime_error("Not a valid Iterator match.");
				}
#endif
				if(rhs > lhs)
				{
					return size_type{ 0 };
				}
				return lhs._position - rhs._position;
			}
			/// <summary>
			/// returns an iterator a given offset backwards in the vector from the passed Iterator. Does not bounds check.
			/// </summary>
			/// <param name="position"></param>
			/// <param name="offset"></param>
			/// <returns>Iterator</returns>
			friend Iterator operator-(const Iterator& position, size_type offset)
			{
#ifdef USING_EXCEPTIONS
				if (position._owner == nullptr)
				{
					throw std::runtime_error("Not a valid Iterator.");
				}
#endif
				return Iterator(*position._owner, position._position - offset);
			}
			/// <summary>
			/// Moves the iterator a given offset forward. No bounds checking.
			/// </summary>
			/// <param name="offset"></param>
			/// <returns></returns>
			Iterator& operator+=(size_type offset);
			/// <summary>
			/// Moves the iterator a given offset backwards. No bounds checking.
			/// </summary>
			/// <param name="offset"></param>
			/// <returns></returns>
			Iterator& operator-=(size_type offset);
		};
		/// <summary>
		/// A ConstIterator that points to an item in a vector. Cannot modify the owning vector.
		/// </summary>
		struct ConstIterator final
		{
			friend Vector;
		private:
			const Vector* _owner{ nullptr };
			const value_type* _position{ nullptr };
		public:
			/// <summary>
			/// Creates a default ConstIterator that has no owner and points to nullptr.
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Constructs a ConstIterator given a Vector to declare as its owner, and a value to point at within said vector.
			/// </summary>
			/// <param name="vector"></param>
			/// <param name="position"></param>
			ConstIterator(const Vector&, const value_type* = nullptr);
			/// <summary>
			/// Constructs a ConstIterator given an Iterator.
			/// </summary>
			/// <param name="Iterator"></param>
			ConstIterator(Iterator);
			/// <summary>
			/// Moves the position of the ConstIterator to its next position in contiguous memory.
			/// </summary>
			/// <returns>ConstIterator reference</returns>
			ConstIterator& operator++();
			/// <summary>
			/// Moves the position of the ConstIterator to its next position in contiguous memory and returns the original ConstIterator.
			/// </summary>
			/// <returns>ConstIterator</returns>
			ConstIterator operator++(int);
			/// <summary>
			/// Moves the position of the ConstIterator to its previous position in contiguous memory.
			/// </summary>
			/// <returns>Iterator reference</returns>
			ConstIterator& operator--();
			/// <summary>
			/// Moves the position of the ConstIterator to its previous position in contiguous memory and returns the original ConstIterator.
			/// </summary>
			/// <returns>ConstIterator</returns>
			ConstIterator operator--(int);
			/// <summary>
			/// Compares ConstIterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const ConstIterator reference other"></param>
			/// <returns>bool</returns>
			bool operator!=(const ConstIterator&) const;
			/// <summary>
			/// Compares ConstIterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const ConstIterator reference other"></param>
			/// <returns>bool</returns>
			bool operator==(const ConstIterator&) const;
			/// <summary>
			/// Compares ConstIterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const ConstIterator reference other"></param>
			/// <returns>bool</returns>
			bool operator<(const ConstIterator&) const;
			/// <summary>
			/// Compares ConstIterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const ConstIterator reference other"></param>
			/// <returns>bool</returns>
			bool operator<=(const ConstIterator&) const;
			/// <summary>
			/// Compares ConstIterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const ConstIterator reference other"></param>
			/// <returns>bool</returns>
			bool operator>(const ConstIterator&) const;
			/// <summary>
			/// Compares ConstIterators based on position in vector they point to. 
			/// </summary>
			/// <param name="const ConstIterator reference other"></param>
			/// <returns>bool</returns>
			bool operator>=(const ConstIterator&) const;
			/// <summary>
			/// Dereferences the position in the vector that the Iterator points to. Checks bounds. The value cannot be modified.
			/// </summary>
			/// <returns>const value_type reference</returns>
			const_reference operator*() const;
			/// <summary>
			/// Returns a const reference to the item at the given index in the ConstIterator's owning vector. Checks for null, but does no bounds checking.
			/// </summary>
			/// <param name="position"></param>
			/// <returns>const value_type reference</returns>
			const_reference operator[](std::size_t) const;
			/// <summary>
			/// returns an ConstIterator a given offset forward in the vector from the passed ConstIterator. Does not bounds check.
			/// </summary>
			/// <param name="position"></param>
			/// <param name="offset"></param>
			/// <returns>ConstIterator</returns>
			friend ConstIterator operator+(const ConstIterator& position, size_type offset)
			{
#ifdef USING_EXCEPTIONS
				if (position._owner == nullptr)
				{
					throw std::runtime_error("Not a valid Iterator.");
				}
#endif
				return ConstIterator(*position._owner, position._position + offset);
			}
			/// <summary>
			/// Returns the distance between two ConstIterators.
			/// </summary>
			/// <param name="lhs"></param>
			/// <param name="rhs"></param>
			/// <returns>size_t</returns>
			friend size_type operator-(const ConstIterator& lhs, const ConstIterator& rhs)
			{
#ifdef USING_EXCEPTIONS
				if (lhs._owner == nullptr || lhs._owner != rhs._owner)
				{
					throw std::runtime_error("Not a valid Iterator match.");
				}
#endif
				if (rhs > lhs)
				{
					return size_type{ 0 };
				}
				return lhs._position - rhs._position;
			}
			/// <summary>
			/// returns an ConstIterator a given offset backwards in the vector from the passed ConstIterator. Does not bounds check.
			/// </summary>
			/// <param name="position"></param>
			/// <param name="offset"></param>
			/// <returns>ConstIterator</returns>
			friend ConstIterator operator-(const ConstIterator& position, size_type offset)
			{
#ifdef USING_EXCEPTIONS
				if (position._owner == nullptr)
				{
					throw std::runtime_error("Not a valid Iterator.");
				}
#endif
				return ConstIterator(*position._owner, position._position - offset);
			}
			/// <summary>
			/// Moves the ConstIterator a given offset forward. No bounds checking.
			/// </summary>
			/// <param name="offset"></param>
			/// <returns></returns>
			ConstIterator& operator+=(size_type offset);
			/// <summary>
			/// Moves the ConstIterator a given offset backwards. No bounds checking.
			/// </summary>
			/// <param name="offset"></param>
			/// <returns></returns>
			ConstIterator& operator-=(size_type offset);
		};

		/// <summary>
		/// Creates an empty vector with a capacity of 5.
		/// </summary>
		explicit Vector(std::function<std::size_t(std::size_t, std::size_t)> = DefaultIncrement<value_type>{});
		/// <summary>
		/// Creates a Vector containing the provided elements and an equivalent size and capacity.
		/// </summary>
		/// <param name=""></param>
		explicit Vector(std::initializer_list<value_type>, std::function<std::size_t(std::size_t, std::size_t)> = DefaultIncrement<value_type>{});
		/// <summary>
		/// Copy Constructs a vector given another vector.
		/// </summary>
		/// <param name="other"></param>
		Vector(const Vector&);
		/// <summary>
		/// Move Constructs a vector given an rvalue reference to a vector.
		/// </summary>
		/// <param name="other"></param>
		Vector(Vector&&) noexcept;
		/// <summary>
		/// Copies the contents of a vector into another vector.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Vector reference</returns>
		Vector& operator=(const Vector&);
		/// <summary>
		/// Moves the contents of another vector into a vector.
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Vector reference</returns>
		Vector& operator=(Vector&&) noexcept;
		/// <summary>
		/// Clears vector and frees memory.
		/// </summary>
		~Vector();
		/// <summary>
		/// Returns the functor currently used for increment.
		/// </summary>
		/// <returns>functor</returns>
		std::function<std::size_t(std::size_t, std::size_t)> GetIncrementStrategy();
		/// <summary>
		/// Changes the current increment to the passed Functor. The functor must return a size_t and take two size_t's as parameters. 
		/// </summary>
		/// <param name="">functor</param>
		void SetIncrementStrategy(std::function<std::size_t(std::size_t, std::size_t)>);
		/// <summary>
		/// Copies an item in the back of the vector. If there is not enough capacity, more memory will be allocated.
		/// </summary>
		void PushBack(const_reference);
		/// <summary>
		/// Copies an item in the back of the vector. If there is not enough capacity, more memory will be allocated.
		/// </summary>
		void PushBack(rvalue_reference);
		/// <summary>
		/// Places an item in the back of the vector. If there is not enough capacity, more memory will be allocated.
		/// </summary>
		template<typename...Args>
		void EmplaceBack(Args&&...);
		/// <summary>
		/// Clears contents of the vector.
		/// </summary>
		void Clear();
		/// <summary>
		/// Reserves a given amount of space for items in the vector. Will not shrink.
		/// </summary>
		/// <param name=""></param>
		void Reserve(size_type);
		/// <summary>
		/// Relocates the items in the vector in order to take up less space in memory.
		/// </summary>
		void ShrinkToFit();
		/// <summary>
		/// Removes the last item in the vector.
		/// </summary>
		void PopBack();
		/// <summary>
		/// Removes the items in a vector in the given range of iterators not including the rightmost iterator.
		/// </summary>
		/// <param name="begin"></param>
		/// <param name="end"></param>
		void Remove(Iterator, Iterator);
		/// <summary>
		/// Finds an item in the vector and removes it.
		/// </summary>
		/// <param name=""></param>
		void Remove(const_reference);
		/// <summary>
		/// Returns an item at a given position in the vector. No bounds checking.
		/// </summary>
		/// <returns>value_type reference</returns>
		[[nodiscard]] reference operator[](size_t);
		/// <summary>
		/// Returns an item at a given position in the vector. No bounds checking.
		/// </summary>
		/// <returns>const value_type reference</returns>
		[[nodiscard]] const_reference operator[](size_t) const;
		/// <summary>
		/// Returns reference to item at the front of the vector.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] reference Front() const;
		/// <summary>
		/// Returns reference to item at the back of the vector.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] reference Back() const;
		/// <summary>
		/// Returns an item at a given position in the vector. Bounds checking.
		/// </summary>
		/// <returns>value_type reference</returns>
		[[nodiscard]] reference At(size_t);
		/// <summary>
		/// Returns an item at a given position in the vector. Bounds checking.
		/// </summary>
		/// <returns>const value_type reference</returns>
		[[nodiscard]] const_reference At(size_t) const;
		/// <summary>
		/// Returns size of the vector.
		/// </summary>
		/// <returns>size_t</returns>
		[[nodiscard]] size_type Size() const;
		/// <summary>
		/// Returns capacity of the vector.
		/// </summary>
		/// <returns>size_t</returns>
		[[nodiscard]] size_type Capacity() const;
		/// <summary>
		/// Checks whether or not the vector is empty.
		/// </summary>
		/// <returns>bool</returns>
		[[nodiscard]] bool IsEmpty() const;
		/// <summary>
		/// Returns an Iterator to the front of the vector.
		/// </summary>
		/// <returns>Iterator</returns>
		[[nodiscard]] Iterator begin();
		/// <summary>
		/// Returns an Iterator to one past the back of the vector.
		/// </summary>
		/// <returns>Iterator</returns>
		[[nodiscard]] Iterator end();
		/// <summary>
		/// Returns an ConstIterator to the front of the vector.
		/// </summary>
		/// <returns>ConstIterator</returns>
		[[nodiscard]] ConstIterator begin() const;
		/// <summary>
		/// Returns an ConstIterator to one past the back of the vector.
		/// </summary>
		/// <returns>ConstIterator</returns>
		[[nodiscard]] ConstIterator end() const;
		/// <summary>
		/// Returns an ConstIterator to the front of the vector.
		/// </summary>
		/// <returns>ConstIterator</returns>
		[[nodiscard]] ConstIterator cbegin() const;
		/// <summary>
		/// Returns an ConstIterator to one past the back of the vector.
		/// </summary>
		/// <returns>ConstIterator</returns>
		[[nodiscard]] ConstIterator cend() const;
		/// <summary>
		/// Finds a given item in the vector and return an Iterator pointing to it. Will return an Iterator pointing to the end if it is not found.
		/// </summary>
		/// <param name="itemToFind"></param>
		/// <returns>Iterator</returns>
		[[nodiscard]] Iterator Find(const_reference);
		/// <summary>
		/// Finds a given item in the vector and return an ConstIterator pointing to it. Will return an ConstIterator pointing to the end if it is not found.
		/// </summary>
		/// <param name="itemToFind"></param>
		/// <returns>ConstIterator</returns>
		[[nodiscard]] ConstIterator Find(const_reference) const;
	};
}

#include "Vector.inl"