#include "Vector.h"
#include <cassert>

namespace FieaGameEngine
{
#pragma region Vector
	template <typename value_type>
	Vector<value_type>::Vector(std::function<std::size_t(std::size_t, std::size_t)> functor) :
		incrementFunctor{functor}
	{
		_vec = reinterpret_cast<value_type*>(malloc(_capacity * sizeof(value_type)));
	}

	template<typename value_type>
	inline Vector<value_type>::Vector(std::initializer_list<value_type> list, std::function<std::size_t(std::size_t, std::size_t)> functor) :
		_capacity{ list.size() }, incrementFunctor{functor}
	{
		_vec = reinterpret_cast<value_type*>(malloc(_capacity *sizeof(value_type)));
		for (auto& item : list)
		{
			EmplaceBack(item);
		}
	}

	template<typename value_type>
	inline Vector<value_type>::Vector(const Vector& other) :
		_capacity{ other._capacity }, _size{ other._size }, incrementFunctor{other.incrementFunctor}
	{
		_vec = reinterpret_cast<value_type*>(malloc(_capacity * sizeof(value_type)));

		for (size_type position{ 0 }; position < _size; ++position)
		{
			new (_vec + position)value_type{ other._vec[position] };
		}
	}

	template<typename value_type>
	inline Vector<value_type>::Vector(Vector&& other) noexcept :
		_vec{ other._vec }, _size{ other._size }, _capacity{ other._capacity }, incrementFunctor{ other.incrementFunctor }
	{
		other._vec = nullptr;
		other._size = 0;
		other._capacity = 0;
	}

	template<typename value_type>
	inline Vector<value_type>& Vector<value_type>::operator=(const Vector& other)
	{
		if(this != &other)
		{
			Clear();
			_size = other._size;
			incrementFunctor = other.incrementFunctor;

			if(_capacity < other._capacity)
			{
				Reserve(other._capacity);
			}

			for (size_type position{ 0 }; position < other._size; ++position)
			{
				new (_vec + position)value_type{ other._vec[position] };
			}
		}

		return *this;
	}

	template<typename value_type>
	inline Vector<value_type>& Vector<value_type>::operator=(Vector&& other) noexcept
	{
		if(this != &other)
		{
			Clear();
			free(_vec);
			_vec = other._vec;
			_size = other._size;
			_capacity = other._capacity;
			incrementFunctor = other.incrementFunctor;

			other._vec = nullptr;
			other._size = 0;
			other._capacity = 0;
		}

		return *this;
	}


	template <typename value_type>
	inline void Vector<value_type>::PushBack(const_reference itemToAdd)
	{
		EmplaceBack(itemToAdd);
	}

	template <typename value_type>
	inline void Vector<value_type>::PushBack(rvalue_reference itemToAdd)
	{
		EmplaceBack(std::forward<rvalue_reference>(itemToAdd));
	}

	template <typename value_type>
	template <typename...Args>
	inline void Vector<value_type>::EmplaceBack(Args&&... args)
	{
		if ((_size + 1) > _capacity)
		{
			_capacity = std::max(incrementFunctor(_capacity, _size), _capacity + 1);
			value_type* vec = reinterpret_cast<value_type*>(realloc(_vec, _capacity * sizeof(value_type)));
			assert(vec != nullptr);
			_vec = vec;
		}
		new (_vec + _size++)value_type{ std::forward<Args>(args)...};
	}

	template <typename value_type>
	inline void Vector<value_type>::Reserve(size_type capacity)
	{
		if(capacity > _capacity)
		{
			value_type* vec = reinterpret_cast<value_type*>(realloc(_vec, (capacity) * sizeof(value_type)));
			assert(vec != nullptr);
			_vec = vec;
			_capacity = capacity;
		}
	}

	template <typename value_type>
	inline void Vector<value_type>::ShrinkToFit()
	{
		value_type* vec = reinterpret_cast<value_type*>(realloc(_vec, (_capacity = _size == 0 ?  1 : _size) * sizeof(value_type)));
		assert(vec != nullptr);
		_vec = vec;
	}

