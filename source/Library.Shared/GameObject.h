#pragma once
#include "Attributed.h"
#include "Action.h"
#include "Factory.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	protected:
		/// <summary>
		/// Constructor that can be called by inherited classes in order to correctly fill out and map a scope to a class' data.
		/// </summary>
		/// <param name="ID">The RTTI Id Type for the class.</param>
		GameObject(IdType id);
	public:
		/// <summary>
		/// Default Constructor for GameObject that invokes the attributed Constructor using GameObject's RTTI TypeIDClass function. 
		/// </summary>
		GameObject();
		GameObject(const GameObject&) = default;
		GameObject(GameObject&&) = default;
		GameObject& operator=(const GameObject&) = default;
		GameObject& operator=(GameObject&&) = default;
		/// <summary>
		/// Constructor marked virtual for inheritance.
		/// </summary>
		virtual ~GameObject();
		/// <summary>
		/// An Update function meant to be called every frame. This call also calls Update on all child GameObjects.
		/// </summary>
		/// <param name="gameTime"></param>
		virtual void Update(const GameTime& gameTime);
		Datum& GetChildren();
		Datum& Actions();
		Action* CreateAction(const std::string& className);

		GameObject* CreateChild(const std::string& className);

		std::string _name{};

		glm::vec4 _position{};
		glm::vec4 _rotation{};
		glm::vec4 _scale{};

		gsl::owner<GameObject*> Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static Vector<Signature> Signatures();
	};

	ConcreteFactory(GameObject, Scope);
}