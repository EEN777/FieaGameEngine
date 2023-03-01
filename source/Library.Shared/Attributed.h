#pragma once
#include "Scope.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	class Attributed : public FieaGameEngine::Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

		std::size_t _dataType;

	protected:
		Attributed(std::size_t rttiType);

	public:
		Attributed() = delete;
		virtual ~Attributed() = default;
		[[nodiscard]] bool IsAttribute(const std::string& attributeName);
		[[nodiscard]] bool IsPrescribedAttribute(const std::string& attributeName);
		[[nodiscard]] bool IsAuxillaryAttribute();
		Datum& AppendAuxillaryAttribute();
	};
}