	template<typename value_type>
	inline void Vector<value_type>::PopBack()
	{
		if(_size >= 1)
		{
			_vec[_size-- - 1].~value_type();
		}
	}

	template <typename value_type>
	inline typename Vector<value_type>::reference Vector<value_type>::operator[](size_t position)
	{
		assert(position < _size);
		return _vec[position];
	}

	template<typename value_type>
	inline typename Vector<value_type>::const_reference Vector<value_type>::operator[](size_t position) const
	{
		assert(position < _size);
		return _vec[position];
	}

	template <typename value_type>
	inline void Vector<value_type>::Clear()
	{
		for (size_type position{ 0 }; position < _size; ++position)
		{
			_vec[position].~value_type();
		}
		_size = 0;
	}

	template <typename value_type>
	inline typename Vector<value_type>::reference Vector<value_type>::Front() const
	{
#ifdef USING_EXCEPTIONS
		if (_size == 0)
		{
			throw std::runtime_error("You are attempting to dereference invalid memory.");
		}
#endif
		return _vec[0];
	}

	template <typename value_type>
	inline typename Vector<value_type>::reference Vector<value_type>::Back() const
	{
#ifdef USING_EXCEPTIONS
		if (_size == 0)
		{
			throw std::runtime_error("You are attempting to dereference invalid memory.");
		}
#endif
		return _vec[_size -1];
	}

	template <typename value_type>
	inline typename Vector<value_type>::reference Vector<value_type>::At(size_t position)
	{
#ifdef USING_EXCEPTIONS
		if (position >= _size)
		{
			throw std::out_of_range("Out of bounds error!");
		}
#endif
		return _vec[position];
	}

	template <typename value_type>
	inline typename Vector<value_type>::const_reference Vector<value_type>::At(size_t position) const
	{
#ifdef USING_EXCEPTIONS
		if (position >= _size)
		{
			throw std::out_of_range("Out of bounds error!");
		}
#endif
		return _vec[position];
	}

	template <typename value_type>
	inline size_t Vector<value_type>::Size() const
	{
		return _size;
	}

	template <typename value_type>
	size_t Vector<value_type>::Capacity() const
	{
		return _capacity;
	}


	template <typename value_type>
	bool Vector<value_type>::IsEmpty() const
	{
		return _size == 0;
	}


	template <typename value_type>
	inline typename Vector<value_type>::Iterator Vector<value_type>::begin()
	{
		return Iterator(*this, _vec);
	}

	template <typename value_type>
	inline typename Vector<value_type>::Iterator Vector<value_type>::end()
	{
		return Iterator(*this, _vec + _size);
	}

	template<typename value_type>
	inline typename Vector<value_type>::ConstIterator Vector<value_type>::begin() const
	{
		return ConstIterator( *this, _vec);
	}

	template<typename value_type>
	inline typename Vector<value_type>::ConstIterator Vector<value_type>::end() const
	{
		return ConstIterator(*this, _vec + _size);
	}

	template<typename value_type>
	inline typename Vector<value_type>::ConstIterator Vector<value_type>::cbegin() const
	{
		return ConstIterator(*this, _vec);
	}

	template<typename value_type>
	inline typename Vector<value_type>::ConstIterator Vector<value_type>::cend() const
	{
		return ConstIterator(*this, _vec + _size);
	}

	template<typename value_type>
	inline typename Vector<value_type>::Iterator Vector<value_type>::Find(const_reference itemToFind)
	{
		Iterator position{ begin() };

		while (position != end())
		{
			if(*position == itemToFind)
			{
				break;
			}
			++position;
		}
		return position;
	}

	template<typename value_type>
	inline typename Vector<value_type>::ConstIterator Vector<value_type>::Find(const_reference itemToFind) const
	{
		ConstIterator position{ begin() };

		while (position != end())
		{
			if (*position == itemToFind)
			{
				break;
			}
			++position;
		}
		return position;
	}

