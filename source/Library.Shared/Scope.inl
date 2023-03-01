#include "Scope.h"
#pragma once

inline FieaGameEngine::Scope* FieaGameEngine::Scope::GetParent()
{
	return _parent;
}

inline std::size_t FieaGameEngine::Scope::Size()
{
	return _size;
}

inline bool FieaGameEngine::Scope::IsEmpty()
{
	return _size == 0;
}

inline void FieaGameEngine::Scope::Reparent(Scope* parent)
{
	_parent = parent;
}
