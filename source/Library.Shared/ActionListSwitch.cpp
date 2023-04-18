#include "ActionListSwitch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListSwitch);

	ActionListSwitch::ActionListSwitch() :
		ActionList(ActionListSwitch::TypeIdClass())
	{
	}

	void ActionListSwitch::Update(const GameTime& gameTime)
	{
		Datum& actions = Append("Actions");
		if (actions.Size() != _actionMap.Size())
		{
			for (std::size_t index{ 0 }; index < actions.Size(); ++index)
			{
				Action* action = static_cast<Action*>(actions.GetAsScope(index));
				Action* nextAction{ nullptr };
				if (index + 1 != actions.Size())
				{
					if (action->Find("break") == nullptr)
					{
						nextAction = static_cast<Action*>(actions.GetAsScope(index + 1));
					}
				}
				_actionMap.Emplace(action->Name(), SwitchStatement{ action, nextAction });
			}
		}
		

		std::string currentCase{ _value };
		bool isFallingThrough{ true };
		while (isFallingThrough)
		{
			SwitchStatement& switchStatement = _actionMap.At(currentCase);
			switchStatement._statement->Update(gameTime);
			if (switchStatement._nextStatement != nullptr)
			{
				currentCase = switchStatement._nextStatement->Name();
			}
			else
			{
				isFallingThrough = false;
			}
		}
	}

	Vector<Signature> ActionListSwitch::Signatures()
	{
		Vector<Signature> signatures
		{
			FieaGameEngine::Signature{ "Value", Datum::DatumTypes::String, 1, offsetof(ActionListSwitch, _value) }
		};

		for (auto& signature : ActionList::Signatures())
		{
			signatures.EmplaceBack(signature);
		}

		return signatures;
	}
	ActionListSwitch::SwitchStatement::SwitchStatement(Action* statement, Action* nextStatement) :
		_statement{ statement }, _nextStatement{ nextStatement }
	{
	}
}
