#include "pch.h"
#include "ActionIncrement.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(ActionIncrement);

	void ActionIncrement::Update(const FieaGameEngine::GameTime&)
	{
		FieaGameEngine::Datum* operand = Search(_operand);
		assert(operand != nullptr);
		assert(operand->Type() == FieaGameEngine::Datum::DatumTypes::Integer);
		operand->GetAsInt() += _step;
	}

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> ActionIncrement::Signatures()
	{
		FieaGameEngine::Vector<FieaGameEngine::Signature> signatures
		{
			FieaGameEngine::Signature{ "Operand", FieaGameEngine::Datum::DatumTypes::String, 1, offsetof(ActionIncrement, _operand) },
			FieaGameEngine::Signature{ "Step", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(ActionIncrement, _step) },
		};

		for (auto& signature : FieaGameEngine::Action::Signatures())
		{
			signatures.EmplaceBack(signature);
		}

		return signatures;
	}
}

