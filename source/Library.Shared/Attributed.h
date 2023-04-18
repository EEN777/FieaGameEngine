#pragma once
#include "Scope.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	class Attributed : public FieaGameEngine::Scope
	{
		using Indices = std::pair<std::size_t, std::size_t>;
		RTTI_DECLARATIONS(Attributed, Scope);

	protected:
		/// <summary>
		/// Creates a Attributed object based off a given RTTI IdType. This can only be invoked by derived classes. Uses a table lookup to create a scope that maps to the data of a class to provide a wrapper for said class. 
		/// </summary>
		/// <param name="rttiType">The RTTI instance identifier.</param>
		Attributed(RTTI::IdType rttiType);
		/// <summary>
		/// Populates a copied scope with the correct scope names and calls SetStorage on all prescribe attribute datums to retarget where the copied datums are looking.
		/// </summary>
		/// <param name="other">The derived class being copied from.</param>
		Attributed(const Attributed& other);
		/// <summary>
		/// Populates a moved scope with the correct scope names and calls SetStorage on all prescribe attribute datums to retarget where the copied datums are looking.
		/// </summary>
		/// <param name="other">The derived class being copied from.</param>
		Attributed(Attributed&& other) noexcept;
		/// <summary>
		/// Populates a copied scope with the correct scope names and calls SetStorage on all prescribe attribute datums to retarget where the copied datums are looking.
		/// </summary>
		/// <param name="other">The derived class being copied from.</param>
		Attributed& operator=(const Attributed& other);
		/// <summary>
		/// Populates a moved scope with the correct scope names and calls SetStorage on all prescribe attribute datums to retarget where the copied datums are looking.
		/// </summary>
		/// <param name="other">The derived class being copied from.</param>
		Attributed& operator=(Attributed&& other) noexcept;
		virtual ~Attributed() = default;

	public:
		Attributed() = delete;
		/// <summary>
		/// Takes a given std::string and checks all attributes for a match.
		/// </summary>
		/// <param name="attributeName">The name of the attribute being searched for.</param>
		/// <returns>Whether or not the given name is found.</returns>
		[[nodiscard]] bool IsAttribute(const std::string& attributeName);
		/// <summary>
		/// Takes a given std::string and checks all prescribed attributes for a match.
		/// </summary>
		/// <param name="attributeName">The name of the attribute being searched for.</param>
		/// <returns>Whether or not the given name is found.</returns>
		[[nodiscard]] bool IsPrescribedAttribute(const std::string& attributeName) const;
		/// <summary>
		/// Takes a given std::string and checks all auxillary attributes for a match.
		/// </summary>
		/// <param name="attributeName">The name of the attribute being searched for.</param>
		/// <returns>Whether or not the given name is found.</returns>
		[[nodiscard]] bool IsAuxillaryAttribute(const std::string& attributeName) const;
		/// <summary>
		/// Appends an extraneous member to an instatiated class with the given name.
		/// </summary>
		/// <param name="attributeName">The name of the member to add.</param>
		/// <param name="type">The datum type to creeate.</param>
		/// <returns>A reference to the newly appended attribute's datum.</returns>
		Datum& AppendAuxillaryAttribute(const std::string& attributeName, Datum::DatumTypes type = Datum::DatumTypes::Unknown);
		/// <summary>
		/// Returns the range in a std::pair of std::size_t of all attributes in the scope. These can be accessed easily using the [] operator.
		/// The second value serves as a functional end(), meaning that it is one past the last valid index.
		/// </summary>
		/// <returns>The range of values in the scope that are attributes.</returns>
		Indices Attributes() const;
		/// <summary>
		/// Returns the range in a std::pair of std::size_t of all prescribed attributes in the scope. These can be accessed easily using the [] operator.
		/// The second value serves as a functional end(), meaning that it is one past the last valid index.
		/// </summary>
		/// <returns>The range of values in the scope that are prescribed attributes.</returns>
		Indices PrescribedAttributes() const;
		/// <summary>
		/// Returns the range in a std::pair of std::size_t of all auxillary attributes in the scope. These can be accessed easily using the [] operator.
		/// The second value serves as a functional end(), meaning that it is one past the last valid index.
		/// </summary>
		/// <returns>The range of values in the scope that are auxillary attributes.</returns>
		Indices AuxillaryAttributes() const;
		/// <summary>
		/// Clears the underlying scope and then repopulates the scope with accurate attributes excepting any auxillary attributes.
		/// </summary>
		void Clear() override;
	private:
		Indices PrescribedAttributes(RTTI::IdType rttiType) const;
		/// <summary>
		/// Fills the base scope with the correct attributes and sets the added datums storage for prescribed attributes.
		/// </summary>
		/// <param name="rttiType">The RTTI IdType to use in the lookup table.</param>
		void Populate(RTTI::IdType rttiType);
		/// <summary>
		/// A similar method to SetExternalStorage, except that it traverses the order vector so that it is instead a constant timer operation to modify the underlying datums.
		/// </summary>
		/// <param name="rttiType">The RTTI IdType to use in the lookup table.</param>
		void SetExternalStorage(RTTI::IdType rttiType);
		/// <summary>
		/// A Helper function used to copare a set of keys. Used in IsAttribute, IsPrescribedAttribute, and IsAuxillaryAttribute. Takes in a range and a name to look for.
		/// </summary>
		/// <param name="attributeName">The name to look for.</param>
		/// <param name="first">The first index.</param>
		/// <param name="last">One past the last index to look for.</param>
		/// <returns>Whether or not the passed in name was found.</returns>
		bool CompareKeys(const std::string& attributeName, Indices) const;
	};
}
