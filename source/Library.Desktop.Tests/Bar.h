#pragma once

#include "RTTI.h"
#include <cstdint>

namespace UnitTests
{
	class Bar final : public FieaGameEngine::RTTI
	{

		RTTI_DECLARATIONS(Bar, FieaGameEngine::RTTI);

	public:
		explicit Bar(std::int32_t data = 0);
		Bar(const Bar& rhs);
		Bar(Bar&& rhs) noexcept;
		Bar& operator=(const Bar& rhs);
		Bar& operator=(Bar&& rhs) noexcept;
		virtual ~Bar();

		std::int32_t Data() const;
		void SetData(std::int32_t data);

	private:
		std::int32_t* _data;
	};
}
