#include "EventQueue.h"

namespace FieaGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		_pendingEvents.Push({ std::move(event), gameTime.CurrentTime() + delay });
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		assert(!_isUpdating);

		_isUpdating = true;

		while (!_pendingEvents.IsEmpty())
		{
			_events.PushBack(_pendingEvents.Top());
			_pendingEvents.Pop();
		}

		auto firstExpiredEvent = std::partition(_events.begin(), _events.end(), [gameTime](const EventWrapper& eventWrapper) {  return gameTime.CurrentTime() >= eventWrapper._timeIsExpired ? false : true; });

		auto position{ firstExpiredEvent };
		while (position != _events.end())
		{
			(*position)._event->Deliver();
			++position;
		}

		_events.Remove(firstExpiredEvent, _events.end());
		_isUpdating = false;
	}

	void EventQueue::Clear()
	{
		assert(!_isUpdating);
		_pendingEvents.Clear();
		_events.Clear();
	}

	bool EventQueue::IsEmpty()
	{
		if (_pendingEvents.IsEmpty() && _events.IsEmpty())
		{
			return true;
		}
		return false;
	}
	std::size_t EventQueue::Size()
	{
		return _events.Size() + _pendingEvents.Size();
	}

	EventQueue::EventWrapper::EventWrapper(std::shared_ptr<EventPublisher> event , std::chrono::steady_clock::time_point timeIsExpired) :
		_event{ std::move(event) }, _timeIsExpired{ timeIsExpired }
	{
	}
}
