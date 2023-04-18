#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "GameState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent);

	ActionEvent::ActionEvent() :
		Action(ActionEvent::TypeIdClass())
	{
	}

	void ActionEvent::Update(const GameTime& gameTime)
	{
		EventMessageAttributed message{};
		message.SetSubType(_subtype);
		auto indices = AuxillaryAttributes();
		for (std::size_t index{ indices.first }; index < indices.second; ++index)
		{
			auto& attribute = message.AppendAuxillaryAttribute(_orderVector[index]->first, _orderVector[index]->second.Type());
			attribute = _orderVector[index]->second;
		}

		GameState::EnqueueEvent(std::make_shared<Event<EventMessageAttributed>>(message), gameTime, std::chrono::milliseconds{ _delay });
	}
	Vector<Signature> ActionEvent::Signatures()
	{
		Vector<Signature> signatures
		{
			Signature{ "SubType", FieaGameEngine::Datum::DatumTypes::String, 1, offsetof(ActionEvent, _subtype) },
			Signature{ "Delay", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(ActionEvent, _delay) }
		};

		for (auto& signature : Action::Signatures())
		{
			signatures.EmplaceBack(signature);
		}

		return signatures;
	}
}
