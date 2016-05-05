#pragma once

#include <string>
#include <map>
#include <dxi/events/ListenerContainer.h>

namespace dxi
{
    namespace events
    {
        /// A mapping between listeners and names, allowing simplified firing and tracking of listeners.
        class ListenerMap
        {
        public:
            ListenerMap();
            ~ListenerMap();
            
            void DefineListener( const std::string & name );
            void Add( const std::string & name, Listener::shared_ptr listener );
            void Fire( const std::string & name, unify::Any & eventData );
            bool Has( const std::string & name ) const;

        private:
            struct CompareNoCase
            {
                bool operator()( const std::string & a, const std::string & b ) const;
            };
            typedef std::map< std::string, std::shared_ptr< ListenerContainer >, CompareNoCase > Map;
            Map m_listeners;

            // Optional list of expected listeners. If empty, we allow any listeners.
            std::list< std::string > m_definedListeners;
        };

        /// Base class to simplify integrating a ListenerMap.
        class ListenerMapOwner
        {
            events::ListenerMap m_listenerMap;
        public:
            ListenerMapOwner();
            virtual ~ListenerMapOwner();

            events::ListenerMap & GetListenerMap();
        };
    }
}