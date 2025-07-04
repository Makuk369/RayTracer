#pragma once
#include <SDL3/SDL.h>

class Timer
{
    public:
		//Initializes variables
		Timer();

		//The various clock actions
		void Start();
		void Stop();
		void Pause();
		void Unpause();

		//Gets the timer's time
		Uint64 getTicks();

        //Gets deltaTime in seconds
        //Should be called at the start of every frame
        float getDeltaTime();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint64 mStartTicks;

		//The ticks stored when the timer was paused
		Uint64 mPausedTicks;

		Uint64 mLastFrameTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};