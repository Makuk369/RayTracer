#pragma once
#include "Utils.hpp"

class Interval
{
    public:
        float min;
        float max;

        Interval() : min(+RTUtilVars::INFINITE_F), max(-RTUtilVars::INFINITE_F) {} // Default interval is empty
        Interval(float min, float max) : min(min), max(max) {}

        float Size() const { return max - min; }

        bool Contains(float x) const { return min <= x && x <= max; }
        bool Surrounds(float x) const { return min < x && x < max; }

    private:
};
