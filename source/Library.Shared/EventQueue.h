#pragma once
#include "GameTime.h"
#include "EventPublisher.h"
#include "Vector.h"
#include "Stack.h"

namespace FieaGameEngine
{
	class EventQueue final
	{
		/// <summary>
		/// EventWrapper is a wrapper for an event that stores a shared_ptr to an event and the time point at which it should be delivered.
		/// </summary>
		struct EventWrapper final
		{
			std::shared_ptr<EventPublisher> _event;
			std::chrono::steady_clock::time_point _timeIsExpired{};

			EventWrapper() = delete;
			EventWrapper(std::shared_ptr<EventPublisher>, std::chrono::steady_clock::time_point);

		};

		Stack <EventWrapper> _pendingEvents{};
		Vector <EventWrapper> _events{};

		bool _isUpdating{ false };

	public:
		/// <summary>
		/// Adds an Event to the queue of events to be fired once their time points have been reached or exceeded. Constructs an EventWrapper to store based off of trhe passed in arguments.
		/// </summary>
		/// <param name="event"> A shared_ptr to a created event </param>
		/// <param name="gameTime"> A reference to a GameTime instance </param>
		/// <param name="delay"> The amount of time in milliseconds you wish to delay the delivery of the message for. </param>
		void Enqueue(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, std::chrono::milliseconds delay = std::chrono::milliseconds{ 0 });
		/// <summary>
		/// Iterates through the list of events and calls deliver on them id their time point has been reached or exceeded by the passed in GameTime's current time.
		/// Users should under no circumstances, call Update on the instance of the same EventQueue your event is stored in or else they will be stuck in an infinite loop.
		/// In Debug, this is asserted against in case of any mistakes, but there are no checks in release mode. 
		/// </summary>
		/// <param name="gameTime"> Reference to an instance of a GameTime </param>
		void Update(const GameTime& gameTime);
		/// <summary>
		/// Clears all events from the event list, effectively cancelling them.
		/// </summary>
		void Clear();
		/// <summary>
		/// Returns whether or not there are any events in the queue.
		/// </summary>
		/// <returns> Whether or not there are any events in the queue </returns>
		bool IsEmpty();
		/// <summary>
		/// Returns the size of the queue.
		/// </summary>
		/// <returns> Size of the queue. </returns>
		std::size_t Size();
	};
}