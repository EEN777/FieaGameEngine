#include "Attributed.h"
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(RTTI::IdType rttiType)
	{
		Populate(rttiType);
	}

	Attributed::Attributed(const Attributed& other) :
		Scope{ other }
	{
		(*this)["this"s] = this;
		SetExternalStorage(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope{ std::move(other) }
	{
		(*this)["this"s] = this;
		SetExternalStorage(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		Scope::operator=(other);
		(*this)["this"s] = this;
		SetExternalStorage(other.TypeIdInstance());
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		Scope::operator=(std::move(other));
		(*this)["this"s] = this;
		SetExternalStorage(other.TypeIdInstance());
		return *this;
	}

	bool Attributed::IsAttribute(const std::string& attributeName)
	{
		return Find(attributeName) == nullptr ? false : true;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& attributeName) const
	{
		return CompareKeys(attributeName, PrescribedAttributes());
	}

	bool Attributed::IsAuxillaryAttribute(const std::string& attributeName) const
	{
		return CompareKeys(attributeName, AuxillaryAttributes());
	}

	Datum& Attributed::AppendAuxillaryAttribute(const std::string& attributeName, Datum::DatumTypes type)
	{
		assert(!IsPrescribedAttribute(attributeName));
		return Append(attributeName, type);
	}

	Attributed::Indices Attributed::Attributes() const
	{
		return { 1, _orderVector.Size() };
	}

	Attributed::Indices Attributed::PrescribedAttributes() const
	{
		return { 1, TypeManager::Instance()->GetSignatureForTypeID(TypeIdInstance()).Size() + 1 };
	}

	Attributed::Indices Attributed::PrescribedAttributes(RTTI::IdType rttiType) const
	{
		return { 1, TypeManager::Instance()->GetSignatureForTypeID(rttiType).Size() + 1 };
	}

	Attributed::Indices Attributed::AuxillaryAttributes() const
	{
		return { TypeManager::Instance()->GetSignatureForTypeID(TypeIdInstance()).Size() + 1, _orderVector.Size() };
	}

	void Attributed::Clear()
	{
		Scope::Clear();
		Populate(TypeIdInstance());
	}

	void Attributed::Populate(RTTI::IdType rttiType)
	{
		auto typeManager = TypeManager::Instance();
		assert(typeManager != nullptr);
		auto& signatures = typeManager->GetSignatureForTypeID(rttiType);
		(*this)["this"s] = this;
		for (auto& signature : signatures)
		{
			Datum& member = Append(signature._name, signature._type);
			if (signature._type != Datum::DatumTypes::Table)
			{
				member.SetStorage(reinterpret_cast<std::byte*>(this) + signature._offset, signature._size);
			}
			else
			{
				member.Reserve(signature._size);
			}
		}
	}

	void Attributed::SetExternalStorage(RTTI::IdType rttiType)
	{
		auto typeManager = TypeManager::Instance();
		assert(typeManager != nullptr);
		auto& signatures = typeManager->GetSignatureForTypeID(rttiType);
		auto indices = PrescribedAttributes(rttiType);

		for (auto index{ indices.first }; index != indices.second; ++index)
		{
			if (_orderVector[index]->second.Type() != Datum::DatumTypes::Table)
			{
				auto currentSignature = signatures[index - 1];
				_orderVector[index]->second.SetStorage(reinterpret_cast<std::byte*>(this) + currentSignature._offset, currentSignature._size);
			}
		}
	}

	bool Attributed::CompareKeys(const std::string& attributeName, Indices indices) const
	{
		for (auto position{ indices.first }; position != indices.second; ++position)
		{
			if (attributeName == (_orderVector[position])->first)
			{
				return true;
			}
		}
		return false;
	}
}