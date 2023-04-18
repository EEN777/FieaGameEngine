#pragma once
#include <cstddef>
#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include "Stack.h"
#include "Scope.h"
#include "HashMap.h"

namespace FieaGameEngine
{
	class JsonTableParseHelper : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, FieaGameEngine::IJsonParseHelper);

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);

		public:
			/// <summary>
			/// Creates a fresh wrapper to be passed into a parse cooridnator instance.
			/// </summary>
			/// <returns>A shared ptr to a wrapper, thus assuming ownership.</returns>
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;
			/// <summary>
			/// Calls the base class constructor.
			/// </summary>
			Wrapper();
			/// <summary>
			/// Overridden base class method that decrements depth, but also pops off the stack of contexts.
			/// </summary>
			void DecrementDepth() override;
			/// <summary>
			/// The Scope to be poulated via the wrapper.
			/// </summary>
			std::shared_ptr<Scope> _scope{ nullptr };
			/// <summary>
			/// A pointer to the current helper.
			/// </summary>
			JsonTableParseHelper* currentHelper{ nullptr };
		};

		/// <summary>
		/// A Context is a struct that stores information specific to a given context while parsing a Json value.
		/// </summary>
		struct Context final
		{
			/// <summary>
			/// Current Scope in context.
			/// </summary>
			Scope* _currentScope;
			/// <summary>
			/// Current Context name.
			/// </summary>
			const std::string _contextName;
			const std::string _className;
			/// <summary>
			/// Array size. non-zero only when object is an array.
			/// </summary>
			const std::size_t _arraySize{ 0 };
			/// <summary>
			/// Index used per context to populate external datums.
			/// </summary>
			std::size_t _index{ 0 };
			bool _isInArray{ false };
			/// <summary>
			/// Primary Constructor for a Context.
			/// </summary>
			/// <param name="scope">The scope at this context.</param>
			/// <param name="name">The name of the context.</param>
			/// <param name="arraySize">The size of the array, if the object is one.</param>
			Context(Scope* scope, const std::string& name, const std::string& className, std::size_t arraySize, bool isInArray = false);
		};
		/// <summary>
		/// A Stack of Context objects used to contexualize data as it is parsed.
		/// </summary>
		FieaGameEngine::Stack<Context> _contexts;
		/// <summary>
		/// Creates an identical helper.
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<IJsonParseHelper> Create() const override;
		/// <summary>
		/// Clears all data members and makes helper ready for a fresh wrapper.
		/// </summary>
		void Initialize() override;
		/// <summary>
		/// Cleans up any data after the wrapper is finished being processed by the helper.
		/// </summary>
		void Cleanup() override;
		/// <summary>
		/// Begins the parsing process by validating that we accept the wrapper and then we attempt to try and find a method that can handle the data type.
		/// </summary>
		/// <param name="wrapper">The wrapper being passed in to be worked on.</param>
		/// <param name="key">The name associated with this value.</param>
		/// <param name="value">Json value to parse through.</param>
		/// <param name="isArray">Whether or not the value is an array.</param>
		/// <returns>bool</returns>
		virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray) override;
		/// <summary>
		/// Handles any neccesary clenup to begin the parse again.
		/// </summary>
		/// <param name="wrapper">The wrapper being worked on.</param>
		/// <param name="key">The name associated with the current context.</param>
		/// <returns>bool</returns>
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key) override;

	private:
		/// <summary>
		/// Attempts to handle the value as an integer.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleIntegers(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Attempts to handle the value as an object.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleObject(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Attempts to handle the value as a float.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleFloats(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Attempts to handle the value as an array.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleArray(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Attempts to handle the value as a string.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleStrings(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Attempts to handle the value as a vector.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleVectors(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Attempts to handle the value as a matrix.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		/// <param name="isArray"></param>
		void HandleMatrices(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray);
		/// <summary>
		/// Performs the neccesary steps for setting up a fresh datum for a given value.
		/// </summary>
		/// <param name="wrapper"></param>
		/// <param name="key"></param>
		/// <param name="isArray"></param>
		/// <param name="type"></param>
		/// <returns></returns>
		Datum& PrimitiveHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, [[maybe_unused]] bool isArray, Datum::DatumTypes type = Datum::DatumTypes::Unknown);

		using HandleFunctor = void(JsonTableParseHelper::*)(FieaGameEngine::JsonParseCoordinator::Wrapper&, const std::string&, Json::Value&, bool);

		inline static const std::array<HandleFunctor, 7> _handleFunctions
		{
			&JsonTableParseHelper::HandleArray,
			&JsonTableParseHelper::HandleIntegers,
			&JsonTableParseHelper::HandleVectors,
			&JsonTableParseHelper::HandleMatrices,
			&JsonTableParseHelper::HandleStrings,
			&JsonTableParseHelper::HandleObject,
			&JsonTableParseHelper::HandleFloats
		};

		bool IsArray(Json::Value& value);
		bool IsInteger(Json::Value& value);
		bool IsFloat(Json::Value& value);
		bool IsVector(Json::Value& value);
		bool IsMatrix(Json::Value& value);
		bool IsString(Json::Value& value);
		bool IsObject(Json::Value& value);

		using TypeCheck = bool(JsonTableParseHelper::*)(Json::Value&);

		inline static const std::array<TypeCheck, 7> _typeChecks
		{
			&JsonTableParseHelper::IsArray,
			&JsonTableParseHelper::IsInteger,
			&JsonTableParseHelper::IsVector,
			&JsonTableParseHelper::IsMatrix,
			&JsonTableParseHelper::IsString,
			&JsonTableParseHelper::IsObject,
			&JsonTableParseHelper::IsFloat
		};
	};
}
