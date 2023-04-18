#include "GameObject.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject);

	GameObject::GameObject() :
		Attributed(GameObject::TypeIdClass())
	{

	}

	GameObject::GameObject(IdType id) :
		Attributed(id)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Update(const GameTime& gameTime)
	{
		Datum& children = GetChildren();
		for (std::size_t index{ 0 }; index < children.Size(); ++index)
		{
			Scope* scope = children.GetAsScope(index);
			assert(scope != nullptr);
			assert(scope->Is(GameObject::TypeIdClass()));
			static_cast<GameObject*>(scope)->Update(gameTime);
		}

		Datum& actions = Actions();
		for (std::size_t index{ 0 }; index < actions.Size(); ++index)
		{
			Scope* scope = actions.GetAsScope(index);
			assert(scope != nullptr);
			assert(scope->Is(Action::TypeIdClass()));
			static_cast<Action*>(scope)->Update(gameTime);
		}
	}

	Datum& GameObject::GetChildren()
	{
		return Append("Children");
	}

	Datum& GameObject::Actions()
	{
		return Append("Actions");
	}

	Action* GameObject::CreateAction(const std::string& className)
	{
		Scope* action = Factory<Scope>::Create(className).release();
		//assert(action->Is(Action::TypeIdClass()));
		Adopt(*action, "Actions");
		return reinterpret_cast<Action*>(action);
	}

	GameObject* GameObject::CreateChild(const std::string& className)
	{
		Scope* gameObject = Factory<Scope>::Create(className).release();
		assert(gameObject->Is(GameObject::TypeIdClass()));
		Adopt(*gameObject, "Children");
		return reinterpret_cast<GameObject*>(gameObject);
	}

	Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(GameObject, _name) },
			{ "Position", Datum::DatumTypes::Vector, 1, offsetof(GameObject, _position) },
			{ "Rotation", Datum::DatumTypes::Vector, 1, offsetof(GameObject, _rotation) },
			{ "Scale", Datum::DatumTypes::Vector, 1, offsetof(GameObject, _scale) },
			{ "Children", Datum::DatumTypes::Table, 0, 0 },
			{ "Actions", Datum::DatumTypes::Table, 0, 0}
		};
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		return this == rhs;
	}

	std::string GameObject::ToString() const
	{
		return _name;
	}

}