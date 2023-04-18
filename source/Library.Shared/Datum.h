#pragma once

#include <cstddef>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <array>
#include "RTTI.h"

namespace FieaGameEngine
{
	class Attributed;
	class Scope;
	class JsonTableParseHelper;
	/// <summary>
	/// Datum is a class that is part of a larger hierarchy that can contain multiple different types including int, float, string, vec4, mat4, and RTTI*. It stores the values in a dynamic array. In order to utilize the storage
	/// of a Datum, the type must first be set. This can be done during initialization or after initialization. The type cannot be changed dynamically unless being assigned to the value of another datum. Datums can also serve as a thin wrapper for
	/// other class' data.
	/// </summary>
	class Datum final
	{
	public:
		friend Scope;
		friend Attributed;
		friend JsonTableParseHelper;
		/// <summary>
		/// Enumeration for the possible data types.
		/// </summary>
		enum class DatumTypes
		{
			Unknown = -1,
			Integer = 0,
			Float = 1,
			String = 2,
			Vector = 3,
			Matrix = 4,
			Pointer = 5,
			Table = 6
		};

	private:
		union DatumValues final
		{
			int* i;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			RTTI** p;
			Scope** t;
			void* vp;
		};


		inline static const std::array<std::size_t, 7> _typeSizes
		{
			sizeof(int),
			sizeof(float),
			sizeof(std::string),
			sizeof(glm::vec4),
			sizeof(glm::mat4),
			sizeof(RTTI*),
			sizeof(Scope*)
		};

		DatumValues _data{ nullptr };
		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		DatumTypes _type{ DatumTypes::Unknown };
		std::size_t _typeSize{ 0 };
		bool _isExternal{ false };

