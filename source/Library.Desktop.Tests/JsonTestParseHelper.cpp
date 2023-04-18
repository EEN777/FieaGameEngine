#include "JsonParseCoordinator.h"
#include "pch.h"
#include "JsonTestParseHelper.h"
#include "RTTI.h"
namespace UnitTests
{
    RTTI_DEFINITIONS(JsonTestParseHelper);

    RTTI_DEFINITIONS(JsonTestParseHelper::Wrapper);

    void JsonTestParseHelper::Wrapper::Initialize()
    {
        FieaGameEngine::JsonParseCoordinator::Wrapper::Initialize();
        _stack.Push(&_scope);
    }

    void JsonTestParseHelper::Wrapper::Cleanup()
    {
        FieaGameEngine::JsonParseCoordinator::Wrapper::Cleanup();
        _stack.Pop();
    }

    std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonTestParseHelper::Wrapper::Create() const
    {
        return std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper>{ new JsonTestParseHelper::Wrapper{} };
    }

    std::shared_ptr<FieaGameEngine::IJsonParseHelper> JsonTestParseHelper::Create() const
    {
        return std::shared_ptr<JsonTestParseHelper>(new JsonTestParseHelper{});
    }

    void JsonTestParseHelper::Initialize()
    {
        FieaGameEngine::IJsonParseHelper::Initialize();
        InitializeCalled = true;
        StartHandlerCount = 0;
        EndHandlerCount = 0;
    }

    void JsonTestParseHelper::Cleanup()
    {
        FieaGameEngine::IJsonParseHelper::Cleanup();
        CleanupCalled = true;
    }

    bool UnitTests::JsonTestParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        ++StartHandlerCount;
        if (!wrapper.Is(JsonTestParseHelper::Wrapper::TypeIdClass()))
        {
            return false;
        }

        for (std::size_t index{ 0 }; index < _typeChecks.size(); ++index)
        {
            auto typeCheck = _typeChecks[index];
            assert(typeCheck != nullptr);
            if ((this->*typeCheck)(value))
            {
                auto handler = _handleFunctions[index];
                assert(handler != nullptr);
                (this->*handler)(wrapper, key, value, isArray);
                return true;
            }
        }

        return false;
    }

    bool UnitTests::JsonTestParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string&)
    {
        wrapper;
        ++EndHandlerCount;
        Cleanup();
        return false;
    }

    void JsonTestParseHelper::HandleIntegers(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        isArray;
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        auto& datum = stack.Top()->Append(key, FieaGameEngine::Datum::DatumTypes::Integer);

        datum.PushBack(value.asInt());
    }

    void JsonTestParseHelper::HandleFloats(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {

        isArray;
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        auto& datum = stack.Top()->Append(key, FieaGameEngine::Datum::DatumTypes::Float);

        datum.PushBack(value.asFloat());
    }

    void JsonTestParseHelper::HandleStrings(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        isArray;
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        auto& datum = stack.Top()->Append(key, FieaGameEngine::Datum::DatumTypes::String);

        datum.PushBack(value.asString());
    }

    void JsonTestParseHelper::HandleVectors(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        isArray;
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        auto& datum = stack.Top()->Append(key, FieaGameEngine::Datum::DatumTypes::Vector);

        datum.PushBackFromString(value.asString());
    }

    void JsonTestParseHelper::HandleMatrices(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        isArray;
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        auto& datum = stack.Top()->Append(key, FieaGameEngine::Datum::DatumTypes::Matrix);

        datum.PushBackFromString(value.asString());
    }

    void JsonTestParseHelper::HandleObject(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, [[maybe_unused]] Json::Value& value,[[maybe_unused]] bool isArray)
    {
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        auto& scope = stack.Top()->AppendScope(key);
        rawWrapper->_stack.Push(&scope);
    }
   
    void JsonTestParseHelper::HandleArray(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, [[maybe_unused]] Json::Value& value, [[maybe_unused]] bool isArray)
    {
        JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(&wrapper);
        auto& stack = rawWrapper->_stack;
        assert(!stack.IsEmpty());
        stack.Top()->Append(key);
    }

    bool JsonTestParseHelper::IsArray(Json::Value& value)
    {
        return value.isArray();
    }

    bool JsonTestParseHelper::IsInteger(Json::Value& value)
    {
        return value.isInt();
    }

    bool JsonTestParseHelper::IsVector(Json::Value& value)
    {
        if (value.isString())
        {
            std::string string{ value.asString() };
            if (string.find("vec4") != string.npos)
            {
                return true;
            }
        }
        return false;
    }

    bool JsonTestParseHelper::IsMatrix(Json::Value& value)
    {
        if (value.isString())
        {
            std::string string{ value.asString() };
            if (string.find("mat4x4") != string.npos)
            {
                return true;
            }
        }
        return false;
    }

    bool JsonTestParseHelper::IsString(Json::Value& value)
    {
        return value.isString();
    }
    bool JsonTestParseHelper::IsObject(Json::Value& value)
    {
        return value.isObject();
    }

    bool JsonTestParseHelper::IsFloat(Json::Value& value)
    {
        return value.isDouble();
    }
    JsonTestParseHelper::Wrapper::Wrapper() :
        FieaGameEngine::JsonParseCoordinator::Wrapper{}
    {

    }

    void JsonTestParseHelper::Wrapper::IncrementDepth()
    {
        FieaGameEngine::JsonParseCoordinator::Wrapper::IncrementDepth();
        MaxDepth = Depth();
    }

    void JsonTestParseHelper::Wrapper::DecrementDepth()
    {
        FieaGameEngine::JsonParseCoordinator::Wrapper::DecrementDepth();
        _stack.Pop();
    }
}