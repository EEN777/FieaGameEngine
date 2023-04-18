#include "GameState.h"

namespace FieaGameEngine
{
	void GameState::EnqueueEvent(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		_eventQueue.Enqueue(std::move(event), gameTime, delay);
	}

	void GameState::Update(const GameTime& gameTime)
	{
		_eventQueue.Update(gameTime);
	}

	void GameState::Reset()
	{
		_eventQueue.Clear();
	}
}
