#pragma once
#include <cstddef>
#include <string>
#include <json/json.h>
#include "Vector.h"
#include "Stack.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;
	class JsonParseCoordinator final
	{
	public:
		class Wrapper : public RTTI
		{
			RTTI_DECLARATIONS(Wrapper, RTTI);
			friend JsonParseCoordinator;

			std::size_t _depth{};
			JsonParseCoordinator* _coordinator{ nullptr };
			void SetJsonParseCoordinator(JsonParseCoordinator*);

		protected:
			Wrapper() = default;
		
		public:
			virtual void Initialize();
			virtual void Cleanup();
			virtual std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const = 0;
			JsonParseCoordinator* GetJsonParseCoordinator();
			virtual void IncrementDepth();
			virtual void DecrementDepth();
			std::size_t Depth() const;
			virtual ~Wrapper();
		};

	private:
		std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> _wrapper{ nullptr };
		Vector<std::shared_ptr<IJsonParseHelper>> _helpers{};
#pragma warning(push)
#pragma warning(disable:4996)
		Json::Reader reader{};
#pragma warning(pop)

	public:
#pragma warning(push)
#pragma warning(disable:4996)
		JsonParseCoordinator(std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper>);
		JsonParseCoordinator(const JsonParseCoordinator&) = delete;
		JsonParseCoordinator(JsonParseCoordinator&&) = default;
		JsonParseCoordinator& operator=(const JsonParseCoordinator&) = delete;
		JsonParseCoordinator& operator=(JsonParseCoordinator&&) = default;
		~JsonParseCoordinator() = default;
#pragma warning(pop)
		std::shared_ptr<FieaGameEngine::JsonParseCoordinator> Clone() const;
		void AddHelper(std::shared_ptr<IJsonParseHelper>);
		void RemoveHelper(std::shared_ptr<IJsonParseHelper>);
		void DeserializeObject(const std::string&);
		void DeserializeObjectFromFile(const std::string&);
		void DeserializeObject(std::istream&);
		void Initialize();
		std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> GetWrapper() const;
		void SetWrapper(std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> wrapper);
	private:
		void ParseMembers(Json::Value);
		void ParseArrayElements(const std::string& arrayName, Json::Value value);
		void Parse(const std::string&, Json::Value);
	};

}
