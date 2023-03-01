#include "Attributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(std::size_t rttiType)
	{
		_dataType = rttiType;
		auto typeManager = TypeManager::Instance();
		auto& signatures = typeManager->GetSignatureForTypeID(rttiType);
		for (auto& signature : signatures)
		{
			Datum& member = Append(signature._name, signature._type);
			if (signature._type != Datum::DatumTypes::Table)
			{
				member.SetStorage(this + signature._offset, signature._size);
			}
		}
	}
}