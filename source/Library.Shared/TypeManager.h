#pragma once
#include "Datum.h"
#include "HashMap.h"

namespace FieaGameEngine
{
	struct Signature final
	{
		using Type = Datum::DatumTypes;
		std::string _name{};
		Type _type{ Type::Unknown };
		std::size_t _size{};
		std::size_t _offset{};

		Signature(const std::string& name, Type type, std::size_t size, std::size_t offset) :
			_name{ name }, _type{ type }, _size{ size }, _offset{ offset }
		{

		}
		~Signature() = default;
	};

	class TypeManager final
	{
	public:
		inline static TypeManager* Instance()
		{
			return _instance;
		}

		inline static void CreateInstance()
		{
			assert(_instance == nullptr);
			if (_instance == nullptr)
			{
				_instance = new TypeManager;
			}
		}

		inline static void DestroyInstance()
		{
			delete _instance;
		}

		inline [[nodiscard]] Vector<Signature>& GetSignatureForTypeID(std::size_t typeId)
		{
			auto position = _signatures.Find(typeId);
			assert(position != _signatures.end());
			return position->second;
		}

		inline void AddSignature(std::size_t typeId, const Vector<Signature>& signatures)
		{
			_signatures.Emplace(typeId, signatures);
		}

	private:
		inline static TypeManager* _instance{ nullptr };
		HashMap<std::size_t, Vector<Signature>> _signatures;
	};
}