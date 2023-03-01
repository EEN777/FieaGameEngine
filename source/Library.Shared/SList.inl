#include "SList.h"

namespace FieaGameEngine
{
#pragma region Iterator
	template <typename value_type>
	SList<value_type>::Iterator::Iterator(const SList& other, Node* node) :
		_owner{ &other }, _currentNode{ node }
	{

	}

	template <typename value_type>
	inline typename SList<value_type>::Iterator& SList<value_type>::Iterator::operator++()
	{
		_currentNode != nullptr ? _currentNode = _currentNode->next :_currentNode;
		return *this;
	}

	template <typename value_type>
	inline typename SList<value_type>::Iterator SList<value_type>::Iterator::operator++(int)
	{
		Iterator iterator{ *this };
		operator++();
		return iterator;
	}

	template<typename value_type>
	inline bool SList<value_type>::Iterator::operator!=(const Iterator& other) const
	{
		if ((_currentNode == other._currentNode) && (_owner == other._owner))
		{
			return false;
		}
		return true;
	}


	template<typename value_type>
	inline bool SList<value_type>::Iterator::operator==(const Iterator& other) const
	{
		if((_currentNode == other._currentNode) && (_owner == other._owner))
		{
			return true;
		}
		return false;
	}

	template<typename value_type>
	inline typename SList<value_type>::reference SList<value_type>::Iterator::operator*() const
	{
#ifdef _DEBUG
		if (_currentNode == nullptr)
		{
			throw std::runtime_error("Attempting to dereference a null pointer. SList::Iterator does not point to a valid SList item.");
		}
#endif
		return _currentNode->item;
	}

#pragma endregion Iterator

#pragma region ConstIterator
	template <typename value_type>
	SList<value_type>::ConstIterator::ConstIterator(const SList& other, Node* node) :
		_owner{ &other }, _currentNode{ node }
	{

	}

	template <typename value_type>
	SList<value_type>::ConstIterator::ConstIterator(Iterator other) :
		_owner{ other._owner }, _currentNode{ other._currentNode }
	{

	}

	template <typename value_type>
	inline typename SList<value_type>::ConstIterator& SList<value_type>::ConstIterator::operator++()
	{
		_currentNode != nullptr ? _currentNode = _currentNode->next : _currentNode;
		return *this;
	}

	template <typename value_type>
	inline typename SList<value_type>::ConstIterator SList<value_type>::ConstIterator::operator++(int)
	{
		ConstIterator iterator{ *this };
		operator++();
		return iterator;
	}

	template<typename value_type>
	inline bool SList<value_type>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _currentNode != other._currentNode || _owner != other._owner;
	}

	template<typename value_type>
	inline bool SList<value_type>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename value_type>
	inline typename SList<value_type>::const_reference SList<value_type>::ConstIterator::operator*() const
	{
#ifdef _DEBUG
		if (_currentNode == nullptr)
		{
			throw std::runtime_error("Attempting to dereference a null pointer. SList::ConstIterator does not point to a valid SList item.");
		}
#endif _DEBUG
		return _currentNode->item;
	}
#pragma endregion ConstIterator

