#include "Datum.h"
#include <cassert>

#pragma region Constructors
FieaGameEngine::Datum::Datum(DatumTypes type) :
	_type{ type }, _typeSize{type != DatumTypes::Unknown ? _typeSizes[static_cast<std::size_t>(type)] : 0}
{
}

FieaGameEngine::Datum::Datum(const Datum& other) :
	_data{ other._isExternal ? other._data : _data }, _size{ other._size },
	_capacity{ other._capacity }, _type{ other._type }, _typeSize{ other._typeSize },
	_isExternal{ other._isExternal }
{
	if( !_isExternal )
	{
		void* temp = malloc(_typeSize * _capacity);
		assert(temp != nullptr);
		_data.vp = temp;
		if (_type == DatumTypes::String)
		{
			_size = 0;
			for (std::size_t position{ 0 }; position < other._size; ++position)
			{
				PushBack(other._data.s[position]);
			}
		}
		else
		{
			memcpy(_data.vp, other._data.vp, _size * _typeSize);
		}
	}
}

FieaGameEngine::Datum::Datum(Datum&& other) noexcept :
	_data{ other._data }, _size{ other._size },
	_capacity{ other._capacity }, _type{ other._type }, _typeSize{ other._typeSize },
	_isExternal{ other._isExternal }
{
	other._data.vp = nullptr;
	other._size = 0;
	other._capacity = 0;
}

FieaGameEngine::Datum::~Datum()
{
	if (!_isExternal)
	{
		Clear();
		free(_data.vp);
	}
}
#pragma endregion 

#pragma region Assignment

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(const Datum& other)
{
	if (this == &other)
	{
		return *this;
	}

	if (!_isExternal)
	{
		Reset();
		_capacity = 0;
		_typeSize = other._typeSize;
		_type = other._type;

		if (!other._isExternal)
		{
			Resize(other._capacity);

			if (_type == DatumTypes::String)
			{
				for (std::size_t position{ 0 }; position < other._size; ++position)
				{
					Set(other._data.s[position], position);
				}
			}
			else
			{
				if (other._size != 0)
				{
					assert(_data.vp != nullptr);
					_size = other._size;
					memcpy(_data.vp, other._data.vp, _size * _typeSize);
				}
			}
		}

		else
		{
			_size = other._size;
			_capacity = _size;
			_data.vp = other._data.vp;
			_type = other._type;
			_isExternal = true;
		}
	}

	else
	{
		if (!other._isExternal)
		{
			_typeSize = other._typeSize;
			_type = other._type;
			_capacity = other._capacity;
			_size = other._size;
			_isExternal = false;
			void* temp = malloc(_typeSize * _capacity);
			assert(temp != nullptr);
			_data.vp = temp;
			if (_type == DatumTypes::String)
			{
				_size = 0;
				for (std::size_t position{ 0 }; position < other._size; ++position)
				{
					PushBack(other._data.s[position]);
				}
			}
			else
			{
				memcpy(_data.vp, other._data.vp, other._size * _typeSize);
			}
		}

		else
		{
			_typeSize = other._typeSize;
			_type = other._type;
			_capacity = 0;
			_size = other._size;
			_data.vp = other._data.vp;
		}

	}
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(Datum&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	if (!_isExternal)
	{
		Reset();
	}
	_data = other._data;
	_size = other._size;
	_capacity = other._capacity;
	_type = other._type;
	_typeSize = other._typeSize;
	_isExternal = other._isExternal;
	other._data.vp = nullptr;
	other._size = 0;
	other._capacity = 0;

	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(int other)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Integer : _type;
	assert(_type == DatumTypes::Integer);
	if (_isExternal)
	{
		Set(other);
	}
	else
	{
		ScalarAssignment(&other);
	}
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(float other)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Float : _type;
	assert(_type == DatumTypes::Float);
	if (_isExternal)
	{
		Set(other);
	}
	else
	{
		ScalarAssignment(&other);
	}
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(const std::string& other)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::String : _type;
	assert(_type == DatumTypes::String);
	if (_isExternal)
	{
		Set(other);
	}
	else
	{
		_typeSize = sizeof(std::string);
		Reset();
		_capacity = 0;
		PushBack(other);
	}
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(const glm::vec4& other)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Vector : _type;
	assert(_type == DatumTypes::Vector);
	if (_isExternal)
	{
		Set(other);
	}
	else
	{
		ScalarAssignment(&other);
	}
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(const glm::mat4& other)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Matrix : _type;
	assert(_type == DatumTypes::Matrix);
	if (_isExternal)
	{
		Set(other);
	}
	else
	{
		ScalarAssignment(&other);
	}
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(RTTI* const other)
{
	assert(_isExternal == false);
	_type = _type == DatumTypes::Unknown ? DatumTypes::Pointer : _type;
	assert(_type == DatumTypes::Pointer);
	ScalarAssignment(&other);
	return *this;
}

FieaGameEngine::Datum& FieaGameEngine::Datum::operator=(const Scope& other)
{
	assert(_isExternal == false);
	_type = _type == DatumTypes::Unknown ? DatumTypes::Table : _type;
	assert(_type == DatumTypes::Table);
	ScalarAssignment(&other);
	return *this;
}

#pragma endregion

#pragma region Utility
void FieaGameEngine::Datum::SetType(DatumTypes type)
{
	_type = _type == DatumTypes::Unknown ? type : _type;
}

FieaGameEngine::Datum::DatumTypes FieaGameEngine::Datum::Type()
{
	return _type;
}

std::size_t FieaGameEngine::Datum::Size()
{
	return _size;
}

std::size_t FieaGameEngine::Datum::Capacity()
{
	return _capacity;
}

void FieaGameEngine::Datum::PopBack()
{
	if (!_isExternal && _size > 0)
	{
		if (_type == DatumTypes::String)
		{
			BackAsString().~basic_string();
		}
		--_size;
	}
}

void FieaGameEngine::Datum::ScalarAssignment(const void* other)
{
	_typeSize = _typeSizes[static_cast<std::size_t>(_type)];
	Reset();
	_size = 1;
	_capacity = 1;
	void* temp = malloc(_typeSize * _capacity);
	assert(temp != nullptr);
	_data.vp = temp;
	memcpy(_data.vp, other, _size * _typeSize);
}

#pragma endregion

#pragma region SetStorage

void FieaGameEngine::Datum::SetStorage(int* array, std::size_t size)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Integer : _type;
	assert(_type == DatumTypes::Integer);
	SetStorage(static_cast<void*>(array), size);
}

void FieaGameEngine::Datum::SetStorage(float* array, std::size_t size)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Float : _type;
	assert(_type == DatumTypes::Float);
	SetStorage(static_cast<void*>(array), size);
}

