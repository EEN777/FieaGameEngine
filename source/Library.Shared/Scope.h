#pragma once
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "gsl/gsl"

namespace FieaGameEngine
{
	/// <summary>
	/// A Scope is an RTTI instance that can be inherited from that is a table that can contain data and tables of other tables or data.
	/// </summary>
	class Scope : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Scope, FieaGameEngine::RTTI);
	public:
		using Pair_Type = std::pair<const std::string, Datum>;

	private:
		Vector<Pair_Type*> _orderVector{};
		HashMap<std::string, Datum> _table{};

		std::size_t _size{ 0 };
		Scope* _parent{ nullptr };

	public:
		/// <summary>
		/// The default constructgor for Scope. The scope will have no contents and no parent.
		/// </summary>
		Scope() = default;
		/// <summary>
		/// The Scope destructor. This destructor is virtual so that it can be overridden by inherited classes.
		/// </summary>
		virtual ~Scope();
		/// <summary>
		/// The copy constructor that takes in a const reference to another scope and copies its contents entirely.
		/// </summary>
		/// <param name="other">The other scope to copy from.</param>
		Scope(const Scope& other);
		/// <summary>
		/// The move constructor that takes in an rvalue reference to a Scope and pilfers its data. If the Scope is nested, the constructor will delete the previously heap allocated
		/// object that is being moved from.
		/// </summary>
		/// <param name="other">The other scope being moved.</param>
		Scope(Scope&& other) noexcept;
		/// <summary>
		/// The copy assignment operator that takes in a const reference to another scope and copies its contents entirely. This will clear the left hand scope before copying.
		/// </summary>
		/// <param name="other">The other scope to copy from.</param>
		/// <returns>A reference to the scope being copied into.</returns>
		Scope& operator=(const Scope& other);
		/// <summary>
		/// The move assignment operator that takes in an rvalue reference to a Scope and pilfers its data. If the Scope is nested, the operator will delete the previously heap allocated
		/// object that is being moved from.
		/// </summary>
		/// <param name="other">The other scope being moved.</param>
		/// <returns>A reference to the scope that is being moved into.</returns>
		Scope& operator=(Scope&& other) noexcept;
		/// <summary>
		/// A Comparison operator that determines whether or not the contents of two given scopes are equivalent. This is not a sameness test.
		/// </summary>
		/// <param name="rhs">The scope the lhs is being compared to.</param>
		/// <returns>Whether or not the two scopes are equivalent.</returns>
		[[nodiscard]] bool operator==(const Scope& rhs) const;
		/// <summary>
		/// A Comparison operator that determines whether or not the contents of two given scopes are not equivalent. This is not a sameness test.
		/// </summary>
		/// <param name="rhs">The scope the lhs is being compared to.</param>
		/// <returns>Whether or not the two scopes are not equivalent.</returns>
		[[nodiscard]] bool operator!=(const Scope& rhs) const;
		/// <summary>
		/// A virtual function that is overriden inherited from RTTI. Checks to see whether or not the RTTI pointer is indeed a scope and then invokes the comparison operator against them.
		/// </summary>
		/// <param name="rhs">The RTTI pointer that the scope is going to be compared against.</param>
		/// <returns>Whether or not the two items are equivalent.</returns>
		virtual bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// A Basic ToString method inherited from RTTI. This override returns a string based on whether or not the scope is nested.
		/// </summary>
		/// <returns>A string that describes the scope.</returns>
		virtual std::string ToString() const override;
		/// <summary>
		/// A virtual method that can be overridden that creates a clone of the scope on the stack and returns a gsl::owner of type Scope* to that newly constructed scope.
		/// </summary>
		/// <returns>A pointer to the newly allocated scope.</returns>
		virtual gsl::owner<Scope*> Clone() const;
		/// <summary>
		/// Allocates memory for the scope if scope size is already known.
		/// </summary>
		/// <param name="size">The number of entries in the scope table.</param>
		Scope(std::size_t size);
		/// <summary>
		/// Append attempts to find a value within the scope that corresponds to the given string, but adds a pair associated with the
		/// aforementioned string if one cannot be found. Retruns a reference to the new Datum.
		/// </summary>
		/// <param name="name">The string to search for.</param>
		/// <param name="type">The type that you wish for any newly created datum to be.</param>
		/// <returns>A reference to the found or newly created Datum.</returns>
		Datum& Append(const std::string& name, Datum::DatumTypes type = Datum::DatumTypes::Unknown);
		/// <summary>
		/// Searches the scope in order to find a string,Datum pair that corresponds to said string. If none is found, then nullptr is returned.
		/// </summary>
		/// <param name="name">The string to search for.</param>
		/// <returns>A Datum pointer.</returns>
		[[nodiscard]] Datum* Find(const std::string& name);
		/// <summary>
		/// Does the same work as Append, but also populates the Datum with a heap allocated scope.
		/// </summary>
		/// <param name="name">The string you wish to associate with the new scope.</param>
		/// <returns>A reference to the newly created scope.</returns>
		Scope& AppendScope(const std::string& name);
		/// <summary>
		/// Serves as a convenient wrapper to Append.
		/// </summary>
		/// <param name="name">The string you wish to create a new pair for.</param>
		/// <returns>A reference to the found or newly created Datum.</returns>
		Datum& operator[](const std::string& name);
		/// <summary>
		/// Returns a Datum corresponding to the order in which it was added.
		/// </summary>
		/// <param name="index">The index at which to grab from.</param>
		/// <returns>A Datum reference.</returns>
		[[nodiscard]] Datum& operator[](std::size_t index);
		/// <summary>
		/// Search looks up the hierarchy of scopes for the most closely nested Datum. Returns nullptr if not found.
		/// </summary>
		/// <param name="name">The string to look for.</param>
		/// <returns>A pointer to a the found Datum.</returns>
		[[nodiscard]] Datum* Search(const std::string& name);
		/// <summary>
		/// Search looks up the hierarchy of scopes for the most closely nested Datum. Returns nullptr if not found.
		/// An output parameter of type Scope*& can be passed in to find the owning Scope.
		/// </summary>
		/// <param name="name">The string to look for.</param>
		/// <param name="foundScope">The Scope pointer reference to put the found scope pointer into.</param>
		/// <returns>A pointer to the Datum that was found.</returns>
		[[nodiscard]] Datum* Search(const std::string& name, Scope*& foundScope);
		/// <summary>
		/// Returns the pointer to the owning parent. Returns nullptr if there is none.
		/// </summary>
		/// <returns>A pointer to the parent Scope.</returns>
		[[nodiscard]] inline Scope* GetParent();
		/// <summary>
		/// Adopt takes a scope reference and adds it into it's table at the given string.
		/// </summary>
		/// <param name="child">Scope that will be adopted.</param>
		/// <param name="name">The string to associate the scope with.</param>
		void Adopt(Scope& child, const std::string& name);
		/// <summary>
		/// Attempts to remove the calling scope from its parent.
		/// </summary>
		/// <returns>Returns a pointer to the parent.</returns>
		Scope* Orphan();
		/// <summary>
		/// Removes the passed in child from the scope.
		/// </summary>
		/// <param name="child">The scope to be orphaned.</param>
		/// <returns>A pointer to the owning scope.</returns>
		Scope* Orphan(Scope* child);
		/// <summary>
		/// Checks to see whether or not this scope is a descendant of another scope.
		/// </summary>
		/// <param name="other">Scope to check for.</param>
		/// <returns>Whether or not the scope is a desendant of the passed in scope.</returns>
		[[nodiscard]] bool IsDescendantOf(const Scope* other) const;
		/// <summary>
		/// Checks to see whether or not this scope is an ancestor of another scope.
		/// </summary>
		/// <param name="other">Scope to check for.</param>
		/// <returns>Whether or not the scope is an ancestor of the passed in scope.</returns>
		[[nodiscard]] bool IsAncestorOf(const Scope* other) const;
		/// <summary>
		/// Returns the size of the scope.
		/// </summary>
		/// <returns>The number of entries in the scope.</returns>
		[[nodiscard]] inline std::size_t Size();
		/// <summary>
		/// Checks whether or not the scope is empty.
		/// </summary>
		/// <returns>Whether or not the scope's size is set to 0;</returns>
		[[nodiscard]] inline bool IsEmpty();
		/// <summary>
		/// Returns a std::pair that contains a Datum* and std::size_t of the scope contained within the current scope.
		/// If it is not found nullptr,0 are returned.
		/// </summary>
		/// <param name="scope">The scope to search for.</param>
		/// <returns>A pair of a Datum pointer and a std::size_t representing the location of the found scope.</returns>
		[[nodiscard]] std::pair<Datum*, std::size_t> FindContainedScope(const Scope* scope);
		/// <summary>
		/// Clears all contents of the scope.
		/// </summary>
		void Clear();

	private:
		Scope(Scope* parent);
		inline void Reparent(Scope* parent);
		inline void CopyHelper(const Scope& sourceScope, Scope& destinationScope);
		void ReparentImmediateFamily();
	};
}

#include "Scope.inl"
