#pragma once
#include "Action.h"
#include "Factory.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A container for multiple actions to be carried out in a sequence. This ordering allows for logical actions.
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

		std::string _name{};

	protected:
		ActionList(IdType id);

	public:
		/// <summary>
		/// Default Constructor for ActionList which must call Action's constructor with its IdType.
		/// </summary>
		ActionList();
		/// <summary>
		/// Update method for ActionList that calls update on every action found in "Actions". 
		/// </summary>
		/// <param name=""></param>
		virtual void Update(const GameTime&) override;
		virtual ~ActionList() {};
		/// <summary>
		/// Signatures for the construction of an ActionList.
		/// </summary>
		/// <returns></returns>
		static Vector<Signature> Signatures();
	};

	ConcreteFactory(ActionList, Scope);
}