	template<typename value_type>
	inline void Vector<value_type>::Remove(Iterator first, Iterator last)
	{
		if (first._owner != last._owner)
		{
			return;
		}

		if (first >= end() || last > end())
		{
			return;
		}

		if ( last > first && first._owner == last._owner)
		{
			auto iterator{ first };
			while(iterator != last)
			{
				(*iterator).~value_type();
				++iterator;
			}
			memmove(first._position, last._position, (end() - last)*sizeof(value_type));
			_size -= (last - first);
		}
	}

	template<typename value_type>
	inline void Vector<value_type>::Remove(const_reference value)
	{
		auto iterator{ Find(value)};
		Remove(iterator, iterator + 1);
	}

	template <typename value_type>
	Vector<value_type>::~Vector()
	{
		for (size_type position{ 0 }; position < _size; ++position )
		{
			_vec[position].~value_type();
		}
		free(_vec);
	}

	template<typename value_type>
	inline std::function<std::size_t(std::size_t, std::size_t)> Vector<value_type>::GetIncrementStrategy()
	{
		return incrementFunctor;
	}

	template<typename value_type>
	inline void Vector<value_type>::SetIncrementStrategy(std::function<std::size_t(std::size_t, std::size_t)> strategy)
	{
		incrementFunctor = strategy;
	}
#pragma endregion Vector

#pragma region Iterator
	template <typename value_type>
	inline bool Vector<value_type>::Iterator::operator!=(const Iterator& other) const
	{
		return _position != other._position || _owner != other._owner;
	}

	template <typename value_type>
	inline bool Vector<value_type>::Iterator::operator==(const Iterator& other) const
	{
		return !(*this != other);
	}

	template <typename value_type>
	inline bool Vector<value_type>::Iterator::operator<(const Iterator& other) const
	{
		assert(_owner == other._owner);
		return _position < other._position;
	}

	template <typename value_type>
	inline bool Vector<value_type>::Iterator::operator<=(const Iterator& other) const
	{
		assert(_owner == other._owner);
		return _position <= other._position;
	}

	template <typename value_type>
	inline bool Vector<value_type>::Iterator::operator>(const Iterator& other) const
	{
		assert(_owner == other._owner);
		return _position > other._position;
	}

	template <typename value_type>
	inline bool Vector<value_type>::Iterator::operator>=(const Iterator& other) const
	{
		assert(_owner == other._owner);
		return _position >= other._position;
	}

	template<typename value_type>
	inline Vector<value_type>::reference Vector<value_type>::Iterator::operator[](std::size_t position) const
	{
#ifdef USING_EXCEPTIONS
		if (_owner == nullptr)
		{
			throw std::runtime_error("You are attempting to dereference a null pointer.");
		}
#endif
		return (*_owner)[position];
	}

	template <typename value_type>
	inline typename Vector<value_type>::reference Vector<value_type>::Iterator::operator*() const
	{
#ifdef USING_EXCEPTIONS
		if ( _position >= _owner->_vec + _owner->_size || _position < _owner->_vec || _position == nullptr)
		{
			throw std::runtime_error("Invalid Iterator");
		}
#endif
		return *_position;
	}

	template <typename value_type>
	Vector<value_type>::Iterator::Iterator(Vector& vector, value_type* position) :
		_owner{ &vector }, _position{ position }
	{

	}

	template <typename value_type>
	typename Vector<value_type>::Iterator& Vector<value_type>::Iterator::operator++()
	{
		if (_owner != nullptr)
		{
			_position >= _owner->_vec + _owner->_size ? _position : ++_position;
		}
		return *this;
	}

	template <typename value_type>
	typename Vector<value_type>::Iterator Vector<value_type>::Iterator::operator++(int)
	{
		Iterator iterator{ *this };
		operator++();
		return iterator;
	}

	template <typename value_type>
	typename Vector<value_type>::Iterator& Vector<value_type>::Iterator::operator--()
	{
		if (_owner != nullptr)
		{
			_position <= _owner->_vec ? _position : --_position;
		}
		return *this;
	}

	template <typename value_type>
	typename Vector<value_type>::Iterator Vector<value_type>::Iterator::operator--(int)
	{
		Iterator iterator{ *this };
		operator--();
		return iterator;
	}

