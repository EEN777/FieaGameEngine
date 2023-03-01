#include "pch.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{

	}

	Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(AttributedFoo, externalInteger) },
			{ "ExternalFloat", Datum::DatumTypes::Float, 1, offsetof(AttributedFoo, externalFloat) },
			{ "ExternalString", Datum::DatumTypes::String, 1, offsetof(AttributedFoo, externalString) },
			{ "ExternalVector", Datum::DatumTypes::Vector, 1, offsetof(AttributedFoo, externalVector) },
			{ "ExternalMatrix", Datum::DatumTypes::Matrix, 1, offsetof(AttributedFoo, externalMatrix) },
			{ "ExternalIntegerArray", Datum::DatumTypes::Integer, arraySize, offsetof(AttributedFoo, externalIntegerArray) },
			{ "ExternalFloatArray", Datum::DatumTypes::Float, arraySize, offsetof(AttributedFoo, externalFloatArray) },
			{ "ExternalStringArray", Datum::DatumTypes::String, arraySize, offsetof(AttributedFoo, externalStringArray) },
			{ "ExternalVectorArray", Datum::DatumTypes::Vector, arraySize, offsetof(AttributedFoo, externalVectorArray) },
			{ "ExternalMatrixArray", Datum::DatumTypes::Matrix, arraySize, offsetof(AttributedFoo, externalMatrixArray) },
			{ "NestedScope", Datum::DatumTypes::Table, 0, 0 },
			{ "NestedScopeArray", Datum::DatumTypes::Table, arraySize, 0}
		};
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		rhs;
		return false;
	}

	std::string AttributedFoo::ToString() const
	{
		return externalString;
	}

}