void FieaGameEngine::Datum::SetStorage(std::string* array, std::size_t size)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::String : _type;
	assert(_type == DatumTypes::String);
	SetStorage(static_cast<void*>(array), size);
}

void FieaGameEngine::Datum::SetStorage(glm::vec4* array, std::size_t size)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Vector : _type;
	assert(_type == DatumTypes::Vector);
	SetStorage(static_cast<void*>(array), size);
}

void FieaGameEngine::Datum::SetStorage(glm::mat4* array, std::size_t size)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Matrix : _type;
	assert(_type == DatumTypes::Matrix);
	SetStorage(static_cast<void*>(array), size);
}

void FieaGameEngine::Datum::SetStorage(RTTI** array, std::size_t size)
{
	_type = _type == DatumTypes::Unknown ? DatumTypes::Pointer : _type;
	assert(_type == DatumTypes::Pointer);
	SetStorage(static_cast<void*>(array), size);
}

void FieaGameEngine::Datum::SetStorage(void* array, std::size_t size)
{
	if (!_isExternal)
	{
		Reset();
	}
	_size = size;
	_capacity = _size;
	_data.vp = array;
	_isExternal = true;
}

#pragma endregion

#pragma region Allocation
void FieaGameEngine::Datum::Resize(std::size_t capacity)
{
	if (!_isExternal)
	{ 
		if (capacity == 0)
		{
			Reset();
			_capacity = 0;
			return;
		}

		if (capacity < _size)
		{
			if (_type == DatumTypes::String)
			{
				for (std::size_t position{ capacity }; position < _size; ++position)
				{
					_data.s[position].~basic_string();
				}
			}
			_size = capacity;
			ShrinkToFit();
		}

		else if(capacity > _capacity)
		{
			Reserve(capacity);
			if (_type == DatumTypes::String)
			{
				for (std::size_t position{ _size }; position < capacity; ++position)
				{
					PushBack(std::string{});
				}
			}

			else
			{
				memset(static_cast<std::byte*>(_data.vp) + (_size * _typeSize), 0, (_capacity - _size) * _typeSize);
				_size = _capacity;
			}
		}
	}

}

