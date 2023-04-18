#pragma once
#include "Stack.h"

namespace FieaGameEngine
{
		template <typename T>
		template <typename...Args>
		inline void Stack<T>::Emplace(Args&&...args)
		{
			_list.EmplaceFront(std::forward<Args>(args)...);
		}

		template <typename T>
		inline void Stack<T>::Push(const_reference value)
		{
			_list.EmplaceFront(value);
		}

		template <typename T>
		inline void Stack<T>::Push(rvalue_reference value)
		{
			_list.EmplaceFront(std::move(value));
		}

		template <typename T>
		inline void Stack<T>::Pop()
		{
			_list.PopFront();
		}

		template <typename T>
		inline typename Stack<T>::reference Stack<T>::Top()
		{
			return _list.Front();
		}

		template <typename T>
		inline typename Stack<T>::size_type Stack<T>::Size() const
		{
			return _list.Size();
		}

		template <typename T>
		inline bool Stack<T>::IsEmpty() const
		{
			return _list.IsEmpty();
		}

		template <typename T>
		inline void Stack<T>::Clear()
		{
			_list.Clear();
		}
}

