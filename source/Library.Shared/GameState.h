#pragma once
#include "EventQueue.h"

namespace FieaGameEngine
{
	class GameState final
	{
		inline static EventQueue _eventQueue{};
		inline static GameTime _gameTime{};

	public:
		/// <summary>
		/// Enqueues event to the globally accesible EventQueue;
		/// </summary>
		/// <param name="event">Event to Queue.</param>
		/// <param name="gameTime">GameTime instance to use for timers.</param>
		/// <param name="delay">Amouunt of time to delay delivery of message for.</param>
		static void EnqueueEvent(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, std::chrono::milliseconds delay = std::chrono::milliseconds{ 0 });
		/// <summary>
		/// Calls Update on the EventQueue.
		/// </summary>
		/// <param name="gameTime"> GameTime instance to pass in. Defaults to the global GameTime.</param>
		static void Update(const GameTime& gameTime = _gameTime);
		/// <summary>
		/// Resets the GameState by clearing the EventQueue.
		/// </summary>
		static void Reset();
		/// <summary>
		/// Returns the global GameTime instance.
		/// </summary>
		/// <returns>Global GameTime instance.</returns>
		static GameTime& GlobalGameTimeInstance() { return _gameTime; };
	};

}
