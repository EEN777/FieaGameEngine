#pragma once
#include <json/json.h>
#include "JsonParseCoordinator.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	class IJsonParseHelper : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, FieaGameEngine::RTTI);
	public:
		virtual void Initialize();
		virtual void Cleanup();
		virtual bool StartHandler(JsonParseCoordinator::Wrapper&, const std::string&, Json::Value&, bool) = 0;
		virtual bool EndHandler(JsonParseCoordinator::Wrapper&, const std::string&) = 0;
		virtual std::shared_ptr<IJsonParseHelper> Create() const = 0;
		virtual ~IJsonParseHelper() = default;
	};
}