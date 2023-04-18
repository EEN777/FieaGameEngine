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
		Signature(const Signature&) = default;
	};

	class TypeManager final
	{
	public:
		/// <summary>
		/// Retrieves the current instance of the TypeManager. Returns nullptr if one hasn't been made.
		/// </summary>
		/// <returns>The single instance of the TypeManager.</returns>
		inline static TypeManager* Instance()
		{
			return _instance;
		}
		/// <summary>
		/// Creates an instance of a TypeManager if one does not currently exist.
		/// </summary>
		inline static void CreateInstance()
		{
			assert(_instance == nullptr);
			if (_instance == nullptr)
			{
				_instance = new TypeManager;
			}
		}
		/// <summary>
		/// Destroys the current instance of the TypeManager.
		/// </summary>
		inline static void DestroyInstance()
		{
			delete _instance;
			_instance = nullptr;
		}
		/// <summary>
		/// Searches a container of type signatures for the matching RTTI Id key and returns a vector of signatures for each member.
		/// </summary>
		/// <param name="typeId">The RTTI TypeId you are searching for.</param>
		/// <returns>The vector of signatures for the corresponding RTTI type.</returns>
		inline [[nodiscard]] Vector<Signature>& GetSignatureForTypeID(RTTI::IdType typeId)
		{
			auto position = _signatures.Find(typeId);
			assert(position != _signatures.end());
			return position->second;
		}
		/// <summary>
		/// Adds a signature of the corresponding RTTI IdType that was passed in into the signatures hashmap and adds the given vector into it at that key.
		/// </summary>
		/// <param name="typeId">The RTTI IdType for the class.</param>
		/// <param name="signatures">A Vector of Signatures.</param>
		inline void AddSignature(RTTI::IdType typeId, const Vector<Signature>& signatures)
		{
			_signatures.Emplace(typeId, signatures);
		}

	private:
		inline static TypeManager* _instance{ nullptr };
		HashMap<std::size_t, Vector<Signature>> _signatures;
	};
}