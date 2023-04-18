#include "pch.h"
#include "Monster.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(Monster);

	Monster::Monster() :
		GameObject(TypeIdClass())
	{
	}

	void UnitTests::Monster::Update(const GameTime& gameTime)
	{
		GameObject::Update(gameTime);
		_hasUpdated = true;
	}

	Vector<Signature> Monster::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(GameObject, _name) },
			{ "Position", Datum::DatumTypes::Vector, 1, offsetof(GameObject, _position) },
			{ "Rotation", Datum::DatumTypes::Vector, 1, offsetof(GameObject, _rotation) },
			{ "Scale", Datum::DatumTypes::Vector, 1, offsetof(GameObject, _scale) },
			{ "Children", Datum::DatumTypes::Table, 0, 0 },
			{ "Health", Datum::DatumTypes::Integer, 1, offsetof(Monster, _health) },
			{ "Damage", Datum::DatumTypes::Integer, 1, offsetof(Monster, _damage) },
			{ "Speed", Datum::DatumTypes::Float, 1, offsetof(Monster, _speed) },
			{ "Vector0", Datum::DatumTypes::Vector, 1, offsetof(Monster, _vector0) },
			{ "Vector1", Datum::DatumTypes::Vector, 1, offsetof(Monster, _vector1) },
			{ "Vector2", Datum::DatumTypes::Vector, 1, offsetof(Monster, _vector2) },
			{ "Vector3", Datum::DatumTypes::Vector, 1, offsetof(Monster, _vector3) },
			{ "Matrix0", Datum::DatumTypes::Matrix, 1, offsetof(Monster, _matrix0) },
			{ "Matrix1", Datum::DatumTypes::Matrix, 1, offsetof(Monster, _matrix1) },
			{ "Matrix2", Datum::DatumTypes::Matrix, 1, offsetof(Monster, _matrix2) },
			{ "Matrix3", Datum::DatumTypes::Matrix, 1, offsetof(Monster, _matrix3) },
			{ "Species", Datum::DatumTypes::String, 1, offsetof(Monster, _species) },
		};
	}

	gsl::owner<Monster*> Monster::Clone() const
	{
		return new Monster(*this);
	}

	std::string Monster::ToString() const
	{
		return _name;
	}
	void Monster::IncreaseHealth(int amount)
	{
		_health += amount;
	}
	void Monster::DecreaseHealth(int amount)
	{
		_health -= amount;
	}
}
