#include "ReactionAttributed.h"
#include "GameState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed);

	void ReactionAttributed::Notify(EventMessageAttributed message)
	{
		if (_subtype == message.SubType())
		{
			Datum& actions = _orderVector.At(2)->second;
			for (std::size_t index{ 0 }; index < actions.Size(); ++index)
			{
				Scope* scope = actions.GetAsScope(index);
				auto typeManager = TypeManager::Instance();
				auto& signatures = typeManager->GetSignatureForTypeID(scope->TypeIdInstance());
				for (auto& signature : signatures)
				{
					auto payload = message.Find(signature._name);
					if (payload != nullptr)
					{
						Datum& variable = scope->Append(signature._name);
						switch (variable.Type())
						{
						case Datum::DatumTypes::Integer:
							for (std::size_t position{ 0 }; position < variable.Size(); ++position)
							{
								variable.Set(payload->GetAsInt(position), position);
							}
							break;
						case Datum::DatumTypes::Float:
							for (std::size_t position{ 0 }; position < variable.Size(); ++position)
							{
								variable.Set(payload->GetAsFloat(position), position);
							}
							break;
						case Datum::DatumTypes::String:
							for (std::size_t position{ 0 }; position < variable.Size(); ++position)
							{
								variable.Set(payload->GetAsString(position), position);
							}
							break;
						case Datum::DatumTypes::Vector:
							for (std::size_t position{ 0 }; position < variable.Size(); ++position)
							{
								variable.Set(payload->GetAsVector(position), position);
							}
							break;
						case Datum::DatumTypes::Matrix:
							for (std::size_t position{ 0 }; position < variable.Size(); ++position)
							{
								variable.Set(payload->GetAsMatrix(position), position);
							}
							break;
						case Datum::DatumTypes::Table:
							variable = *payload;
						}
					}
				}

			}
			ActionList::Update(GameState::GlobalGameTimeInstance());
		}
	}

	ReactionAttributed::ReactionAttributed() :
		Reaction(ReactionAttributed::TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(_subscriber);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(_subscriber);
	}

	Vector<Signature> ReactionAttributed::Signatures()
	{
		Vector<Signature> signatures
		{
			Signature{ "SubType", Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, _subtype) }
		};

		for (auto& signature : ActionList::Signatures())
		{
			signatures.EmplaceBack(signature);
		}

		return signatures;
	}
}