	template<typename value_type>
	inline typename Vector<value_type>::Iterator::Iterator& Vector<value_type>::Iterator::operator+=(size_type offset)
	{
		_position += offset;
		return *this;
	}

	template<typename value_type>
	inline typename Vector<value_type>::Iterator::Iterator& Vector<value_type>::Iterator::operator-=(size_type offset)
	{
		_position -= offset;
		return *this;
	}
#pragma endregion Iterator

#pragma region ConstIterator
	template <typename value_type>
	Vector<value_type>::ConstIterator::ConstIterator(const Vector& vector, const value_type* position) :
		_owner{ &vector }, _position{ position }
	{
		
	}

	template <typename value_type>
	Vector<value_type>::ConstIterator::ConstIterator(Iterator other) :
		_owner{ other._owner }, _position{ other._position }
	{
		
	}

	template <typename value_type>
	inline bool Vector<value_type>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _position != other._position || _owner != other._owner;
	}

	template <typename value_type>
	inline bool Vector<value_type>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(*this != other);
	}

	template <typename value_type>
	inline bool Vector<value_type>::ConstIterator::operator<(const ConstIterator& other) const
	{
		assert(_owner == other._owner);
		return _position < other._position;
	}

	template<typename value_type>
	inline bool Vector<value_type>::ConstIterator::operator<=(const ConstIterator& other) const
	{
		assert(_owner == other._owner);
		return _position <= other._position;
	}

	template <typename value_type>
	inline bool Vector<value_type>::ConstIterator::operator>(const ConstIterator& other) const
	{
		assert(_owner == other._owner);
		return _position > other._position;
	}

	template<typename value_type>
	inline bool Vector<value_type>::ConstIterator::operator>=(const ConstIterator& other) const
	{
		assert(_owner == other._owner);
		return _position >= other._position;
	}

	template<typename value_type>
	inline Vector<value_type>::const_reference Vector<value_type>::ConstIterator::operator[](std::size_t position) const
	{
#ifdef USING_EXCEPTIONS
		if (_owner == nullptr)
		{
			throw std::runtime_error("You are attempting to dereference a null pointer.");
		}
#endif
		return (*_owner)[position];
	}

	template <typename value_type>
	inline typename Vector<value_type>::const_reference Vector<value_type>::ConstIterator::operator*() const
	{
#ifdef USING_EXCEPTIONS
		if(_position >= _owner->_vec + _owner->_size || _position < _owner->_vec || _position == nullptr)
		{
			throw std::runtime_error("Invalid Iterator");
		}
#endif
		return *_position;
	}

	template <typename value_type>
	typename Vector<value_type>::ConstIterator& Vector<value_type>::ConstIterator::operator++()
	{
		if (_owner != nullptr)
		{
			_position >= _owner->_vec + _owner->_size ? _position: ++_position;
		}
		return *this;
	}

	template <typename value_type>
	typename Vector<value_type>::ConstIterator Vector<value_type>::ConstIterator::operator++(int)
	{
		ConstIterator iterator{ *this };
		operator++();
		return iterator;
	}

	template <typename value_type>
	typename Vector<value_type>::ConstIterator& Vector<value_type>::ConstIterator::operator--()
	{
		if (_owner != nullptr)
		{
			_position <= _owner->_vec ? _position : --_position;
		}
		return *this;
	}

	template <typename value_type>
	typename Vector<value_type>::ConstIterator Vector<value_type>::ConstIterator::operator--(int)
	{
		ConstIterator iterator{ *this };
		operator--();
		return iterator;
	}

	template<typename value_type>
	inline Vector<value_type>::ConstIterator& Vector<value_type>::ConstIterator::operator+=(size_type offset)
	{
		_position += offset;
		return *this;
	}

	template<typename value_type>
	inline Vector<value_type>::ConstIterator& Vector<value_type>::ConstIterator::operator-=(size_type offset)
	{
		_position -= offset;
		return *this;
	}
#pragma endregion ConstIterator
}
