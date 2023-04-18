#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed);

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(EventMessageAttributed::TypeIdClass())
	{
	}

	Vector<Signature> EventMessageAttributed::Signatures()
    {
		return Vector<Signature>
		{
			FieaGameEngine::Signature{ "SubType", Datum::DatumTypes::String, 1, offsetof(EventMessageAttributed, _subtype) }
		};
    }
}
