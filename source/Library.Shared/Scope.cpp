#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(std::size_t size) 
	{
		_orderVector.Reserve(size);
	}

	Scope::Scope(const Scope& other)
	{
		CopyHelper(other, *this);
	}

	Scope::Scope(Scope&& other) noexcept :
		_orderVector{ std::move(other._orderVector) }, _table{ std::move(other._table) }, _size{ other._size }
	{
		if (this != &other)
		{
			other._size = 0;
			if (other._parent != nullptr)
			{
				Scope* original = other.Orphan();
				delete original;
			}
			ReparentImmediateFamily();
		}
	}

	Scope& Scope::operator=(const Scope& other)
	{
		assert(!IsDescendantOf(&other) && !IsAncestorOf(&other));
		if (this != &other && !IsDescendantOf(&other) && !IsAncestorOf(&other))
		{
			Clear();
			CopyHelper(other, *this);
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		assert(!IsDescendantOf(&other) && !IsAncestorOf(&other));
		if (this != &other && !IsDescendantOf(&other) && !IsAncestorOf(&other))
		{
			Scope* original = nullptr;
			Scope* otherParent = other.GetParent();
			if (otherParent != nullptr)
			{
				original = otherParent->Orphan(&other);
			}
			Clear();
			_orderVector = std::move(other._orderVector);
			_table = std::move(other._table);
			_size = other._size;
			other._size = 0;
			ReparentImmediateFamily();
			delete original;
		}
		return *this;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (this == &rhs)
		{
			return true;
		}

		else
		{
			if (_size == rhs._size)
			{
				std::size_t position{ 0 };
				for (auto& pair : _orderVector)
				{
					if (*pair != *rhs._orderVector[position])
					{
						return false;
					}
					++position;
				}
			}

			else
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if ( rhs != nullptr && rhs->Is(Scope::TypeIdClass()))
		{
			return *this == *(reinterpret_cast<const Scope*>(rhs));
		}
		else
		{
			return false;
		}
	}

	std::string Scope::ToString() const
	{
		if (_parent != nullptr)
		{
			return std::string{ "NestedScope" };
		}
		else
		{
			return std::string{ "Scope" };
		}
	}
	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope{ *this };
	}

	Scope::Scope(Scope* parent) :
		_parent{ parent }
	{

	}

	Scope::~Scope()
	{
		if (_parent != nullptr)
		{
			_parent->Orphan(this);
		}

		Clear();
	}

	Datum* Scope::Search(const std::string& name)
	{
		Datum* foundDatum = Find(name);
		if (foundDatum == nullptr)
		{
			Scope* parent = _parent;
			while (parent != nullptr)
			{
				foundDatum = parent->Find(name);
				if (foundDatum != nullptr)
				{
					break;
				}
				parent = parent->GetParent();
			}
		}
		return foundDatum;
	}

	Datum* Scope::Search(const std::string& name, Scope*& foundScope)
	{
		Datum* foundDatum = Find(name);
		Scope* parent = _parent;
		foundScope = this;
		if (foundDatum == nullptr)
		{
			foundScope = nullptr;
			while (parent != nullptr)
			{
				foundDatum = parent->Find(name);
				if (foundDatum != nullptr)
				{
					foundScope = parent;
					break;
				}
				parent = parent->GetParent();
			}
		}
		return foundDatum;
	}

	Datum& Scope::Append(const std::string& name, Datum::DatumTypes type)
	{
		auto position{ _table.Find(name) };
		if (position == _table.end())
		{
			auto newPosition = _table.Emplace(name, Datum{ type });
			_orderVector.EmplaceBack(&(*newPosition));
			++_size;
			return _orderVector.Back()->second;
		}
		else
		{
			return position->second;
		}
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](std::size_t index)
	{
		assert(index < _orderVector.Size());
		return _orderVector[index]->second;
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		assert(!IsDescendantOf(&child));
		if (!IsDescendantOf(&child))
		{
			Datum& item = Append(name, Datum::DatumTypes::Table);

			if (child.GetParent() != nullptr)
			{
				child.Orphan();
			}

			assert(item.Type() == Datum::DatumTypes::Table);

			item.PushBack(&child);

			child.Reparent(this);
		}
	}

	Scope* Scope::Orphan()
	{
		Scope* temp = nullptr;

		if (_parent != nullptr)
		{
			temp = _parent->Orphan(this);
		}

		return temp;
	}

	Scope* Scope::Orphan(Scope* child)
	{
		Scope* temp = nullptr;

		std::pair<Datum*, std::size_t> result = FindContainedScope(child);

		if (result.first != nullptr)
		{
			temp = result.first->GetAsScope(result.second);
			result.first->RemoveAt(result.second);
		}

		return temp;
	}

	bool Scope::IsDescendantOf(const Scope* scope) const
	{
		if (_parent != nullptr)
		{
			if (_parent == scope)
			{
				return true;
			}
			else
			{
				return _parent->IsDescendantOf(scope);
			}
		}
		else
		{
			return false;
		}
	}

	bool Scope::IsAncestorOf(const Scope* scope) const
	{
		assert(scope != nullptr);
		return scope->IsDescendantOf(this);
	}
	std::pair<Datum*, std::size_t> Scope::FindContainedScope(const Scope* scope)
	{
		for (auto& item : _orderVector)
		{
			Datum& currentDatum = item->second;
			if (currentDatum.Type() == Datum::DatumTypes::Table)
			{
				for (std::size_t position{ 0 }; position < currentDatum.Size(); ++position)
				{
					if (scope == currentDatum.GetAsScope(position))
					{
						return std::pair(&currentDatum, position);
					}
				}
			}
		}
		return std::pair(nullptr, 0);
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		Datum& item = Append(name, Datum::DatumTypes::Table);

		assert(item.Type() == Datum::DatumTypes::Table);

		Scope* scope = new Scope{ this };
		item.PushBack(scope);
		return *scope;
	}

	Datum* Scope::Find(const std::string& name)
	{
		auto position = _table.Find(name);
		return position == _table.end() ? nullptr : &(position->second);
	}

	void Scope::Clear()
	{
		for (auto& item : _orderVector)
		{
			Datum& currentDatum = item->second;
			if (currentDatum.Type() == Datum::DatumTypes::Table)
			{
				std::size_t datumSize{ currentDatum.Size() };
				for (std::size_t position{ 0 }; position < datumSize; ++position)
				{
					delete currentDatum.GetAsScope();
				}
			}
		}

		_orderVector.Clear();
		_table.Clear();
		_size = 0;
	}

	inline void Scope::CopyHelper(const Scope& sourceScope, Scope& destinationScope)
	{
		for (auto& item : sourceScope._orderVector)
		{
			Datum& currentDatum = item->second;
			if (item->second.Type() != Datum::DatumTypes::Table)
			{
				Datum& addedDatum = destinationScope.Append(item->first, currentDatum.Type());
				addedDatum = currentDatum;
			}
			else
			{
				Datum& addedDatum{ destinationScope.Append(item->first, Datum::DatumTypes::Table) };
				for (std::size_t index{ 0 }; index < currentDatum.Size(); ++index)
				{
					addedDatum.PushBack(currentDatum.GetAsScope(index)->Clone());
					addedDatum.GetAsScope(index)->Reparent(this);
				}
			}
		}
	}

	void Scope::ReparentImmediateFamily()
	{
		for (auto& pair : _orderVector)
		{
			if (pair->second.Type() == Datum::DatumTypes::Table)
			{
				for (std::size_t index{ 0 }; index < pair->second.Size(); ++index)
				{
					pair->second.GetAsScope(index)->Reparent(this);
				}
			}
		}
	}

}