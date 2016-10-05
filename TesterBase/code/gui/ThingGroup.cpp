#include <gui/ThingGroup.h>

gui::ThingGroup::ThingGroup()
{
}

gui::ThingGroup::~ThingGroup()
{
	Destroy();
}

void gui::ThingGroup::Destroy()
{
	for( std::list< Thing * >::iterator itr = m_things.begin(), end = m_things.end(); itr != end; ++itr )
	{
		delete *itr;
	}
	m_things.clear();
	m_thingMap.clear();
}

void gui::ThingGroup::Update( float delta, Input & input )
{
	for( std::list< Thing * >::iterator itr = m_things.begin(), end = m_things.end(); itr != end; ++itr )
	{
		(*itr)->Update( delta, input );
	}
}

void gui::ThingGroup::Draw()
{
	for( std::list< Thing * >::iterator itr = m_things.begin(), end = m_things.end(); itr != end; ++itr )
	{
		(*itr)->Draw();
	}
}

void gui::ThingGroup::AddThing( Thing * thing, const std::string name )
{
	m_things.push_back( thing );
	
	if ( ! name.empty() )
	{
		m_thingMap[ name ] = thing;
	}
}

gui::Thing * gui::ThingGroup::FindThing( const std::string name ) const
{
	std::map< std::string, Thing * >::const_iterator itr = m_thingMap.find( name );
	return itr == m_thingMap.end() ? 0 : itr->second;
}

