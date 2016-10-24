// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dximouse/Mouse.h>

using namespace dximouse;
using namespace dxi;
using namespace input;

Mouse::Mouse( dxi::core::IGame * game )
	: m_game( game )
{
	m_subSources.resize( game->GetOS()->RendererCount() );
	m_subSourcesUpdated.resize( game->GetOS()->RendererCount() );
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

State Mouse::GetState( size_t subSource, std::string name, std::string condition ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return input::State::Invalid;
	}

	const Source & source = m_subSources[subSource];

	if ( unify::StringIs( name, "LeftButton" ) )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			return source.LeftButton ? State::True : State::False;
		}
		else if ( unify::StringIs( condition, "Pressed" ) )
		{
			return source.LeftButtonPressed ? State::True : State::False;
		}
	}

	if ( unify::StringIs( name, "RightButton" ) )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{	
			return source.RightButton ? State::True : State::False;
		}
		else if ( unify::StringIs( condition, "Pressed" ) )
		{
			return source.RightButtonPressed ? State::True : State::False;
		}
	}

	if ( unify::StringIs( name, "MiddleButton" ) )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			return source.MiddleButton ? State::True : State::False;
		}
		else if ( unify::StringIs( condition, "Pressed" ) )
		{
			return source.MiddleButtonPressed ? State::True : State::False;
		}
	}

	return State::Invalid;
}

bool Mouse::HasValue( size_t subSource, std::string name ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return false;
	}

	if ( unify::StringIs( name, "PositionX" ) )
	{
		return true;
	}

	if ( unify::StringIs( name, "PositionY" ) )
	{
		return true;
	}

	if ( unify::StringIs( name, "MouseWheel" ) )
	{
		return true;
	}

	if ( unify::StringIs( name, "ChangeX" ) )
	{
		return true;
	}

	if ( unify::StringIs( name, "ChangeY" ) )
	{
		return true;
	}

	return false;
}

float Mouse::GetValue( size_t subSource, std::string name ) const
{
	if ( subSource >= m_subSources.size() )
	{
		return 0.0f;
	}

	const Source & source = m_subSources[subSource];

	if ( unify::StringIs( name, "PositionX" ) )
	{
		return source.PositionX;
	}

	if ( unify::StringIs( name, "PositionY" ) )
	{
		return source.PositionY;
	}

	if ( unify::StringIs( name, "MouseWheel" ) )
	{
		return source.MouseWheel;
	}

	if ( unify::StringIs( name, "ChangeX" ) )
	{
		return source.ChangeX;
	}

	if ( unify::StringIs( name, "ChangeY" ) )
	{
		return source.ChangeY;
	}

	return 0.0f;
}

bool Mouse::SetState( size_t subSource, std::string name, std::string condition, bool set )
{
	if ( subSource >= m_subSources.size() ) return false;

	Source & source = m_subSourcesUpdated[subSource];

	if ( unify::StringIs( name, "LeftButton" ) )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			source.LeftButton = set;
			return true;
		}
		return false;
	}

	if ( unify::StringIs( name, "RightButton" ) )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			source.RightButton = set;
			return true;
		}
		return false;
	}

	if ( unify::StringIs( name, "MiddleButton" ) )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			source.MiddleButton = set;
			return true;
		}
		return false;
	}

	return false;
}

bool Mouse::SetValue( size_t subSource, std::string name, float value )
{
	if ( subSource >= m_subSources.size() ) return false;

	Source & source = m_subSourcesUpdated[subSource];

	if ( unify::StringIs( name, "PositionX" ) )
	{
		source.PositionX = value;
	}

	if ( unify::StringIs( name, "PositionY" ) )
	{
		source.PositionY = value;
	}

	if ( unify::StringIs( name, "MouseWheel" ) )
	{
		source.MouseWheel = value;
	}

	return true;
}

