#include "JsonParseCoordinator.h"
#include "pch.h"
#include "JsonTableParseHelper.h"
#include "RTTI.h"
#include "Factory.h"
#include "ActionExpression.h"
#include "ExpressionParser.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(JsonTableParseHelper);

    RTTI_DEFINITIONS(JsonTableParseHelper::Wrapper);

    std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonTableParseHelper::Wrapper::Create() const
    {
        return std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper>{ new JsonTableParseHelper::Wrapper{} };
    }

    std::shared_ptr<FieaGameEngine::IJsonParseHelper> JsonTableParseHelper::Create() const
    {
        return std::shared_ptr<JsonTableParseHelper>(new JsonTableParseHelper{});
    }

    void JsonTableParseHelper::Initialize()
    {
        FieaGameEngine::IJsonParseHelper::Initialize();
    }

    void JsonTableParseHelper::Cleanup()
    {
        FieaGameEngine::IJsonParseHelper::Cleanup();
    }

    bool JsonTableParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        if (!wrapper.Is(JsonTableParseHelper::Wrapper::TypeIdClass()))
        {
            return false;
        }

        for (std::size_t index{ 0 }; index < _typeChecks.size(); ++index)
        {
            auto typeCheck = _typeChecks[index];
            assert(typeCheck != nullptr);
            if ((this->*typeCheck)(value))
            {
                JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(&wrapper);
                if (rawWrapper->currentHelper == nullptr)
                {
                    rawWrapper->currentHelper = this;
                }

                auto handler = _handleFunctions[index];
                assert(handler != nullptr);
                (this->*handler)(wrapper, key, value, isArray);
                return true;
            }
        }

        return false;
    }

    bool JsonTableParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string&)
    {
        wrapper;
        Cleanup();
        return false;
    }

    void JsonTableParseHelper::HandleIntegers(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        auto& datum = PrimitiveHandler(wrapper, key, isArray, Datum::DatumTypes::Integer);
        if (datum._isExternal)
        {
            Context& currentContext = _contexts.Top();
            datum.Set(value.asInt(), currentContext._index);

            if (currentContext._arraySize != 0)
            {
                ++currentContext._index;
            }
        }
        else
        {
            datum.PushBack(value.asInt());
        }
    }

    void JsonTableParseHelper::HandleFloats(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {

        auto& datum = PrimitiveHandler(wrapper, key, isArray, Datum::DatumTypes::Float);
        if (datum._isExternal)
        {
            Context& currentContext = _contexts.Top();
            datum.Set(value.asFloat(), currentContext._index);

            if (currentContext._arraySize != 0)
            {
                ++currentContext._index;
            }
        }
        else
        {
            datum.PushBack(value.asFloat());
        }
    }

    void JsonTableParseHelper::HandleStrings(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        auto& datum = PrimitiveHandler(wrapper, key, isArray, Datum::DatumTypes::String);
        Context& currentContext = _contexts.Top();
        if (key == "Expression" && currentContext._className == "ActionExpression")
        {
            ExpressionParser expressionParser{};
            ActionExpression* actionExpression = static_cast<ActionExpression*>(currentContext._currentScope);
            expressionParser.ParseExpression(value.asString(), actionExpression);
            actionExpression->AcceptOutput(std::move(expressionParser.Output()));
        }
        if (datum._isExternal)
        {
            datum.Set(value.asString(), currentContext._index);

            if (currentContext._arraySize != 0)
            {
                ++currentContext._index;
            }
        }
        else
        {
            datum.PushBack(value.asString());
        }
    }

    void JsonTableParseHelper::HandleVectors(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        Context & currentContext = _contexts.Top();
        if ((key == "Expression" && currentContext._className == "ActionExpression"))
        {
            HandleStrings(wrapper, key, value, isArray);
            return;
        }

        auto& datum = PrimitiveHandler(wrapper, key, isArray, Datum::DatumTypes::Vector);
        if (datum._isExternal)
        {
            datum.SetFromString(value.asString(), currentContext._index);

            if (currentContext._arraySize != 0)
            {
                ++currentContext._index;
            }
        }
        else
        {
            datum.PushBackFromString(value.asString());
        }
    }

    void JsonTableParseHelper::HandleMatrices(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArray)
    {
        Context& currentContext = _contexts.Top();
        if ((key == "Expression" && currentContext._className == "ActionExpression"))
        {
            HandleStrings(wrapper, key, value, isArray);
            return;
        }

        auto& datum = PrimitiveHandler(wrapper, key, isArray, Datum::DatumTypes::Matrix);
        if (datum._isExternal)
        {
            datum.SetFromString(value.asString(), currentContext._index);

            if (currentContext._arraySize != 0)
            {
                ++currentContext._index;
            }
        }
        else
        {
            datum.PushBackFromString(value.asString());
        }
    }

    Datum& JsonTableParseHelper::PrimitiveHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, [[maybe_unused]] bool isArray, Datum::DatumTypes type)
    {
        JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(&wrapper);

        if (_contexts.IsEmpty() && rawWrapper->_scope == nullptr)
        {
            rawWrapper->_scope = std::make_unique<Scope>();
            _contexts.Push(Context{ rawWrapper->_scope.get(),"Root", "NA", 0});
        }

        assert(!_contexts.IsEmpty());
        return _contexts.Top()._currentScope->Append(key, type);
    }

    void JsonTableParseHelper::HandleObject(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, [[maybe_unused]] Json::Value& value, [[maybe_unused]] bool isArray)
    {
        JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(&wrapper);

        bool isEmpty{ _contexts.IsEmpty() };
        if (!isEmpty)
        {
            Context& currentContext = _contexts.Top();
            if (currentContext._arraySize > 0)
            {
                _contexts.Push(Context{ currentContext._currentScope, key, "NA", 0, true});
                return;
            }
        }

        Scope* scope{ nullptr };
        std::unique_ptr<Scope> product{ nullptr };
        std::string variableName{key};
        std::size_t iterator = key.find(" ");
        std::string parsedClassName{ "NA" };
        if (iterator != key.npos)
        {
            std::string className{ key.substr(0, iterator)};
            variableName = std::string{ key.substr(iterator + 1, key.npos) };
            product = Factory<Scope>::Create(className);
            parsedClassName = className;
        }

        if (product == nullptr)
        {
            scope = new Scope{};
        }
        else
        {
            scope = product.release();
        }

        if (!isEmpty && _contexts.Top()._isInArray)
        {
            _contexts.Top()._currentScope->Adopt(*scope, _contexts.Top()._contextName);
            _contexts.Push(Context{ scope, key, parsedClassName, 0});
            return;
        }

        if (_contexts.IsEmpty() && rawWrapper->_scope == nullptr)
        {
            rawWrapper->_scope = std::shared_ptr<Scope>{ scope };
            _contexts.Push(Context{ rawWrapper->_scope.get(),variableName, parsedClassName, 0 });
        }
        else
        {
            assert(!_contexts.IsEmpty());
            _contexts.Top()._currentScope->Adopt(*scope, variableName);
            _contexts.Push(Context{ scope, key, parsedClassName, 0 });
        }
    }

    void JsonTableParseHelper::HandleArray(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, [[maybe_unused]] Json::Value& value, [[maybe_unused]] bool isArray)
    {
        JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(&wrapper);

        if (_contexts.IsEmpty() && rawWrapper->_scope == nullptr)
        {
            rawWrapper->_scope = std::make_unique<Scope>();
            _contexts.Push(Context{ rawWrapper->_scope.get(),"Root", "NA", 0});
        }

        assert(!_contexts.IsEmpty());
        Scope* currentScope = _contexts.Top()._currentScope;
        if (value[0].isObject())
        {
            currentScope->Append(key, Datum::DatumTypes::Table);
        }
        else
        {
            currentScope->Append(key);
        }
        _contexts.Push(Context{ currentScope, key, "NA", value.size()});
    }

    bool JsonTableParseHelper::IsArray(Json::Value& value)
    {
        return value.isArray();
    }

    bool JsonTableParseHelper::IsInteger(Json::Value& value)
    {
        return value.isInt();
    }

    bool JsonTableParseHelper::IsVector(Json::Value& value)
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

    bool JsonTableParseHelper::IsMatrix(Json::Value& value)
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

    bool JsonTableParseHelper::IsString(Json::Value& value)
    {
        return value.isString();
    }
    bool JsonTableParseHelper::IsObject(Json::Value& value)
    {
        return value.isObject();
    }

    bool JsonTableParseHelper::IsFloat(Json::Value& value)
    {
        return value.isDouble();
    }
    JsonTableParseHelper::Wrapper::Wrapper() :
        FieaGameEngine::JsonParseCoordinator::Wrapper{}
    {

    }

    void JsonTableParseHelper::Wrapper::DecrementDepth()
    {
        FieaGameEngine::JsonParseCoordinator::Wrapper::DecrementDepth();
        if (currentHelper != nullptr)
        {
            currentHelper->_contexts.Pop();
        }
    }
    JsonTableParseHelper::Context::Context(Scope* scope, const std::string& name, const std::string& className, std::size_t arraySize, bool isInArray) :
        _currentScope{ scope }, _contextName{ name }, _className{ className }, _arraySize { arraySize }, _isInArray{ isInArray }
    {
    }
}