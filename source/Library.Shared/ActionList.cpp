#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList(IdType id) :
		Action(id)
	{
	}

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{
	}

	void ActionList::Update(const GameTime& gameTime) 
	{
		Datum& actions = Append("Actions");
		
		for (std::size_t index{ 0 }; index < actions.Size(); ++index)
		{
			Scope* scope = actions.GetAsScope(index);
			assert(scope != nullptr);
			assert(scope->Is(Action::TypeIdClass()));
			static_cast<Action*>(scope)->Update(gameTime);
		}
	}

	Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Actions", Datum::DatumTypes::Table, 0, 0 }
		};
	}
}
