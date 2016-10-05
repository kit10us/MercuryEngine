#pragma once

#include <gui/Thing.h>
#include <list>
#include <string>
#include <map>

namespace gui
{
	class ThingGroup : public Thing
	{
	public:
		ThingGroup();
		virtual ~ThingGroup();
		virtual void Update( float delta, Input & input );
		virtual void Draw();

		void Destroy();

		void AddThing( Thing * thing, const std::string name = std::string() );
		Thing * FindThing( const std::string name ) const;

	private:
		std::list< Thing * > m_things;
		std::map< std::string, Thing * > m_thingMap; // Does NOT manage memory.
	};
}

