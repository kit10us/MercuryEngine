#pragma once
#include <unify/Any.h>
#include <memory>

/// Tutorial:
/// Add a listener member variable (such as m_onMyEvent) to a class.
/// When the event happens, you call the listener functor passing in any value as a parameter (m_onMyEvent( 12 ))
///

namespace dxi
{
	namespace events
	{
		/// <summary>
		/// Callback functor for events.
		/// </summary>
        class Listener
        {
        public:
            typedef std::shared_ptr< Listener > shared_ptr;

            virtual ~Listener() {}
            virtual void operator()( unify::Any & eventData ) = 0;
        };
	}
}
