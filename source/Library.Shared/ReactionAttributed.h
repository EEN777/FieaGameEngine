#pragma once
#include "Reaction.h"
#include "EventMessageAttributed.h"
#include "Event.h"

namespace FieaGameEngine
{
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

		std::string _subtype{};

		Event<EventMessageAttributed>::Subscriber _subscriber{ std::function<void(EventMessageAttributed)> { std::bind(&ReactionAttributed::Notify, this, std::placeholders::_1) }};

	public:
		/// <summary>
		/// Notify method that is subscribed to events of EventMessageAttributed upon construction.
		/// </summary>
		/// <param name="message">The payload message being delivered.</param>
		void Notify(EventMessageAttributed message);
		/// <summary>
		/// Public constructor that calls Reactions constructor.
		/// </summary>
		ReactionAttributed();
		virtual ~ReactionAttributed();
		/// <summary>
		/// Returns the Signature of prescribed attributes of the class.
		/// </summary>
		/// <returns>A Vector of Signatures describing the prescribed attributes.</returns>
		static Vector<Signature> Signatures();
	};

	ConcreteFactory(ReactionAttributed, Scope);
}