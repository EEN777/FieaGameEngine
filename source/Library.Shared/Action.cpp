#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);


	Action::Action(IdType id) :
		Attributed{ id }
	{
	}

	const std::string& Action::Name() const
	{
		return _name;
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}
	Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			FieaGameEngine::Signature{ "Name", FieaGameEngine::Datum::DatumTypes::String, 1, offsetof(Action, _name) }
		};
	}
}

