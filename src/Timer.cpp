#include "headers/Timer.hpp"

Timer::Timer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;
    mLastFrameTicks = 0;

    mPaused = false;
    mStarted = false;
};

void Timer::Start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
};

void Timer::Stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
};

void Timer::Pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
};

void Timer::Unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
};

Uint64 Timer::getTicks()
{
	//The actual timer time
	Uint64 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
};

float Timer::getDeltaTime()
{
    Uint64 currentFrameTicks = SDL_GetTicksNS();
    float deltaTime = (currentFrameTicks - mLastFrameTicks) / 1000000000.f; // 1 sec = 1000000000 nanosec
    mLastFrameTicks = currentFrameTicks;
    return deltaTime;
};

bool Timer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
};

bool Timer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
};