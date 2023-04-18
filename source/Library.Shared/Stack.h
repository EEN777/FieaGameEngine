#pragma once
#include "SList.h"

namespace FieaGameEngine
{
	template <typename T>
	class Stack final
	{
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const reference;
		using rvalue_reference = value_type&&;

		template <typename...Args>
		void Emplace(Args&&...args);
		void Push(const_reference value);
		void Push(rvalue_reference value);
		void Pop();
		[[nodiscard]] reference Top();
		[[nodiscard]] const_reference Top() const;

		[[nodiscard]] size_type Size() const;
		[[nodiscard]] bool IsEmpty() const;

		void Clear();


	private:
		SList<value_type> _list{};
	};

}
#include "Stack.inl"
