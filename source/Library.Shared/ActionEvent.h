#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

		std::string _subtype{};
		int _delay{};

	public:
		/// <summary>
		/// Public constructor that calls Actions constructor.
		/// </summary>
		ActionEvent();
		/// <summary>
		/// Creates the event to be added to the global GameState EventQueue. Copies all auxillary attributes into the payload for delivery. 
		/// </summary>
		/// <param name="">const GameTime reference.</param>
		virtual void Update(const GameTime&) override;
		/// <summary>
		/// Returns the Signature of prescribed attributes of the class.
		/// </summary>
		/// <returns>A Vector of Signatures describing the prescribed attributes.</returns>
		static Vector<Signature> Signatures();
	};

	ConcreteFactory(ActionEvent, Scope);
}