void FieaGameEngine::Datum::Reserve(std::size_t capacity)
{
	if (!_isExternal)
	{
		if (capacity > _capacity)
		{
			Relocate(capacity);
			_capacity = capacity;
		}
	}
}

void FieaGameEngine::Datum::ShrinkToFit()
{
	if (!_isExternal)
	{
		if (_capacity > _size)
		{
			Relocate(_size);
			_capacity = _size;
		}
	}
}

void FieaGameEngine::Datum::Relocate(std::size_t capacity)
{
	if (!_isExternal && capacity > 0)
	{
		void* vec = reinterpret_cast<void*>(realloc(_data.vp, (capacity)*_typeSize));
		assert(vec != nullptr);
		_data.vp = vec;
	}
}

void FieaGameEngine::Datum::Reset()
{
	Clear();
	free(_data.vp);
	_data.vp = nullptr;
}
void FieaGameEngine::Datum::Clear()
{
	if (!_isExternal && _type == DatumTypes::String)
	{
		for (std::size_t position{ 0 }; position < _size; ++position)
		{
			_data.s[position].~basic_string();
		}
	}
	_size = 0;
}

#pragma endregion

#pragma region Comparison

bool FieaGameEngine::Datum::operator==(const Datum& rhs) const
{
	bool result{ false };

	if (_type == rhs._type && _size == rhs._size)
	{
		if (_type == DatumTypes::Unknown)
		{
			result = true;
		}
		else
		{
			SetCompareFunction func = _setCompareFunctions[static_cast<int>(_type)];
			assert(func != nullptr);
			result = (this->*func)(rhs._data.vp);
		}
	}

	return result;
}

bool FieaGameEngine::Datum::operator!=(const Datum& rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::operator==(int rhs) const
{
	bool result{ false };

	if (_size == 1 && _type == DatumTypes::Integer)
	{
		result = FrontAsInt() == rhs;
	}
	return result;
}

bool FieaGameEngine::Datum::operator!=(int rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::operator==(float rhs) const
{
	bool result{ false };

	if (_size == 1 && _type == DatumTypes::Float)
	{
		result = FrontAsFloat() == rhs;
	}
	return result;
}

bool FieaGameEngine::Datum::operator!=(float rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::operator==(const std::string& rhs) const
{
	bool result{ false };

	if (_size == 1 && _type == DatumTypes::String)
	{
		result = FrontAsString() == rhs;
	}
	return result;
}

bool FieaGameEngine::Datum::operator!=(const std::string& rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::operator==(const glm::vec4& rhs) const
{
	bool result{ false };

	if (_size == 1 && _type == DatumTypes::Vector)
	{
		result = FrontAsVector() == rhs;
	}
	return result;
}

bool FieaGameEngine::Datum::operator!=(const glm::vec4& rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::operator==(const glm::mat4& rhs) const
{
	bool result{ false };

	if (_size == 1 && _type == DatumTypes::Matrix)
	{
		result = FrontAsMatrix() == rhs;
	}
	return result;
}

bool FieaGameEngine::Datum::operator!=(const glm::mat4& rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::operator==(RTTI* rhs) const
{
	bool result{ false };

	if (_size == 1 && _type == DatumTypes::Pointer)
	{
		result = FrontAsRTTI()->Equals(rhs);
	}
	return result;
}

bool FieaGameEngine::Datum::operator!=(RTTI* rhs) const
{
	return !operator==(rhs);
}

bool FieaGameEngine::Datum::GenericSetCompare(void* data) const
{
	assert(_type != DatumTypes::String && _type != DatumTypes::Pointer);
	return memcmp(_data.vp, data, _size * _typeSize) == 0 ? true : false;
}

bool FieaGameEngine::Datum::StringSetCompare(void* data) const
{
	assert(_type == DatumTypes::String);
	for (std::size_t position{ 0 }; position < _size; ++position)
	{
		if (_data.s[position] != static_cast<std::string*>(data)[position])
		{
			return false;
		}
	}
	return true;
}

bool FieaGameEngine::Datum::PointerSetCompare(void* data) const
{
	assert(_type == DatumTypes::Pointer || _type == DatumTypes::Table);
	for (std::size_t position{ 0 }; position < _size; ++position)
	{
		if (!_data.p[position]->Equals(static_cast<RTTI**>(data)[position]))
		{
			return false;
		}
	}
	return true;
}

#pragma endregion

#pragma region PushBack

void FieaGameEngine::Datum::PushBack(int item)
{
	if (!_isExternal)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Integer;
			_typeSize = sizeof(int);
		}
		assert(_type == DatumTypes::Integer);
		PushBack(&item);
	}
}

void FieaGameEngine::Datum::PushBack(float item)
{
	if (!_isExternal)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Float;
			_typeSize = sizeof(float);
		}
		assert(_type == DatumTypes::Float);
		PushBack(&item);
	}
}

void FieaGameEngine::Datum::PushBack(const std::string& item)
{
	if (!_isExternal)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::String;
			_typeSize = sizeof(std::string);
		}
		assert(_type == DatumTypes::String);
		if (_capacity == _size)
		{
			Reserve(_capacity + 1);
		}
		new (_data.s + _size++)std::string{ item };
	}
}