#pragma region SList

	template<typename value_type>
	inline SList<value_type>::Node::Node(const_reference itemToAdd, Node* nextNode) :
		item{ itemToAdd }, next {nextNode }
	{
	}

	template<typename value_type>
	inline SList<value_type>::Node::Node(rvalue_reference itemToAdd, Node* nextNode) :
		item{ std::forward<value_type>(itemToAdd) }, next{ nextNode }
	{

	}

	template<typename value_type>
	inline SList<value_type>::SList(std::initializer_list<value_type> list)
	{
		for (auto& item : list)
		{
			EmplaceBack(item);
		}
	}
	template <typename value_type>
	SList<value_type>::SList(const SList& other)
	{
		for (auto& item : other)
		{
			PushBack(item);
		}
	}

	template<typename value_type>
	inline SList<value_type>::SList(SList&& other) noexcept :
		_front{ other._front }, _back{ other._back }, _size{ other._size }
	{
		other._front = nullptr;
		other._back = nullptr;
		other._size = 0;
	}

	template <typename value_type>
	SList<value_type>& SList<value_type>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();

			Node* currentNode{ other._front };

			while (currentNode->next != nullptr)
			{
				PushBack(currentNode->item);
				currentNode = currentNode->next;
			}

			PushBack(currentNode->item);
		}

		return *this;
	}

	template<typename value_type>
	inline SList<value_type>& SList<value_type>::operator=(SList&& other) noexcept
	{
		Clear();
		_front = other._front;
		_back = other._back;
		_size = other._size;

		other._front = nullptr;
		other._back = nullptr;
		other._size = 0;
		return *this;
	}

	template <typename value_type>
	inline typename SList<value_type>::size_type SList<value_type>::Size() const
	{
		return _size;
	}

	template <typename value_type>
	[[nodiscard]] inline typename SList<value_type>::reference SList<value_type>::At(size_type location) const
	{
		if (location <= (_size - 1) && _size > 0)
		{
			Node* currentPosition = _front;
			for (size_t position{ 0 }; position < location; ++position)
			{
				currentPosition = currentPosition->next;
			}

			return currentPosition->item;
		}
		throw std::runtime_error("Out of bounds! Element does not exist!");
	}

	template<typename value_type>
	inline typename SList<value_type>::Iterator SList<value_type>::begin()
	{
		return Iterator(*this, _front);
	}

	template<typename value_type>
	inline typename SList<value_type>::Iterator SList<value_type>::end()
	{
		return Iterator(*this, nullptr);
	}

	template<typename value_type>
	inline typename SList<value_type>::ConstIterator SList<value_type>::begin() const
	{
		return ConstIterator(*this, _front);
	}

	template<typename value_type>
	inline typename SList<value_type>::ConstIterator SList<value_type>::end() const
	{
		return ConstIterator( *this, nullptr );
	}

	template<typename value_type>
	inline typename SList<value_type>::ConstIterator SList<value_type>::cbegin() const
	{
		return ConstIterator(*this, _front);
	}

	template<typename value_type>
	inline typename SList<value_type>::ConstIterator SList<value_type>::cend() const
	{
		return ConstIterator(*this, nullptr);
	}

	template <typename value_type>
	inline  SList<value_type>::Iterator SList<value_type>::PushBack(const_reference itemToAdd)
	{
		return EmplaceBack(itemToAdd);
	}

	template<typename value_type>
	inline  SList<value_type>::Iterator SList<value_type>::PushBack(rvalue_reference itemToAdd)
	{
		return EmplaceBack(std::forward<value_type>(itemToAdd));
	}

	template<typename value_type>
	template<typename ...Args>
	inline SList<value_type>::Iterator SList<value_type>::EmplaceBack(Args && ...args)
	{
		Node* node = new Node{ value_type{std::forward<Args>(args)...}, nullptr };

		if (_size == 0)
		{
			_front = node;
		}

		else
		{
			_back->next = node;
		}

		_back = node;

		++_size;

		return Iterator(*this, node);
	}


	template <typename value_type>
	inline void SList<value_type>::PushFront(const_reference itemToAdd)
	{
		EmplaceFront(itemToAdd);
	}

	template<typename T>
	inline void SList<T>::PushFront(rvalue_reference itemToAdd)
	{
		EmplaceFront(std::forward<value_type>(itemToAdd));
	}

	template<typename value_type>
	template<typename ...Args>
	inline void SList<value_type>::EmplaceFront(Args && ...args)
	{
		_front = new Node{ value_type{std::forward<Args>(args)...}, _front };
		if (_size == 0)
		{
			_back = _front;
		}
		++_size;
	}

	template <typename value_type>
	inline void SList<value_type>::PopFront()
	{
		if (_size == 0)
		{
			return;
		}
		Node* node = _front;
		if (_size > 1)
		{
			_front = _front->next;
		}
		else
		{
			_front = nullptr;
		}
		delete node;
		--_size;
	}
	template <typename value_type>
	inline void SList<value_type>::PopBack()
	{

		if (_size == 0)
		{
			return;
		}

		Node* currentNode = _front;
		Node* previousNode = _front;

		while (currentNode->next != nullptr)
		{
			previousNode = currentNode;
			currentNode = currentNode->next;
		}

		delete currentNode;

		if(_size ==1)
		{
			_back = nullptr;
			_front = nullptr;
		}

		else
		{
			_back = previousNode;
			_back->next = nullptr;
		}
		--_size;
	}

	template <typename value_type>
	inline void SList<value_type>::Clear()
	{
		Node* currentNode = _front;
		while(currentNode != nullptr)
		{
			Node* node = currentNode;
			currentNode = currentNode->next;
			delete node;
		}

		_front = nullptr;
		_back = nullptr;
		_size = 0;
	}

	template <typename value_type>
	[[nodiscard]] inline bool SList<value_type>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename value_type>
	[[nodiscard]] inline typename SList<value_type>::reference SList<value_type>::Front() const
	{
#ifdef _DEBUG
		if (_front == nullptr)
		{
			throw std::runtime_error("List is empty, you are attempting to dereference a null pointer");
		}
#endif 
		return _front->item;
	}

	template <typename value_type>
	[[nodiscard]] inline typename SList<value_type>::reference SList<value_type>::Back() const
	{
#ifdef _DEBUG
		if (_back == nullptr)
		{
			throw std::runtime_error("List is empty, you are attempting to dereference a null pointer");
		}
#endif
		return _back->item;
	}

	template <typename value_type>
	[[nodiscard]] inline typename SList<value_type>::Iterator SList<value_type>::Find(const_reference value)
	{
		auto iterator{ begin() };
		while (iterator != end())
		{
			if (*iterator == value)
			{
				break;
			}
			++iterator;
		}
		return iterator;
	}

	template <typename value_type>
	[[nodiscard]] inline typename SList<value_type>::ConstIterator SList<value_type>::Find(const_reference value) const
	{
		auto iterator{ cbegin() };
		while (iterator != cend())
		{
			if (*iterator == value)
			{
				break;
			}
			++iterator;
		}
		return iterator;
	}

	template <typename value_type>
	inline void SList<value_type>::Remove(const_reference value)
	{
		SList::Iterator previous{ begin() };
		SList::Iterator iterator{ begin() };

		while (iterator != end())
		{
			if (value == *iterator)
			{
				if (_front == iterator._currentNode)
				{
					_front = iterator._currentNode->next;
					delete iterator._currentNode;
					--_size;
				}

				else
				{
					previous._currentNode->next = iterator._currentNode->next;
					delete iterator._currentNode;
					if (previous._currentNode->next == nullptr)
					{
						_back = previous._currentNode;
					}
					--_size;
				}

				return;
			}

			previous = iterator++;
		}
	}

	template<typename value_type>
	inline void SList<value_type>::Remove(Iterator position)
	{
		if (position._owner != this)
		{
			return;
		}

		if (position != end())
		{
			if (position._currentNode == _back)
			{
				PopBack();
			}

			else
			{
				Node* node = position._currentNode->next;
				position._currentNode->item.~value_type();
				new (&position._currentNode->item)value_type(std::move(node->item));

				if (position._currentNode->next == nullptr)
				{
					_back = position._currentNode;
				}

				else
				{
					position._currentNode->next = (position._currentNode->next)->next;
				}

				delete node;
			}
		}


	}

	template <typename value_type>
	inline typename SList<value_type>::Iterator SList<value_type>::InsertAfter(const_reference itemToAdd, SList<value_type>::Iterator position)
	{
		if (this != position._owner)
		{
			throw std::runtime_error("Invalid Iterator");
		}

		if (position._currentNode == _back || position._currentNode == nullptr)
		{
			PushBack(itemToAdd);
			return Iterator(*this, _back);
		}

		else
		{
			Node* nextNode = position._currentNode->next;
			Node* node = new Node{ itemToAdd, nextNode };

			position._currentNode->next = node;
			++_size;
			return Iterator{ *this, node };
		}
	}

	template <typename value_type>
	SList<value_type>::~SList()
	{
		Clear();
	}

#pragma endregion SList

}