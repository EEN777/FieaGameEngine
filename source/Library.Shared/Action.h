#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract base class Action that can hold logic/operations that can be performed on Update calls.
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

		std::string _name{};

	protected:
		Action(IdType id);

	public:
		/// <summary>
		/// Update method to be called on tick for an Action.
		/// </summary>
		/// <param name="gameTime">A reference to the current GameTime.</param>
		virtual void Update(const GameTime&) = 0;
		/// <summary>
		/// Gets the name of the action.
		/// </summary>
		/// <returns>Name of the action.</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of an action.
		/// </summary>
		/// <param name="name">The name to be set for the action.</param>
		void SetName(const std::string& name);
		/// <summary>
		/// A Vector of Signatures to be used when building the object for parsing.
		/// </summary>
		/// <returns>The Signatures needed to construct the object.</returns>
		static Vector<Signature> Signatures();
		virtual ~Action() {};
	};
}
