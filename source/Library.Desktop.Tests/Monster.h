#pragma once
#include "GameObject.h"
#include "Factory.h"

namespace UnitTests
{
	class Monster final : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(Monster, GameObject);

	public:
		Monster();
		Monster(const Monster&) = default;
		Monster(Monster&&) = default;
		Monster& operator=(const Monster&) = default;
		Monster& operator=(Monster&&) = default;
		~Monster() = default;
		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

		int _health{};
		int _damage{};
		float _speed{};
		glm::vec4 _vector0{};
		glm::vec4 _vector1{};
		glm::vec4 _vector2{};
		glm::vec4 _vector3{};
		glm::mat4 _matrix0{};
		glm::mat4 _matrix1{};
		glm::mat4 _matrix2{};
		glm::mat4 _matrix3{};

		std::string _species{};

		bool _hasUpdated{ false };

		gsl::owner<Monster*> Clone() const override;
		std::string ToString() const override;

		void IncreaseHealth(int);
		void DecreaseHealth(int);

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Monster, FieaGameEngine::Scope);
}