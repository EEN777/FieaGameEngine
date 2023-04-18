#include "HashMap.h"
#pragma once

namespace FieaGameEngine
{
#pragma region HashMap
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::function<std::size_t(const TKey&)> method) :
		hashMethod{ method }
	{
		_map.Reserve(_chainCount);
		for (std::size_t position{ 0 }; position != _chainCount; ++position)
		{
			_map.EmplaceBack();
		}
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::size_t size, std::function<std::size_t(const TKey&)> method) :
		_chainCount{ size }, hashMethod{ method }
	{
		_map.Reserve(_chainCount);
		for (std::size_t position{ 0 }; position != _chainCount; ++position)
		{
			_map.EmplaceBack();
		}
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::function<bool(const TKey&, const TKey&)> equalityMethod, std::function<std::size_t(const TKey&)> hash) :
		equality{ equalityMethod }, hashMethod{ hash }
	{
		_map.Reserve(_chainCount);
		for (std::size_t position{ 0 }; position != _chainCount; ++position)
		{
			_map.EmplaceBack();
		}
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::size_t size, std::function<bool(const TKey&, const TKey&)> equalityMethod, std::function<std::size_t(const TKey&)> hash) :
		_chainCount{ size }, equality{ equalityMethod }, hashMethod{ hash }
	{
		_map.Reserve(_chainCount);
		for (std::size_t position{ 0 }; position != _chainCount; ++position)
		{
			_map.EmplaceBack();
		}
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(std::initializer_list<PairType> pairs) :
		HashMap<TKey, TData>{pairs.size()}
	{
		for (auto& pair : pairs)
		{
			Emplace(pair);
		}
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Insert(const PairType& kvPair)
	{
		return Emplace(kvPair);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Insert(PairType&& kvPair)
	{
		return Emplace(std::forward<PairType>(kvPair));
	}

	template<typename TKey, typename TData>
	template<typename ...Args>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Emplace(Args && ...args)
	{
		auto pair{ PairType{std::forward<Args>(args)...} };
		auto keyHash = hashMethod(pair.first) % _chainCount;
		auto iterator = Find(pair.first,keyHash);
		auto first{ pair.first };
		if (iterator == end())
		{
			_map[keyHash].EmplaceBack(std::move(pair));
			++_size;
			return Find(first, keyHash);
		}
		return iterator;
	}
	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		Iterator iterator{ Find(key) };
		assert(iterator._chainPosition != (*iterator._chain).end());
		return (*iterator._chainPosition).second;
	}

	template<typename TKey, typename TData>
	inline const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		ConstIterator iterator{ Find(key) };
		assert(iterator._chainPosition != (*iterator._chain).end());
		return (*iterator._chainPosition).second;
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return ContainsKey(key, hashMethod(key) % _chainCount);
	}

	template<typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ContainsKey(const TKey& key, std::size_t hash) const
	{
		bool result{ false };
		auto& chain{ _map[hash] };

		if (!(chain.IsEmpty()))
		{
			for (auto& value : chain)
			{
				if (equality(value.first, key))
				{
					result = true;
				}
			}
		}
		return result;
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear()
	{
		for (auto& chain : _map)
		{
			chain.Clear();
		}
		_size = 0;
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Remove(Iterator iterator)
	{
		if (iterator._owner == this)
		{
			if (iterator._chain != _map.end() && iterator._chainPosition != (*iterator._chain).end())
			{
				(*(iterator._chain)).Remove(iterator._chainPosition);
				--_size;
			}
		}
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Remove(const TKey& key)
	{
		Remove(Find(key));
	}

	template<typename TKey, typename TData>
	inline std::size_t HashMap<TKey, TData>::Size() const
	{
		return _size;
	}

	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		Iterator position{ Find(key) };
		if (position == end())
		{
			Emplace(key, TData{});
			return (At(key));
		}

		return (*position._chainPosition).second;
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		return Find(key, (hashMethod(key) % _chainCount));
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key, std::size_t hash)
	{
		auto vecIterator{ _map.begin() + hash };
		auto pairIterator{ _map[hash].begin() };
		if (_map[hash].IsEmpty())
		{
			return end();
		}

		else
		{
			while (pairIterator != (_map[hash]).end())
			{
				if (equality((*pairIterator).first, key))
				{
					return Iterator{ *this, vecIterator, pairIterator };
				}
				++pairIterator;
			}
			return end();
		}
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		return Find(key, (hashMethod(key) % _chainCount));
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key, std::size_t hash) const
	{
		auto vecIterator{ _map.begin() + hash };
		auto pairIterator{ _map[hash].begin() };
		if (_map[hash].IsEmpty())
		{
			return end();
		}

		else
		{
			while (pairIterator != (_map[hash]).end())
			{
				if (equality((*pairIterator).first, key))
				{
					return ConstIterator{ *this, vecIterator, pairIterator };
				}
				++pairIterator;
			}
			return end();
		}
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		return Iterator(*this);
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator{ *this, _map.end() };
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const
	{
		return ConstIterator{*this};
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end()const
	{
		return ConstIterator{ *this, _map.cend() };
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		return ConstIterator{ *this };
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return ConstIterator{ *this, _map.cend() };
	}
	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Rehash(std::size_t size)
	{
		Vector < std::pair<std::size_t, PairType>> items{};
		if (size > 1)
		{

			for (auto& item : *this)
			{
				items.PushBack(std::pair{ (hashMethod(item.first) % size), item});
			}

			_map.Clear();
			_map.Reserve(size);

			for (std::size_t position{ 0 }; position < size; ++position)
			{
				_map.EmplaceBack();
			}

			_chainCount = size;
				
			for (auto& pair : items)
			{
				(_map[pair.first]).PushBack(pair.second);
			}
		}
	}

	template<typename TKey, typename TData>
	inline float HashMap<TKey, TData>::GetLoadFactor()
	{
		return float{ (static_cast<float>(_size) / static_cast<float>(_chainCount)) };
	}

#pragma endregion

#pragma region Iterator

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner) :
		_owner{ &owner }, _chain{ owner._map.begin() }
	{
		for (auto& chain : _owner->_map)
		{
			if (chain.IsEmpty())
			{
				++_chain;
			}
			else
			{
				break;
			}
		}

		assert(_chain != _owner->_map.end());
		_chainPosition = (*_chain).begin();
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, Vector<ChainType>::Iterator vecEnd) :
		_owner{ &owner }, _chain{ vecEnd }
	{
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, Vector<ChainType>::Iterator vec, ChainType::Iterator pair) :
		_owner{ &owner }, _chain{ vec }, _chainPosition{ pair }
	{
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if ( _owner != nullptr)
		{
			++_chainPosition;
			if (_chainPosition == ((*_chain).end()))
			{
				++_chain;
				while (_chain != _owner->_map.end())
				{
					if (!(*_chain).IsEmpty())
					{
						_chainPosition = (*_chain).begin();
						break;
					}

					else
					{
						++_chain;
					}
				}
			}

			if (_chain == _owner->_map.end())
			{
				_chainPosition = ChainType::Iterator();
			}
		}
		return *this;
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator iterator{ *this };
		operator++();
		return iterator;
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& other) const
	{
		return _chain != other._chain || _chainPosition != other._chainPosition;
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& other) const
	{
		return !(*this != other);
	}

	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::Iterator::operator*()
	{
		return *_chainPosition;
	}
	template<typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::Iterator::operator->()
	{
		return &(*_chainPosition);
	}

#pragma endregion 

#pragma region ConstIterator

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& owner) :
		_owner{ &owner }, _chain{ owner._map.begin() }
	{
		for (auto& chain : _owner->_map)
		{
			if (chain.IsEmpty())
			{
				++_chain;
			}
			else
			{
				break;
			}
		}

		assert(_chain != _owner->_map.end());
		_chainPosition = (*_chain).begin();
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& owner, Vector<ChainType>::ConstIterator vecEnd) :
		_owner{ &owner }, _chain{ vecEnd }
	{
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& owner, Vector<ChainType>::ConstIterator vec, ChainType::ConstIterator pair) :
		_owner{ &owner }, _chain{ vec }, _chainPosition{ pair }
	{

	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner{ other._owner }, _chain{ other._chain }, _chainPosition{ other._chainPosition }
	{

	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++()
	{
		if (_owner != nullptr)
		{
			++_chainPosition;
			if (_chainPosition == ((*_chain).end()))
			{
				++_chain;
				while (_chain != _owner->_map.end())
				{
					if (!(*_chain).IsEmpty())
					{
						_chainPosition = (*_chain).begin();
						break;
					}

					else
					{
						++_chain;
					}
				}
			}

			if (_chain == _owner->_map.end())
			{
				_chainPosition = ChainType::ConstIterator();
			}
		}
		return *this;
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator iterator{ *this };
		operator++();
		return iterator;
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _chain != other._chain || _chainPosition != other._chainPosition;
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(*this != other);
	}

	template<typename TKey, typename TData>
	inline const HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::ConstIterator::operator*()
	{
		return *_chainPosition;
	}

	template<typename TKey, typename TData>
	inline const HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::ConstIterator::operator->()
	{
		return &(*_chainPosition);
	}

#pragma endregion
}