void FieaGameEngine::Datum::PushBack(std::string&& item)
{
	if (!_isExternal)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::String;
			_typeSize = sizeof(std::string);
		}
		assert(_type == DatumTypes::String);
		if (_capacity == _size)
		{
			Reserve(_capacity + 1);
		}
		new (_data.s + _size++)std::string{ std::forward<std::string>(item) };
	}
}

void FieaGameEngine::Datum::PushBack(const glm::vec4& item)
{
	if (!_isExternal)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Vector;
			_typeSize = sizeof(glm::vec4);
		}
		assert(_type == DatumTypes::Vector);
		PushBack(&item);
	}
}

void FieaGameEngine::Datum::PushBack(const glm::mat4& item)
{
	if (!_isExternal)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Matrix;
			_typeSize = sizeof(glm::mat4);
		}
		assert(_type == DatumTypes::Matrix);
		PushBack(&item);
	}
}

void FieaGameEngine::Datum::PushBack(RTTI* item)
{
	if (!_isExternal)
	{
		assert(_type == DatumTypes::Pointer);
		PushBack(&item);
	}
}

void FieaGameEngine::Datum::PushBackFromString(const std::string& string)
{
	assert(_type != DatumTypes::Unknown && _type != DatumTypes::Pointer);
	PushBackFromStringFunction func = _pushbackFromStringFunctions[static_cast<int>(_type)];
	assert(func != nullptr);
	(this->*func)(string);
}

void FieaGameEngine::Datum::PushBack(Scope* item)
{
	if (!_isExternal)
	{
		assert(_type == DatumTypes::Table);
		PushBack(&item);
	}
}
void FieaGameEngine::Datum::PushBack(const void* item)
{
	if (!_isExternal)
	{
		if (_capacity == _size)
		{
			Reserve(_capacity + 1);
		}

		memcpy(static_cast<std::byte*>(_data.vp) + (_size++) * _typeSize, item, _typeSize);
	}
}

#pragma endregion 

#pragma region SetAndGet

void FieaGameEngine::Datum::Set(int item, std::size_t position)
{
	assert(_type == DatumTypes::Integer);
	assert(position < _size);

	_data.i[position] = item;
}

void FieaGameEngine::Datum::Set(float item, std::size_t position)
{
	assert(_type == DatumTypes::Float);
	assert(position < _size);

	_data.f[position] = item;
}

void FieaGameEngine::Datum::Set(const std::string& item, std::size_t position)
{
	assert(_type == DatumTypes::String);
	assert(position < _size);

	_data.s[position] = item;
}

void FieaGameEngine::Datum::Set(glm::vec4 item, std::size_t position)
{
	assert(_type == DatumTypes::Vector);
	assert(position < _size);

	_data.v[position] = item;
}

void FieaGameEngine::Datum::Set(const glm::mat4& item, std::size_t position)
{
	assert(_type == DatumTypes::Matrix);
	assert(position < _size);

	_data.m[position] = item;
}

void FieaGameEngine::Datum::Set(RTTI* item, std::size_t position)
{
	assert(_type == DatumTypes::Pointer);
	assert(position < _size);

	_data.p[position] = item;
}

int& FieaGameEngine::Datum::GetAsInt(std::size_t position)
{
	assert(_type == DatumTypes::Integer);
	assert(position < _size);

	return _data.i[position];
}

int FieaGameEngine::Datum::GetAsInt(std::size_t position) const
{
	assert(_type == DatumTypes::Integer);
	assert(position < _size);

	return _data.i[position];
}

