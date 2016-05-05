#pragma once

#include <dxi/events/Listener.h>
#include <list>

namespace dxi
{
	namespace events
	{
        // Multiple listeners per one event.
        class ListenerContainer : public Listener
        {
        public:
            void AddListener( Listener::shared_ptr listener );
            void RemoveListener( Listener::shared_ptr listener );
            void RemoveAllListeners();
            size_t NumberOfListeners();
            void operator()( unify::Any & eventData );

        private:
            std::list< Listener::shared_ptr > m_listeners;
        };
	}
}
