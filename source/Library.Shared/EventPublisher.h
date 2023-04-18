#pragma once
#include "GameTime.h"
#include "RTTI.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract base class that serves as a convenient way to store any type of event within an EventQueue. 
	/// </summary>
	class EventPublisher
	{
	public:
		/// <summary>
		/// Pure virtual function used by Event to traverse its list of subscribers.
		/// </summary>
		virtual void Deliver() = 0;
	};
}
