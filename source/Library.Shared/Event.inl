#pragma once
#include "Event.h"

namespace FieaGameEngine
{
	template <typename T>
	Event<T>::Event(T message) :
		_message{ message }
	{
	}

	template <typename T>
	inline Event<T>::Subscriber::Subscriber(std::function<void(T)> functor) :
		_callback{ functor }, _id{ reinterpret_cast<std::size_t>(this) }
	{
	}

	template <typename T>
	void Event<T>::Subscribe(Subscriber subscriber)
	{
		_pendingSubscribers.Push(subscriber);
	}


	template <typename T>
	void Event<T>::Unsubscribe(Subscriber subscriber)
	{
		for (auto position{ _subscribers.begin() }; position != _subscribers.end(); ++position )
		{
			Subscriber& callback = *position;
			if (callback._id == subscriber._id)
			{
				_subscribers.Remove(position, position + 1);
				return;
			}
		}
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		_subscribers.Clear();
		_pendingSubscribers.Clear();
	}
	template<typename T>
	inline void Event<T>::Deliver()
	{
		while (!_pendingSubscribers.IsEmpty())
		{
			_subscribers.PushBack(std::move(_pendingSubscribers.Top()));
			_pendingSubscribers.Pop();
		}

		for (auto& subscriber : _subscribers)
		{
			subscriber._callback(_message);
		}
	}

	template <typename T>
	inline const T& Event<T>::Message()
	{
		return _message;
	}
}
