/*
  ==============================================================================

    Utilities.cpp
    Created: 9 Sep 2024 12:11:17am
    Author:  guico

  ==============================================================================
*/

#include "Utilities.h"

std::string Utilities::formatTotalTime(double timeInSeconds)
{
	//total time display
	int minutes = static_cast<int>(timeInSeconds) / 60; //get min without decimals
	int seconds = static_cast<int>(timeInSeconds) % 60; //get seconds without decimals
	std::string timeStringTotal =
		(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + //ternary operator + min
		(seconds < 10 ? "0" : "") + std::to_string(seconds);        //ternary operator + sec

	return timeStringTotal;
}

std::string Utilities::formatCurrentTime(double timeInSeconds, double pos)
{
	//current time display
	int minutesCurrent = static_cast<int>(pos * timeInSeconds) / 60; //get min without decimals
	int secondsCurrent = static_cast<int>(pos * timeInSeconds) % 60; //get seconds without decimals
	std::string timeStringCurrent =
		(minutesCurrent < 10 ? "0" : "") + std::to_string(minutesCurrent) + ":" + //ternary operator + min
		(secondsCurrent < 10 ? "0" : "") + std::to_string(secondsCurrent);        //ternary operator + sec

	return timeStringCurrent;
}
