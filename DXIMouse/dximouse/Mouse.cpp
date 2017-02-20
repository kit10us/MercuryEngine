// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dximouse/Mouse.h>

using namespace dximouse;
using namespace me;
using namespace input;

std::vector< std::string > g_ButtonNames =
{
	"LeftButton",
	"RightButton",
	"MiddleButton"
};

std::vector< std::string > g_AnalogNames =
{
	"PositionX",
	"PositionY",
	"MouseWheel",
	"ChangeX",
	"ChangeY"
};

Mouse::Mouse( IGame * game )
	: m_game( game )
{
	m_subSources.resize( game->GetOS()->RendererCount() );
	m_subSourcesUpdated.resize( game->GetOS()->RendererCount() );

	size_t index = 0;
	for ( auto && name : g_ButtonNames )
	{
		m_nameToIndex[ name ] = index++;
	}
	for ( auto && name : g_AnalogNames )
	{
		m_nameToIndex[ name ] = index++;
	}
}

Mouse::~Mouse()
{
}

std::string Mouse::Name() const
{
	return "Mouse";
}

void Mouse::Update()
{
	for ( size_t i = 0; i < m_subSources.size(); ++i )
	{
		Source & updated = m_subSourcesUpdated[i];
		Source & source = m_subSources[i];

		source.LeftButtonPressed = false;
		source.RightButtonPressed = false;
		source.MiddleButtonPressed = false;

		if ( updated.LeftButton == false && source.LeftButton == true )
		{
			source.LeftButtonPressed = true;
		}
		if ( updated.RightButton == false && source.RightButton == true )
		{
			source.RightButtonPressed = true;
		}
		if ( updated.MiddleButton == false && source.MiddleButton == true )
		{
			source.MiddleButtonPressed = true;
		}

		source.LeftButton = updated.LeftButton;
		source.RightButton = updated.RightButton;
		source.MiddleButton = updated.MiddleButton;

		source.ChangeX = 0;
		if ( source.PositionX != -1.0f )
		{
			source.ChangeX = source.PositionX - updated.PositionX;
		}
		source.PositionX = updated.PositionX;

		source.ChangeY = 0;
		if ( source.PositionY != -1.0f )
		{
			source.ChangeY = source.PositionY - updated.PositionY;
		}
		source.PositionY = updated.PositionY;

		source.MouseWheel = updated.MouseWheel;
		updated.MouseWheel = 0;
	}
}

size_t Mouse::SubSourceCount() const
{
	return m_subSources.size();
}

size_t Mouse::InputCount( size_t subSource ) const
{
	return g_ButtonNames.size() + g_AnalogNames.size();
}

std::string Mouse::InputName( size_t subSource, size_t index ) const
{
	if ( index < g_ButtonNames.size() )
	{	
		return g_ButtonNames[ index ];
	}
	else if ( index < g_AnalogNames.size() )
	{
		return g_AnalogNames[ index - g_ButtonNames.size() ];
	}
	else
	{
		return std::string();
	}
}

size_t Mouse::InputIndex( size_t subSource, std::string name ) const
{
	auto && itr = m_nameToIndex.find( name );
	return itr == m_nameToIndex.end() ? MAXSIZE_T : itr->second;
}

size_t Mouse::InputConditionCount( size_t subSource, size_t index ) const
{	
	if ( index < g_ButtonNames.size() )
	{
		return 2;
	}
	else
	{
		return MAXSIZE_T;
	}
}

std::string Mouse::InputConditionName( size_t subSource, size_t index, size_t condition ) const
{
	if ( index < g_ButtonNames.size() )
	{
		switch ( index )
		{
		case 0:
			return "Down";
		case 1:
			return "Pressed";
		default:
			return std::string();
		}
	}
	else
	{
		return std::string();
	}
}

size_t Mouse::InputConditionIndex( size_t subSource, size_t index, std::string condition ) const
{
	if ( index < g_ButtonNames.size() )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			return 0;
		}
		else if ( unify::StringIs( condition, "Pressed" ) )
		{
			return 1;
		}
		else
		{
			return MAXSIZE_T;
		}
	}
	else
	{
		return MAXSIZE_T;
	}
}

State Mouse::GetState( size_t subSource, size_t index, size_t condition ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return State::Invalid;
	}

	const Source & source = m_subSources[subSource];

	switch ( index )
	{
	case 0:
		switch ( condition )
		{
		case 0:
			return source.LeftButton ? State::True : State::False;
		case 1:
			return source.LeftButtonPressed ? State::True : State::False;
		default:
			return State::Invalid;
		}
	case 1:
		switch ( condition )
		{
		case 0:
			return source.RightButton ? State::True : State::False;
		case 1:
			return source.RightButtonPressed ? State::True : State::False;
		default:
			return State::Invalid;
		}
	case 2:
		switch ( condition )
		{
		case 0:
			return source.MiddleButton ? State::True : State::False;
		case 1:
			return source.MiddleButtonPressed ? State::True : State::False;
		default:
			return State::Invalid;
		}
	default:
		return State::Invalid;
	}
}

bool Mouse::HasValue( size_t subSource, size_t index ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return false;
	}

	if ( index < g_ButtonNames.size() )
	{
		return false;
	}
	else if ( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Mouse::GetValue( size_t subSource, size_t index ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return 0.0f;
	}

	const Source & source = m_subSources[subSource];

	if ( index < g_ButtonNames.size() )
	{
		return 0.0f;
	}
	else if ( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		switch ( index - g_ButtonNames.size() )
		{
		case 0:
			return source.PositionX;
		case 1:
			return source.PositionY;
		case 2:
			return source.MouseWheel;
		case 3:
			return source.ChangeX;
		case 4:
			return source.ChangeY;
		default:
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
	return 0.0f;
}

bool Mouse::SetState( size_t subSource, size_t index, size_t condition, bool set )
{
	if ( subSource >= m_subSources.size() ) return false;

	Source & source = m_subSourcesUpdated[subSource];

	switch ( index )
	{
	case 0:
		switch ( condition )
		{
		case 0:
			source.LeftButton = set;
			return true;
		default:
			return false;
		}
	case 1:
		switch ( condition )
		{
		case 0:
			source.RightButton = set;
			return true;
		default:
			return false;
		}
	case 2:
		switch ( condition )
		{
		case 0:
			source.MiddleButton = set;
			return true;
		default:
			return false;
		}
	default:
		return false;
	}
}

bool Mouse::SetValue( size_t subSource, size_t index, float value )
{
	if ( subSource >= m_subSources.size() ) return false;

	Source & source = m_subSourcesUpdated[subSource];

	if ( index < g_ButtonNames.size() )
	{
		return false;
	}
	else if( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		switch ( index - g_ButtonNames.size() )
		{
		case 0:
			source.PositionX = value;
			return true;
		case 1:
			source.PositionY = value;
			return true;
		case 2:
			source.MouseWheel = value;
			return true;

		default:
			return false;
		}
	}

	return false;
}