float& FieaGameEngine::Datum::GetAsFloat(std::size_t position)
{
	assert(_type == DatumTypes::Float);
	assert(position < _size);

	return _data.f[position];
}

float FieaGameEngine::Datum::GetAsFloat(std::size_t position) const
{
	assert(_type == DatumTypes::Float);
	assert(position < _size);

	return _data.f[position];
}

std::string& FieaGameEngine::Datum::GetAsString(std::size_t position)
{
	assert(_type == DatumTypes::String);
	assert(position < _size);

	return _data.s[position];
}

const std::string& FieaGameEngine::Datum::GetAsString(std::size_t position) const
{
	assert(_type == DatumTypes::String);
	assert(position < _size);

	return _data.s[position];
}

glm::vec4& FieaGameEngine::Datum::GetAsVector(std::size_t position)
{
	assert(_type == DatumTypes::Vector);
	assert(position < _size);

	return _data.v[position];
}

const glm::vec4& FieaGameEngine::Datum::GetAsVector(std::size_t position) const
{
	assert(_type == DatumTypes::Vector);
	assert(position < _size);

	return _data.v[position];
}

glm::mat4& FieaGameEngine::Datum::GetAsMatrix(std::size_t position)
{
	assert(_type == DatumTypes::Matrix);
	assert(position < _size);

	return _data.m[position];
}

const glm::mat4& FieaGameEngine::Datum::GetAsMatrix(std::size_t position) const
{
	assert(_type == DatumTypes::Matrix);
	assert(position < _size);

	return _data.m[position];
}

FieaGameEngine::RTTI*& FieaGameEngine::Datum::GetAsRTTI(std::size_t position)
{
	assert(_type == DatumTypes::Pointer);
	assert(position < _size);

	return _data.p[position];
}

FieaGameEngine::RTTI* FieaGameEngine::Datum::GetAsRTTI(std::size_t position) const
{
	assert(_type == DatumTypes::Pointer);
	assert(position < _size);

	return _data.p[position];
}

FieaGameEngine::Scope* FieaGameEngine::Datum::GetAsScope(std::size_t index) const
{
	assert(_type == DatumTypes::Table);
	assert(index < _size);

	return _data.t[index];
}

#pragma endregion

#pragma region Remove

void FieaGameEngine::Datum::Remove(int value)
{
	assert(_type == DatumTypes::Integer);
	std::size_t position = Find(value);
	if (position < _size)
	{
		RemoveAt(position);
	}
}

void FieaGameEngine::Datum::Remove(float value)
{
	assert(_type == DatumTypes::Float);
	std::size_t position = Find(value);
	if (position < _size)
	{
		RemoveAt(position);
	}
}

void FieaGameEngine::Datum::Remove(const std::string& value)
{
	assert(_type == DatumTypes::String);
	std::size_t position = Find(value);
	if (position < _size)
	{
		RemoveAt(position);
	}
}

void FieaGameEngine::Datum::Remove(const glm::vec4& value)
{
	assert(_type == DatumTypes::Vector);
	std::size_t position = Find(value);
	if (position < _size)
	{
		RemoveAt(position);
	}
}

void FieaGameEngine::Datum::Remove(const glm::mat4& value)
{
	assert(_type == DatumTypes::Matrix);
	std::size_t position = Find(value);
	if (position < _size)
	{
		RemoveAt(position);
	}
}

void FieaGameEngine::Datum::Remove(const RTTI* value)
{
	assert(_type == DatumTypes::Pointer);
	std::size_t position = Find(value);
	if (position < _size)
	{
		RemoveAt(position);
	}
}

void FieaGameEngine::Datum::RemoveAt(std::size_t index)
{
	if (!_isExternal)
	{
		assert(index < _size);
		if (_type == DatumTypes::String)
		{
			_data.s[index].~basic_string();
		}
		--_size;
		memmove((static_cast<std::byte*>(_data.vp) + (index * _typeSize)), ((static_cast<std::byte*>(_data.vp)) + ((index + 1) * _typeSize)), (_size - index) * _typeSize);
	}
}
#pragma endregion

#pragma region FrontAndBack

int& FieaGameEngine::Datum::FrontAsInt()
{
	return GetAsInt(0);
}

float& FieaGameEngine::Datum::FrontAsFloat()
{
	return GetAsFloat(0);
}

