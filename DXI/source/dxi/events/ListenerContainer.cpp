#include <dxi/events/ListenerContainer.h>

using namespace dxi;
using namespace events;

void ListenerContainer::AddListener( Listener::shared_ptr listener )
{
    m_listeners.push_back( listener );
}

void ListenerContainer::RemoveListener( Listener::shared_ptr listener )
{
    m_listeners.remove( listener );
}

void ListenerContainer::RemoveAllListeners()
{
    m_listeners.clear();
}

size_t ListenerContainer::NumberOfListeners()
{
    return m_listeners.size();
}

void ListenerContainer::operator()( unify::Any & eventData )
{
    class FireListener
    {
    public:
        FireListener( unify::Any & eventData ) : m_eventData( eventData )
        {
        }

        void operator()( Listener::shared_ptr listener )
        {
            (*listener.get())( m_eventData );
        }
    private:
        unify::Any & m_eventData;
    };
    std::for_each( m_listeners.begin(), m_listeners.end(), FireListener( eventData ) );
}