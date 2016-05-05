// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>

namespace unify
{
	typedef float Seconds; // TODO: TEMP

    class TimeDelta
    {
        float m_ms;

        TimeDelta( float ms );
    public:
        static TimeDelta TimeDeltaInMS( float ms );
        static TimeDelta TimeDeltaInSeconds( float seconds );

        float GetMS() const;
        float GetSeconds() const;
    };
}