#pragma once
#include "Attributed.h"

namespace UnitTests
{
	class AttributedFoo final : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);

	public:
		static const std::size_t arraySize{ 5 };

		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		~AttributedFoo() = default;

		int externalInteger{};
		float externalFloat{};
		std::string externalString{};
		glm::vec4 externalVector{};
		glm::mat4 externalMatrix{};

		int externalIntegerArray[arraySize]{};
		float externalFloatArray[arraySize]{};
		std::string externalStringArray[arraySize]{};
		glm::vec4 externalVectorArray[arraySize]{};
		glm::mat4 externalMatrixArray[arraySize]{};

		gsl::owner<AttributedFoo*> Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		

	};
}