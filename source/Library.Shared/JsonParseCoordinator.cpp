#include "pch.h"
#include "JsonParseCoordinator.h"
#include <fstream>
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseCoordinator::Wrapper);

	void FieaGameEngine::JsonParseCoordinator::Wrapper::IncrementDepth()
	{
		++_depth;
	}

	void FieaGameEngine::JsonParseCoordinator::Wrapper::DecrementDepth()
	{
		--_depth;
	}

	JsonParseCoordinator* FieaGameEngine::JsonParseCoordinator::Wrapper::GetJsonParseCoordinator()
	{
		return _coordinator;
	}

	std::size_t FieaGameEngine::JsonParseCoordinator::Wrapper::Depth() const
	{
		return _depth;
	}

	JsonParseCoordinator::Wrapper::~Wrapper()
	{
		Cleanup();
	}

	void FieaGameEngine::JsonParseCoordinator::Wrapper::Initialize()
	{
		_depth = 0;
	}

	void JsonParseCoordinator::Wrapper::Cleanup()
	{

	}

	JsonParseCoordinator::JsonParseCoordinator(std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper) :
		_wrapper{ wrapper }
	{
		assert(_wrapper != nullptr);
		_wrapper->SetJsonParseCoordinator(this);
	}

	void JsonParseCoordinator::Wrapper::SetJsonParseCoordinator(JsonParseCoordinator* instance)
	{
		assert(instance != nullptr);
		_coordinator = instance;
	}

	std::shared_ptr<FieaGameEngine::JsonParseCoordinator> JsonParseCoordinator::Clone() const
	{
		auto coordinator = std::shared_ptr<FieaGameEngine::JsonParseCoordinator>(new JsonParseCoordinator{ _wrapper->Create() });

		for (auto& helper : _helpers)
		{
			coordinator->AddHelper(helper->Create());
		}

		return coordinator;
	}

	void FieaGameEngine::JsonParseCoordinator::AddHelper(std::shared_ptr<IJsonParseHelper> helper)
	{
		assert(helper.get() != nullptr);
		if (_helpers.Find(helper) == _helpers.end())
		{
			helper->Initialize();
			_helpers.PushBack(helper);
		}
	}

	void JsonParseCoordinator::RemoveHelper(std::shared_ptr<IJsonParseHelper> helper)
	{
		_helpers.Remove(helper);
	}

	void FieaGameEngine::JsonParseCoordinator::DeserializeObject(const std::string& string)
	{
		std::istringstream stream{ string };
		DeserializeObject(stream);
	}

	void JsonParseCoordinator::DeserializeObjectFromFile(const std::string& fileName)
	{
		std::ifstream file(fileName);
		assert(file.good());
		DeserializeObject(file);
	}

	void JsonParseCoordinator::DeserializeObject(std::istream& stream)
	{
		Json::Value value;
#pragma warning(push)
#pragma warning(disable:4996)
		reader.parse(stream, value);
		ParseMembers(value);
#pragma warning(pop)
	}

	void JsonParseCoordinator::Initialize()
	{
		_wrapper->Initialize();
		for (auto& helper : _helpers)
		{
			helper->Initialize();
		}
	}

	std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonParseCoordinator::GetWrapper() const
	{
		return _wrapper;
	}

	void JsonParseCoordinator::SetWrapper(std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper)
	{
		_wrapper = wrapper;
	}

	void JsonParseCoordinator::ParseMembers(Json::Value value)
	{
		auto keys{ value.getMemberNames() };
		for (auto& key : keys)
		{
			Parse(key, value[key]);
		}
	}

	void JsonParseCoordinator::ParseArrayElements(const std::string& arrayName, Json::Value value)
	{
		for (auto& element : value)
		{
			Parse(arrayName, element);
		}
	}

	void JsonParseCoordinator::Parse(const std::string& key, Json::Value value)
	{
		bool isArray = value.isArray();
		if (value.isObject())
		{
			_wrapper->IncrementDepth();
		}

		for (auto& helper : _helpers)
		{
			if (helper->StartHandler(*_wrapper, key, value, isArray))
			{
				if (isArray)
				{
					_wrapper->IncrementDepth();
					ParseArrayElements(key, value);
					_wrapper->DecrementDepth();
				}
				else if (value.isObject())
				{
					ParseMembers(value);
					_wrapper->DecrementDepth();
				}
				helper->EndHandler(*_wrapper, key);
			}
		}
	}

}