std::string& FieaGameEngine::Datum::FrontAsString()
{
	return GetAsString(0);
}

glm::vec4& FieaGameEngine::Datum::FrontAsVector()
{
	return GetAsVector(0);
}

glm::mat4& FieaGameEngine::Datum::FrontAsMatrix()
{
	return GetAsMatrix(0);
}

FieaGameEngine::RTTI*& FieaGameEngine::Datum::FrontAsRTTI()
{
	return GetAsRTTI(0);
}

int FieaGameEngine::Datum::FrontAsInt() const
{
	return GetAsInt(0);
}

float FieaGameEngine::Datum::FrontAsFloat() const
{
	return GetAsFloat(0);
}

const std::string& FieaGameEngine::Datum::FrontAsString() const
{
	return GetAsString(0);
}

const glm::vec4& FieaGameEngine::Datum::FrontAsVector() const
{
	return GetAsVector(0);
}

const glm::mat4& FieaGameEngine::Datum::FrontAsMatrix() const
{
	return GetAsMatrix(0);
}

FieaGameEngine::RTTI* FieaGameEngine::Datum::FrontAsRTTI() const
{
	return GetAsRTTI(0);
}

int& FieaGameEngine::Datum::BackAsInt()
{
	return GetAsInt(_size - 1);
}

float& FieaGameEngine::Datum::BackAsFloat()
{
	return GetAsFloat(_size - 1);
}

std::string& FieaGameEngine::Datum::BackAsString()
{
	return GetAsString(_size - 1);
}

glm::vec4& FieaGameEngine::Datum::BackAsVector()
{
	return GetAsVector(_size - 1);
}

glm::mat4& FieaGameEngine::Datum::BackAsMatrix()
{
	return GetAsMatrix(_size - 1);
}

FieaGameEngine::RTTI*& FieaGameEngine::Datum::BackAsRTTI()
{
	return GetAsRTTI(_size - 1);
}

int FieaGameEngine::Datum::BackAsInt() const
{
	return GetAsInt(_size - 1);
}

float FieaGameEngine::Datum::BackAsFloat() const
{
	return GetAsFloat(_size - 1);
}

const std::string& FieaGameEngine::Datum::BackAsString() const
{
	return GetAsString(_size - 1);
}

const glm::vec4& FieaGameEngine::Datum::BackAsVector() const
{
	return GetAsVector(_size - 1);
}

const glm::mat4& FieaGameEngine::Datum::BackAsMatrix() const
{
	return GetAsMatrix(_size - 1);
}

FieaGameEngine::RTTI* FieaGameEngine::Datum::BackAsRTTI() const
{
	return GetAsRTTI(_size - 1);
}


#pragma endregion 

#pragma region Find

std::size_t FieaGameEngine::Datum::Find(int value) const
{
	assert(_type == DatumTypes::Integer);
	std::size_t position{ 0 };
	while (position < _size)
	{
		if (_data.i[position] == value)
		{
			break;
		}
		++position;
	}

	return position;
}

std::size_t FieaGameEngine::Datum::Find(float value) const
{
	assert(_type == DatumTypes::Float);
	std::size_t position{ 0 };
	while (position < _size)
	{
		if (_data.f[position] == value)
		{
			break;
		}
		++position;
	}

	return position;
}

std::size_t FieaGameEngine::Datum::Find(const std::string& value) const
{
	assert(_type == DatumTypes::String);
	std::size_t position{ 0 };
	while (position < _size)
	{
		if (_data.s[position] == value)
		{
			break;
		}
		++position;
	}

	return position;
}

std::size_t FieaGameEngine::Datum::Find(const glm::vec4& value) const
{
	assert(_type == DatumTypes::Vector);
	std::size_t position{ 0 };
	while (position < _size)
	{
		if (_data.v[position] == value)
		{
			break;
		}
		++position;
	}

	return position;
}

std::size_t FieaGameEngine::Datum::Find(const glm::mat4& value) const
{
	assert(_type == DatumTypes::Matrix);
	std::size_t position{ 0 };
	while (position < _size)
	{
		if (_data.m[position] == value)
		{
			break;
		}
		++position;
	}

	return position;
}

std::size_t FieaGameEngine::Datum::Find(const RTTI* value) const
{
	assert(_type == DatumTypes::Pointer);
	std::size_t position{ 0 };
	while (position < _size)
	{
		if (_data.p[position] == value)
		{
			break;
		}
		++position;
	}

	return position;
}

