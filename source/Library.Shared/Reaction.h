#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{

	class Reaction : public ActionList
	{
		RTTI_DECLARATIONS(Reaction, ActionList);
	protected:
		Reaction(IdType id);
		virtual ~Reaction() {};
	};
}
