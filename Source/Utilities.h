/*
  ==============================================================================

    Utilities.h
    Created: 9 Sep 2024 12:11:17am
    Author:  guico

  ==============================================================================
*/

#pragma once
#include <string>

class Utilities
{
    public:
        /** Static function to transform the total seconds in formated min:sec **/
		static std::string formatTotalTime(double timeInSeconds);

        /** Static function to transform the current reprodution 
        time from seconds to formated min:sec **/
		static std::string formatCurrentTime(double timeInSeconds, double pos);

};
