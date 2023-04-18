#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionListSwitch provides functionalty for an ActionList to emply switch statement logic.
	/// </summary>
	class ActionListSwitch final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListSwitch, ActionList);
		/// <summary>
		/// SwitchStatement holds the statement action to be updated and, if there is no break statement, the next statement to update.
		/// </summary>
		struct SwitchStatement final
		{
			Action* _statement{ nullptr };
			Action* _nextStatement{ nullptr };

			SwitchStatement() = delete;
			/// <summary>
			/// Constructs a SwitchStatement that contains pointers to two Actions. The first Action to update, and then the one following it. If there is none, this will be null.
			/// </summary>
			/// <param name="statement"></param>
			/// <param name="nextStatement"></param>
			SwitchStatement(Action* statement, Action* nextStatement);
			SwitchStatement(const SwitchStatement&) = default;
			SwitchStatement(SwitchStatement&&) = default;
			SwitchStatement& operator=(const SwitchStatement&) = default;
			SwitchStatement& operator=(SwitchStatement&&) = default;
			~SwitchStatement() = default;
		};


		std::string _value{ "Default" };
		HashMap<std::string, SwitchStatement> _actionMap{};

	public:
		/// <summary>
		/// Default Constructor for ActionListSwitch, which in turn calls a specialized constructor for ActionList.
		/// </summary>
		ActionListSwitch();
		/// <summary>
		/// Given its value in JSON script, it will perform the corresponding action on Update.
		/// </summary>
		/// <param name="gameTime">Reference to an instance of a GameTime.</param>
		virtual void Update(const GameTime& gameTime) override;
		/// <summary>
		/// Returns the Vector of Signatures neccessary to construct an ActionListSwitch.
		/// </summary>
		/// <returns></returns>
		static Vector<Signature> Signatures();

	};

	ConcreteFactory(ActionListSwitch, Scope);
}
