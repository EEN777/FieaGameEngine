#pragma once
#include "Action.h"

namespace UnitTests 
{
	class ActionIncrement final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

	public:
		std::string _operand{};
		int _step{};

		virtual void Update(const FieaGameEngine::GameTime&) override;
		ActionIncrement();

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(ActionIncrement, FieaGameEngine::Scope);
}
