#pragma once
#include "EventPublisher.h"
#include "Stack.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A Templated class that inherits from EventPublisher and can deliver any given T through a std::function of void(T) to any reigstered Subscriber.
	/// </summary>
	template <typename T>
	class Event final : public EventPublisher
	{
		using CallbackFunction = std::function<void(T)>;

	public:
		/// <summary>
		/// Subscriber serves as a wrapper for our std::function that also implements a GUID through the address of the object an insantiation.
		/// </summary>
		struct Subscriber final
		{
			CallbackFunction _callback{};
			std::size_t _id{ 0 };

			Subscriber() = delete;
			Subscriber(CallbackFunction functor);
		};

	private:
		const T _message{};
		inline static Vector<Subscriber> _subscribers{};
		inline static Stack<Subscriber> _pendingSubscribers{};

	public:
		/// <summary>
		/// Constructor for an event that stores a copy of a given T.
		/// </summary>
		Event(T message);
		/// <summary>
		/// Static method that subscribes a given Subscriber to the list of active subscribers.
		/// </summary>
		static void Subscribe(Subscriber subscriber);
		/// <summary>
		/// Static method that unsubscribes a given Subscriber from the list of active subscribers.
		/// </summary>
		static void Unsubscribe(Subscriber subscriber);
		/// <summary>
		/// Static method that unsubscribes all Subscribers from the list of active subscribers.
		/// </summary>
		static void UnsubscribeAll();
		/// <summary>
		/// Iterates through all subscribers and delivers the current message.
		/// </summary>
		void Deliver() override;
		/// <summary>
		/// Returns a constant reference to the stored message.
		/// </summary>
		const T& Message();
	};

}
#include "Event.inl"
