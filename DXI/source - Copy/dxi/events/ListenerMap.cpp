#include <dxi/events/ListenerMap.h>
#include <unify/Exception.h>

using namespace dxi;
using namespace events;

ListenerMap::ListenerMap()
{
}

ListenerMap::~ListenerMap()
{
}

void ListenerMap::DefineListener( const std::string & name )
{
    m_definedListeners.push_back( name );
}

void ListenerMap::Add( const std::string & name, Listener::shared_ptr listener )
{
    if ( m_definedListeners.empty() == false )
    {
        std::list< std::string >::const_iterator itr = std::find( m_definedListeners.begin(), m_definedListeners.end(), name );
        if ( itr == m_definedListeners.end() )
        {
            throw unify::Exception( "Failed to find defined listener! ( " + name + " )!" );
        }
    }

    Map::iterator itr = m_listeners.find( name );
    std::shared_ptr< ListenerContainer > listenerContainer; 

    if ( itr == m_listeners.end() )
    {
        listenerContainer.reset( new ListenerContainer );
        m_listeners[ name ] = listenerContainer;
    }
    else
    {
        listenerContainer = itr->second;
    }
    listenerContainer->AddListener( listener );
}

void ListenerMap::Fire( const std::string & name, unify::Any & eventData )
{
    Map::const_iterator itr = m_listeners.find( name );
    if ( itr == m_listeners.end() )
    {
        // Do nothing - not having a corresponding listener is not a failure.
    }
    else
    {
        ( *itr->second )( eventData );
    }
}

bool ListenerMap::Has( const std::string & name ) const
{
    Map::const_iterator itr = m_listeners.find( name );
    return itr != m_listeners.end();
}

bool ListenerMap::CompareNoCase::operator()( const std::string & a, const std::string & b ) const
{
    return _stricmp( a.c_str(), b.c_str() ) < 0;
}

/////////////////////////////////////////////////

ListenerMapOwner::ListenerMapOwner()
{
}

ListenerMapOwner::~ListenerMapOwner()
{
}

ListenerMap & ListenerMapOwner::GetListenerMap()
{
    return m_listenerMap;
}