#pragma endregion

#pragma region ToString
std::string FieaGameEngine::Datum::ToString(std::size_t index)
{
	assert(index < _size);
	assert(_type != DatumTypes::Unknown);
	ToStringFunction func = _toStringFunctions[static_cast<int>(_type)];
	assert(func != nullptr);
	return (this->*func)(index);
}

std::string FieaGameEngine::Datum::ToStringInt(std::size_t index)
{
	assert(_type == DatumTypes::Integer);
	return std::to_string(_data.i[index]);
}

std::string FieaGameEngine::Datum::ToStringFloat(std::size_t index)
{
	assert(_type == DatumTypes::Float);
	return std::to_string(_data.f[index]);
}

std::string FieaGameEngine::Datum::ToStringString(std::size_t index)
{
	assert(_type == DatumTypes::String);
	return _data.s[index];
}

std::string FieaGameEngine::Datum::ToStringVector(std::size_t index)
{
	assert(_type == DatumTypes::Vector);
	return glm::to_string(_data.v[index]);
}

std::string FieaGameEngine::Datum::ToStringMatrix(std::size_t index)
{
	assert(_type == DatumTypes::Matrix);
	return glm::to_string(_data.m[index]);
}

std::string FieaGameEngine::Datum::ToStringPointer(std::size_t index)
{
	assert(_type == DatumTypes::Pointer);
	return _data.p[index]->ToString();
}

#pragma endregion

#pragma region SetFromString

void FieaGameEngine::Datum::SetFromString(const std::string& string, std::size_t index)
{
	assert(_type != DatumTypes::Unknown && _type != DatumTypes::Pointer);
	SetFromStringFunction func = _setFromStringFunctions[static_cast<int>(_type)];
	assert(func != nullptr);
	(this->*func)(string, index);
}

void FieaGameEngine::Datum::SetIntFromString(const std::string& string, std::size_t index)
{
	Set(std::stoi(string), index);
}

void FieaGameEngine::Datum::SetFloatFromString(const std::string& string, std::size_t index)
{
	Set(std::stof(string), index);
}

void FieaGameEngine::Datum::SetVectorFromString(const std::string& string, std::size_t index)
{
	glm::vec4 vec{};
	int result = sscanf_s(string.c_str(), " vec4(%f, %f, %f, %f) ", &vec.x, &vec.y, &vec.z, &vec.w);
	assert(result == 4);
	if (result == 4)
	{
		Set(vec, index);
	}
}

void FieaGameEngine::Datum::SetMatrixFromString(const std::string& string, std::size_t index)
{
	glm::vec4 vec1{};
	glm::vec4 vec2{};
	glm::vec4 vec3{};
	glm::vec4 vec4{};
	int result = sscanf_s(string.c_str(), " mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f)) ",
		&vec1.x, &vec1.y, &vec1.z, &vec1.w,
		&vec2.x, &vec2.y, &vec2.z, &vec2.w,
		&vec3.x, &vec3.y, &vec3.z, &vec3.w,
		&vec4.x, &vec4.y, &vec4.z, &vec4.w
		);
	assert(result == 16);
	if (result == 16)
	{
		Set(glm::mat4{ vec1, vec2, vec3, vec4 }, index);
	}
}

void FieaGameEngine::Datum::PushBackVectorFromString(const std::string& string)
{
	glm::vec4 vec{};
	int result = sscanf_s(string.c_str(), " vec4(%f, %f, %f, %f) ", &vec.x, &vec.y, &vec.z, &vec.w);
	assert(result == 4);
	if (result == 4)
	{
		PushBack(vec);
	}
}

void FieaGameEngine::Datum::PushBackMatrixFromString(const std::string& string)
{
	glm::vec4 vec1{};
	glm::vec4 vec2{};
	glm::vec4 vec3{};
	glm::vec4 vec4{};
	int result = sscanf_s(string.c_str(), " mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f)) ",
		&vec1.x, &vec1.y, &vec1.z, &vec1.w,
		&vec2.x, &vec2.y, &vec2.z, &vec2.w,
		&vec3.x, &vec3.y, &vec3.z, &vec3.w,
		&vec4.x, &vec4.y, &vec4.z, &vec4.w
	);
	assert(result == 16);
	if (result == 16)
	{
		PushBack(glm::mat4{ vec1, vec2, vec3, vec4 });
	}
}

#pragma endregion