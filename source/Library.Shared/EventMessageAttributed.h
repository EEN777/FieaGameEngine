#pragma once
#include "Attributed.h"

namespace FieaGameEngine
{
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

		std::string _subtype{};

	public:
		/// <summary>
		/// Public Constructor that calls Attributeds constructor using the IdType of the class.
		/// </summary>
		EventMessageAttributed();
		/// <summary>
		/// Returns the associated SubType of the EventMessageAttributed.
		/// </summary>
		/// <returns> The SubType of the EventMessageAttributed. </returns>
		inline const std::string& SubType() { return _subtype; };
		/// <summary>
		/// Sets the SubType of the EventMessageAttributed.
		/// </summary>
		/// <param name="subtype">Name of the SubType to assign to the EventMessageAttributed.</param>
		inline void SetSubType(const std::string& subtype) { _subtype = subtype; };
		/// <summary>
		/// Returns the Signature of prescribed attributes of the class.
		/// </summary>
		/// <returns>A Vector of Signatures describing the prescribed attributes.</returns>
		static Vector<Signature> Signatures();
	};
}