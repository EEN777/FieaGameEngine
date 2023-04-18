#pragma once
#include <cstddef>
#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include "Stack.h"
#include "Scope.h"
#include "HashMap.h"

namespace UnitTests
{
	class JsonTestParseHelper : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper);

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);

		public:
			void Initialize() override;
			void Cleanup() override;
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;
			Wrapper();
			void IncrementDepth() override;
			void DecrementDepth() override;
			std::size_t MaxDepth{ 0 };
			FieaGameEngine::Stack<FieaGameEngine::Scope*> _stack;
			FieaGameEngine::Scope _scope{};
		};

		std::shared_ptr<IJsonParseHelper> Create() const override;
		void Initialize() override;
		void Cleanup() override;
		virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper&, const std::string&, Json::Value&, bool) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper&, const std::string&) override;
		bool InitializeCalled{ false };
		bool CleanupCalled{ false };
		std::size_t StartHandlerCount{ 0 };
		std::size_t EndHandlerCount{ 0 };

	private:
		void HandleIntegers(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		void HandleObject(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		void HandleFloats(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		void HandleArray(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		void HandleStrings(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		void HandleVectors(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		void HandleMatrices(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);

		using HandleFunctor = void(JsonTestParseHelper::*)(FieaGameEngine::JsonParseCoordinator::Wrapper&, const std::string&, Json::Value&, bool);

		inline static const std::array<HandleFunctor, 7> _handleFunctions
		{
			&JsonTestParseHelper::HandleArray,
			&JsonTestParseHelper::HandleIntegers,
			&JsonTestParseHelper::HandleVectors,
			&JsonTestParseHelper::HandleMatrices,
			&JsonTestParseHelper::HandleStrings,
			&JsonTestParseHelper::HandleObject,
			&JsonTestParseHelper::HandleFloats
		};

		bool IsArray(Json::Value& value);
		bool IsInteger(Json::Value& value);
		bool IsFloat(Json::Value& value);
		bool IsVector(Json::Value& value);
		bool IsMatrix(Json::Value& value);
		bool IsString(Json::Value& value);
		bool IsObject(Json::Value& value);

		using TypeCheck = bool(JsonTestParseHelper::*)(Json::Value&);

		inline static const std::array<TypeCheck, 7> _typeChecks
		{
			&JsonTestParseHelper::IsArray,
			&JsonTestParseHelper::IsInteger,
			&JsonTestParseHelper::IsVector,
			&JsonTestParseHelper::IsMatrix,
			&JsonTestParseHelper::IsString,
			&JsonTestParseHelper::IsObject,
			&JsonTestParseHelper::IsFloat
		};
	};
}