	public:
		/// <summary>
		/// Default constructs a new Datum instance that has no type, data, size, or capacity.
		/// </summary>
		Datum() = default;
		/// <summary>
		/// Default constructs a datum that now has the type and can store objects of the type corresponding to the passed in enumaeration.
		/// </summary>
		/// <param name="type"> The enumeration of Datum::DatumTypes that corresponds to the desired type for the Datum to store and act as.</param>
		explicit Datum(DatumTypes type);
		/// <summary>
		/// Constructs a datum equal to, but not the same as, the passed datum.
		/// </summary>
		/// <param name="other">The other datum being copied from.</param>
		explicit Datum(const Datum& other);
		/// <summary>
		/// Constructs a datum using an rvalue reference to a Datum. 
		/// </summary>
		/// <param name="other">The rvalue being used to construct the Datum</param>
		explicit Datum(Datum&& other) noexcept;
		~Datum();
		/// <summary>
		/// Assigns the current Datum to the value(s) of the right hand Datum.  
		/// </summary>
		/// <param name="other">Datum to copy from.</param>
		/// <returns>A reference to the newly copied to Datum.</returns>
		Datum& operator=(const Datum& other);
		/// <summary>
		/// Assigns the current Datum to the value(s) of the right hand Datum.
		/// </summary>
		/// <param name="other">Datum rvalue reference to be moved from.</param>
		/// <returns>A reference to the newly move assigned to Datum.</returns>
		Datum& operator=(Datum&& other) noexcept;
		/// <summary>
		/// If the assigned Datum has internal storage, it is cleared and resized to 1 and contains only the element on the right.
		/// </summary>
		/// <param name="other">int to assign to Datum</param>
		/// <returns>Reference to the Datum being assigned.</returns>
		Datum& operator=(int other);
		/// <summary>
		/// If the assigned Datum has internal storage, it is cleared and resized to 1 and contains only the element on the right.
		/// </summary>
		/// <param name="other">float to assign to Datum</param>
		/// <returns>Reference to the Datum being assigned.</returns>
		Datum& operator=(float other);
		/// <summary>
		/// If the assigned Datum has internal storage, it is cleared and resized to 1 and contains only the element on the right.
		/// </summary>
		/// <param name="other">string to assign to Datum</param>
		/// <returns>Reference to the Datum being assigned.</returns>
		Datum& operator=(const std::string& other);
		/// <summary>
		/// If the assigned Datum has internal storage, it is cleared and resized to 1 and contains only the element on the right.
		/// </summary>
		/// <param name="other">vec4 to assign to Datum</param>
		/// <returns>Reference to the Datum being assigned.</returns>
		Datum& operator=(const glm::vec4& other);
		/// <summary>
		/// If the assigned Datum has internal storage, it is cleared and resized to 1 and contains only the element on the right.
		/// </summary>
		/// <param name="other">mat4 to assign to Datum</param>
		/// <returns>Reference to the Datum being assigned.</returns>
		Datum& operator=(const glm::mat4& other);
		/// <summary>
		/// If the assigned Datum has internal storage, it is cleared and resized to 1 and contains only the element on the right.
		/// </summary>
		/// <param name="other">RTTI* to assign to Datum</param>
		/// <returns>Reference to the Datum being assigned.</returns>
		Datum& operator=(RTTI* const);
		Datum& operator=(const Scope&);
		/// <summary>
		/// Checks equivalence to another Datum. This comparison does not take into account the capacity of the Datums, but rather the type, size, and contents of said Datums.
		/// </summary>
		/// <param name="rhs">The right hand Datum being compared to.</param>
		/// <returns>bool whether or not the two Datums are equivalent</returns>
		bool operator==(const Datum& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an int. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">int value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator==(int rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an float. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">float value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator==(float rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an string. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">string value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator==(const std::string& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an vec4. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">vec4 value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator==(const glm::vec4& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an mat4. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">mat4 value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator==(const glm::mat4& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an RTTI*. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">RTTI* value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator==(RTTI* rhs) const;
		/// <summary>
		/// Checks equivalence to another Datum. This comparison does not take into account the capacity of the Datums, but rather the type, size, and contents of said Datums.
		/// </summary>
		/// <param name="rhs">The right hand Datum being compared to.</param>
		/// <returns>bool whether or not the two Datums are equivalent</returns>
		bool operator!=(const Datum& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an int. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">int value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator!=(int rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an string. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">string value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator!=(float rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an string. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">string value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator!=(const std::string& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an vec4. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">vec4 value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator!=(const glm::vec4& rhs) const;
		/// <summary>
		/// Checks Equivalence between a Datum and an RTTI*. The Datum must be of size 1 and of the same type in order to be compared.
		/// </summary>
		/// <param name="rhs">RTTI* value to be compared against.</param>
		/// <returns>bool whether or not the scalar values are equivalent.</returns>
		bool operator!=(const glm::mat4& rhs) const;
		/// <summary>
		/// Checks equivalence to another Datum. This comparison does not take into account the capacity of the Datums, but rather the type, size, and contents of said Datums.
		/// </summary>
		/// <param name="rhs">The right hand Datum being compared to.</param>
		/// <returns>bool whether or not the two Datums are equivalent</returns>
		bool operator!=(RTTI* rhs) const;
		/// <summary>
		/// Takes in a DatumTypes enumeration that sets the type of the Datum.
		/// </summary>
		/// <param name="type">DatumTypes enumeration that corresponds to the data type housed within the Datum.</param>
		void SetType(DatumTypes type);
		/// <summary>
		/// A getter for the current type assigned to the Datum.
		/// </summary>
		/// <returns>A DatumTypes enumeration specifying the type of data stored within the Datum.</returns>
		DatumTypes Type();
		/// <summary>
		/// Sets the the storage to external for the Datum and assigns it as a thin layer to view the contents of another structure.
		/// </summary>
		/// <param name="array">Array that holds the data. </param>
		/// <param name="size">The size of the type.</param>
		void SetStorage(int* array, std::size_t size);
		/// <summary>
		/// Sets the the storage to external for the Datum and assigns it as a thin layer to view the contents of another structure.
		/// </summary>
		/// <param name="array">Array that holds the data. </param>
		/// <param name="size">The size of the type.</param>
		void SetStorage(float* array, std::size_t size);
		/// <summary>
		/// Sets the the storage to external for the Datum and assigns it as a thin layer to view the contents of another structure.
		/// </summary>
		/// <param name="array">Array that holds the data. </param>
		/// <param name="size">The size of the type.</param>
		void SetStorage(std::string* array, std::size_t size);
		/// <summary>
		/// Sets the the storage to external for the Datum and assigns it as a thin layer to view the contents of another structure.
		/// </summary>
		/// <param name="array">Array that holds the data. </param>
		/// <param name="size">The size of the type.</param>
		void SetStorage(glm::vec4* array, std::size_t size);
		/// <summary>
		/// Sets the the storage to external for the Datum and assigns it as a thin layer to view the contents of another structure.
		/// </summary>
		/// <param name="array">Array that holds the data. </param>
		/// <param name="size">The size of the type.</param>
		void SetStorage(glm::mat4* array, std::size_t size);
		/// <summary>
		/// Sets the the storage to external for the Datum and assigns it as a thin layer to view the contents of another structure.
		/// </summary>
		/// <param name="array">Array that holds the data. </param>
		/// <param name="size">The size of the type.</param>
		void SetStorage(RTTI** array, std::size_t size);
		/// <summary>
		/// Returns the number of elements held within the Datum.
		/// </summary>
		/// <returns>std::size_t number of elements</returns>
		std::size_t Size();
		/// <summary>
		/// Removes all elements from the Datum and does work where needed by destructing the objects.
		/// </summary>
		void Clear();
		/// <summary>
		/// Returns the total number of allocated spots availalbe for stored data.
		/// </summary>
		/// <returns>std::size_t The total number of allocated spots for data.</returns>
		std::size_t Capacity();
		/// <summary>
		/// Resizes the datum to the requested size. Will shrink or grow the Datum. If the datum grows, it will initialize the data as needed.
		/// </summary>
		/// <param name="capacity">The size requested to conform to.</param>
		void Resize(std::size_t capacity);
		/// <summary>
		/// Attempts to grow the Datum to the specified capacity. Will not shrink the Datum.
		/// </summary>
		/// <param name="capacity">The number of spots to allocate.</param>
		void Reserve(std::size_t capacity);
		/// <summary>
		/// Will adjust the number of allocated spots to conform to the current size. It will reset the Datum if 0 is passed in.
		/// </summary>
		void ShrinkToFit();
		/// <summary>
		/// Will remove the last element in the Datum.
		/// </summary>
		void PopBack();
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">int value to put at the end</param>
		void PushBack(int item);
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">float value to put at the end</param>
		void PushBack(float item);
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">std::string reference value to put at the end</param>
		void PushBack(const std::string& item);
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">std::string rvalue reference value to put at the end</param>
		void PushBack(std::string&& item);
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">vec4 reference value to put at the end</param>
		void PushBack(const glm::vec4& item);
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">mat4 reference value to put at the end</param>
		void PushBack(const glm::mat4& item);
		/// <summary>
		/// Pushes the specified value to the end of the Datum.
		/// </summary>
		/// <param name="item">RTTI* value to put at the end</param>
		void PushBack(RTTI* item);
		void PushBackFromString(const std::string& string);
		/// <summary>
		/// Sets the value at the specified position granted that the index is less than the Datum's size.
		/// </summary>
		/// <param name="item">int value to assign</param>
		/// <param name="index">position to assign to.</param>
		void Set(int item, std::size_t index = 0);
		/// <summary>
		/// Sets the value at the specified position granted that the index is less than the Datum's size.
		/// </summary>
		/// <param name="item">float value to assign</param>
		/// <param name="index">position to assign to.</param>
		void Set(float item, std::size_t index = 0);
		/// <summary>
		/// Sets the value at the specified position granted that the index is less than the Datum's size.
		/// </summary>
		/// <param name="item">std::string reference value to assign</param>
		/// <param name="index">position to assign to.</param>
		void Set(const std::string& item, std::size_t index = 0);
		/// <summary>
		/// Sets the value at the specified position granted that the index is less than the Datum's size.
		/// </summary>
		/// <param name="item">vec4 reference value to assign</param>
		/// <param name="index">position to assign to.</param>
		void Set(glm::vec4 item, std::size_t index = 0);
		/// <summary>
		/// Sets the value at the specified position granted that the index is less than the Datum's size.
		/// </summary>
		/// <param name="item">mat4 reference value to assign</param>
		/// <param name="index">position to assign to.</param>
		void Set(const glm::mat4& item, std::size_t index = 0);
		/// <summary>
		/// Sets the value at the specified position granted that the index is less than the Datum's size.
		/// </summary>
		/// <param name="item">RTTI* value to assign</param>
		/// <param name="index">position to assign to.</param>
		void Set(RTTI* item, std::size_t index = 0);
		/// <summary>
		/// Traverses the Datum in order to find the given value and returns an index to said item if it is found, otherwise it will return the size of the Datum.
		/// </summary>
		/// <param name="value">The value to search for.</param>
		/// <returns>std::size_t an index to the found item, will return the size of the Datum.</returns>
		std::size_t Find(int value) const;
		/// <summary>
		/// Traverses the Datum in order to find the given value and returns an index to said item if it is found, otherwise it will return the size of the Datum.
		/// </summary>
		/// <param name="value">The value to search for.</param>
		/// <returns>std::size_t an index to the found item, will return the size of the Datum.</returns>
		std::size_t Find(float value) const;
		/// <summary>
		/// Traverses the Datum in order to find the given value and returns an index to said item if it is found, otherwise it will return the size of the Datum.
		/// </summary>
		/// <param name="value">The value to search for.</param>
		/// <returns>std::size_t an index to the found item, will return the size of the Datum.</returns>
		std::size_t Find(const std::string& value) const;
		/// <summary>
		/// Traverses the Datum in order to find the given value and returns an index to said item if it is found, otherwise it will return the size of the Datum.
		/// </summary>
		/// <param name="value">The value to search for.</param>
		/// <returns>std::size_t an index to the found item, will return the size of the Datum.</returns>
		std::size_t Find(const glm::vec4& value) const;
		/// <summary>
		/// Traverses the Datum in order to find the given value and returns an index to said item if it is found, otherwise it will return the size of the Datum.
		/// </summary>
		/// <param name="value">The value to search for.</param>
		/// <returns>std::size_t an index to the found item, will return the size of the Datum.</returns>
		std::size_t Find(const glm::mat4& value) const;
		/// <summary>
		/// Traverses the Datum in order to find the given value and returns an index to said item if it is found, otherwise it will return the size of the Datum.
		/// </summary>
		/// <param name="value">The value to search for.</param>
		/// <returns>std::size_t an index to the found item, will return the size of the Datum.</returns>
		std::size_t Find(const RTTI* value) const;
		/// <summary>
		/// Uses Find with the given value and if the returned index is less than size, it will remove the data stored at that index.
		/// </summary>
		/// <param name="value">The value to find and remove.</param>
		void Remove(int value);
		/// <summary>
		/// Uses Find with the given value and if the returned index is less than size, it will remove the data stored at that index.
		/// </summary>
		/// <param name="value">The value to find and remove.</param>
		void Remove(float value);
		/// <summary>
		/// Uses Find with the given value and if the returned index is less than size, it will remove the data stored at that index.
		/// </summary>
		/// <param name="value">The value to find and remove.</param>
		void Remove(const std::string& value);
		/// <summary>
		/// Uses Find with the given value and if the returned index is less than size, it will remove the data stored at that index.
		/// </summary>
		/// <param name="value">The value to find and remove.</param>
		void Remove(const glm::vec4& value);
		/// <summary>
		/// Uses Find with the given value and if the returned index is less than size, it will remove the data stored at that index.
		/// </summary>
		/// <param name="value">The value to find and remove.</param>
		void Remove(const glm::mat4& value);
		/// <summary>
		/// Uses Find with the given value and if the returned index is less than size, it will remove the data stored at that index.
		/// </summary>
		/// <param name="value">The value to find and remove.</param>
		void Remove(const RTTI* value);
		/// <summary>
		/// Removes the data at the given index if the passed index is less than size.
		/// </summary>
		/// <param name="index">The index to remove at.</param>
		void RemoveAt(std::size_t index);
		/// <summary>
		/// Returns the value at the given index as an int.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>A mutable reference to the value.</returns>
		int& GetAsInt(std::size_t index = 0);
		/// <summary>
		/// Returns the value at the given index as a float.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>A mutable reference to the value.</returns>
		float& GetAsFloat(std::size_t index = 0);
		/// <summary>
		/// Returns the value at the given index as a std::string reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>A mutable reference to the value.</returns>
		std::string& GetAsString(std::size_t index = 0);
		/// <summary>
		/// Returns the value at the given index as a vec4 reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>A mutable reference to the value.</returns>
		glm::vec4& GetAsVector(std::size_t index = 0);
		/// <summary>
		/// Returns the value at the given index as a mat4 reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>A mutable reference to the value.</returns>
		glm::mat4& GetAsMatrix(std::size_t index = 0);
		/// <summary>
		/// Returns the value at the given index as a RTTI* reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>A mutable reference to the value.</returns>
		RTTI*& GetAsRTTI(std::size_t index = 0);
		/// <summary>
		/// Returns the value at the given index as an int.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>An immutable copy of the value.</returns>
		int GetAsInt(std::size_t index = 0) const;
		/// <summary>
		/// Returns the value at the given index as a float.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>An immutable copy of the value.</returns>
		float GetAsFloat(std::size_t index = 0) const;
		/// <summary>
		/// Returns the value at the given index as a std::string reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>An immutable reference of the value.</returns>
		const std::string& GetAsString(std::size_t index = 0) const;
		/// <summary>
		/// Returns the value at the given index as a vec4 reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>An immutable reference of the value.</returns>
		const glm::vec4& GetAsVector(std::size_t index = 0) const;
		/// <summary>
		/// Returns the value at the given index as a mat4 reference.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>An immutable reference of the value.</returns>
		const glm::mat4& GetAsMatrix(std::size_t index = 0) const;
		/// <summary>
		/// Returns the value at the given index as an RTTI*.
		/// </summary>
		/// <param name="index">The index at which to pull from.</param>
		/// <returns>An immutable copy of the value.</returns>
		RTTI* GetAsRTTI(std::size_t index = 0) const;
		Scope* GetAsScope(std::size_t index = 0) const;
		/// <summary>
		/// Returns the value at the front as an int.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		int& FrontAsInt();
		/// <summary>
		/// Returns the value at the front as a float.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		float& FrontAsFloat();
		/// <summary>
		/// Returns the value at the front as a std::string reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		std::string& FrontAsString();
		/// <summary>
		/// Returns the value at the front as a vec4 reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		glm::vec4& FrontAsVector();
		/// <summary>
		/// Returns the value at the front as a mat4 reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		glm::mat4& FrontAsMatrix();
		/// <summary>
		/// Returns the value at the front as an RTTI* reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		RTTI*& FrontAsRTTI();
		/// <summary>
		/// Returns the value at the front as an int.
		/// </summary>
		/// <returns>An immutable copy of the value.</returns>
		int FrontAsInt() const;
		/// <summary>
		/// Returns the value at the front as an float.
		/// </summary>
		/// <returns>An immutable copy of the value.</returns>
		float FrontAsFloat() const;
		/// <summary>
		/// Returns the value at the front as a std::string reference.
		/// </summary>
		/// <returns>An immutable reference of the value.</returns>
		const std::string& FrontAsString() const;
		/// <summary>
		/// Returns the value at the front as a vec4 reference.
		/// </summary>
		/// <returns>An immutable reference of the value.</returns>
		const glm::vec4& FrontAsVector() const;
		/// <summary>
		/// Returns the value at the front as a mat4 reference.
		/// </summary>
		/// <returns>An immutable reference of the value.</returns>
		const glm::mat4& FrontAsMatrix() const;
		/// <summary>
		/// Returns the value at the front as a RTTI*.
		/// </summary>
		/// <returns>An immutable copy of the value.</returns>
		RTTI* FrontAsRTTI() const;
		/// <summary>
		/// Returns the value at the back as an int.
		/// </summary>
		/// <returns>A mutable copy to the value.</returns>
		int& BackAsInt();
		/// <summary>
		/// Returns the value at the back as a float.
		/// </summary>
		/// <returns>A mutable copy to the value.</returns>
		float& BackAsFloat();
		/// <summary>
		/// Returns the value at the back as a std::string reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		std::string& BackAsString();
		/// <summary>
		/// Returns the value at the back as a vec4 reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		glm::vec4& BackAsVector();
		/// <summary>
		/// Returns the value at the back as a mat4 reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		glm::mat4& BackAsMatrix();
		/// <summary>
		/// Returns the value at the back as an RTTI* reference.
		/// </summary>
		/// <returns>A mutable reference to the value.</returns>
		RTTI*& BackAsRTTI();
		/// <summary>
		/// Returns the value at the back as an int.
		/// </summary>
		/// <returns>An immutable copy of the value.</returns>
		int BackAsInt() const;
		/// <summary>
		/// Returns the value at the back as a float.
		/// </summary>
		/// <returns>An immutable copy of the value.</returns>
		float BackAsFloat() const;
		/// <summary>
		/// Returns the value at the back as a std::string reference.
		/// </summary>
		/// <returns>An immutable reference of the value.</returns>
		const std::string& BackAsString() const;
		/// <summary>
		/// Returns the value at the back as a vec4 reference.
		/// </summary>
		/// <returns>An immutable reference of the value.</returns>
		const glm::vec4& BackAsVector() const;
		/// <summary>
		/// Returns the value at the back as a mat4 reference.
		/// </summary>
		/// <returns>An immutable reference of the value.</returns>
		const glm::mat4& BackAsMatrix() const;
		/// <summary>
		/// Returns the value at the back as an RTTI*.
		/// </summary>
		/// <returns>An immutable copy of the value.</returns>
		RTTI* BackAsRTTI() const;
		/// <summary>
		/// Returns a string of the value at the given index.
		/// </summary>
		/// <param name="index">The index at which to pull the value from.</param>
		/// <returns>std::string The data as a string.</returns>
		std::string ToString(std::size_t index = 0);
		/// <summary>
		/// Sets an item using a value interpreted through a passed string at the given index.
		/// </summary>
		/// <param name="string">The string that contains ther information about the data to set.</param>
		/// <param name="index">An index to the position to place the value.</param>
		void SetFromString(const std::string& string, std::size_t index = 0);

	private:
		void PushBack(Scope* item);
		/// <summary>
		/// A wrapper for realloc that takes a capacity argument.
		/// </summary>
		/// <param name="capacity">Amount of storage to request.</param>
		void Relocate(std::size_t capacity);
		/// <summary>
		/// Resets the Datum to a blank slate.
		/// </summary>
		void Reset();
		/// <summary>
		/// Generic assignment for trivially copyable types.
		/// </summary>
		/// <param name="data">The pointer to stored data.</param>
		void ScalarAssignment(const void* data);
		/// <summary>
		/// Sets a set of external storage for the Datum to monitor.
		/// </summary>
		/// <param name="array">The array to wrap.</param>
		/// <param name="size">The size of the passed array.</param>
		void SetStorage(void* array, std::size_t size);
		/// <summary>
		/// A generic mechanism for pushing back trivially copiable data.
		/// </summary>
		/// <param name="data">Pointer to the data to store.</param>
		void PushBack(const void* data);
		/// <summary>
		/// Creates a string fron an int at the given index.
		/// </summary>
		/// <param name="index">position to pull the data from.</param>
		/// <returns>std::string version of the data.</returns>
		std::string ToStringInt(std::size_t index);
		/// <summary>
		/// Creates a string fron a float at the given index.
		/// </summary>
		/// <param name="index">position to pull the data from.</param>
		/// <returns>std::string version of the data.</returns>
		std::string ToStringFloat(std::size_t index);
		/// <summary>
		/// Returns a string fron a std::string at the given index.
		/// </summary>
		/// <param name="index">position to pull the data from.</param>
		/// <returns>std::string version of the data.</returns>
		std::string ToStringString(std::size_t index);
		/// <summary>
		/// Creates a string fron a vec4 at the given index.
		/// </summary>
		/// <param name="index">position to pull the data from.</param>
		/// <returns>std::string version of the data.</returns>
		std::string ToStringVector(std::size_t index);
		/// <summary>
		/// Creates a string fron a mat4 at the given index.
		/// </summary>
		/// <param name="index">position to pull the data from.</param>
		/// <returns>std::string version of the data.</returns>
		std::string ToStringMatrix(std::size_t index);
		/// <summary>
		/// Creates a string fron an RTTI* at the given index.
		/// </summary>
		/// <param name="index">position to pull the data from.</param>
		/// <returns>std::string version of the data.</returns>
		std::string ToStringPointer(std::size_t index);
		/// <summary>
		/// Generically compares a trivially comparable set of data given an array of data.
		/// </summary>
		/// <param name="data">The array of data.</param>
		/// <returns>bool whether or not the set is equivalent.</returns>
		bool GenericSetCompare(void* data) const;
		/// <summary>
		/// Compares a set of strings in an array.
		/// </summary>
		/// <param name="data">The array of data.</param>
		/// <returns>bool whether or not the set is equivalent.</returns>
		bool StringSetCompare(void* data) const;
		/// <summary>
		/// Compares a set of RTTI* by comparing their RTTI objects.
		/// </summary>
		/// <param name="data">The array of data.</param>
		/// <returns>bool whether or not the set is equivalent.</returns>
		bool PointerSetCompare(void* data) const;
		/// <summary>
		/// Creates an int from a std::string and places it at a given index.
		/// </summary>
		/// <param name="string">std::string to grab the data from.</param>
		/// <param name="index">Index at which to place the data.</param>
		void SetIntFromString(const std::string& string, std::size_t index);
		/// <summary>
		/// Creates a float from a std::string and places it at a given index.
		/// </summary>
		/// <param name="string">std::string to grab the data from.</param>
		/// <param name="index">Index at which to place the data.</param>
		void SetFloatFromString(const std::string& string, std::size_t index);
		/// <summary>
		/// Creates a vec4 from a std::string and places it at a given index.
		/// </summary>
		/// <param name="string">std::string to grab the data from.</param>
		/// <param name="index">Index at which to place the data.</param>
		void SetVectorFromString(const std::string& string, std::size_t index);
		/// <summary>
		/// Creates a mat4 from a std::string and places it at a given index.
		/// </summary>
		/// <param name="string">std::string to grab the data from.</param>
		/// <param name="index">Index at which to place the data.</param>
		void SetMatrixFromString(const std::string& string, std::size_t index);
		void PushBackVectorFromString(const std::string& string);
		void PushBackMatrixFromString(const std::string& string);

		using ToStringFunction = std::string(Datum::*)(std::size_t);
		inline static const std::array< ToStringFunction, 7> _toStringFunctions
		{

			&ToStringInt,     //DatumTypes::Integer = 0
			&ToStringFloat,   //DatumTypes::Float
			&ToStringString,  //DatumTypes::String
			&ToStringVector,  //DatumTypes::Vector
			&ToStringMatrix,  //DatumTypes::Matrix
			&ToStringPointer, //DatumTypes::Pointer
			&ToStringPointer  //DatumTypes::Table
		};

		using SetCompareFunction = bool(Datum::*)(void*) const;
		inline static const std::array<SetCompareFunction, 7> _setCompareFunctions
		{
			&GenericSetCompare, //DatumTypes::Integer = 0
			&GenericSetCompare, //DatumTypes::Float
			&StringSetCompare,  //DatumTypes::String
			&GenericSetCompare, //DatumTypes::Vector
			&GenericSetCompare, //DatumTypes::Matrix
			&PointerSetCompare, //DatumTypes::Pointer
			&PointerSetCompare  //DatumTypes::Table
		};

		using SetFromStringFunction = void(Datum::*)(const std::string&, std::size_t);
		inline static const std::array<SetFromStringFunction, 7> _setFromStringFunctions
		{
			&SetIntFromString,     //DatumTypes::Integer = 0
			&SetFloatFromString,   //DatumTypes::Float
			&Set,				   //DatumTypes::String
			&SetVectorFromString,  //DatumTypes::Vector
			&SetMatrixFromString,  //DatumTypes::Matrix
			nullptr,			   //DatumTypes::Pointer
			nullptr				   //DatumTypes::Table	
		};

		using PushBackFromStringFunction = void(Datum::*)(const std::string&);
		inline static const std::array<PushBackFromStringFunction, 7> _pushbackFromStringFunctions
		{
			nullptr,					//DatumTypes::Integer = 0
			nullptr,					//DatumTypes::Float
			nullptr,				    //DatumTypes::String
			&PushBackVectorFromString,  //DatumTypes::Vector
			&PushBackMatrixFromString,  //DatumTypes::Matrix
			nullptr,					//DatumTypes::Pointer
			nullptr						//DatumTypes::Table	
		};
	};
}

