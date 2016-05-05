#include <dxi/pathing/Path.h>
#include <dxi/pathing/PathController.h>

using namespace dxi;
using namespace pathing;

Path::Path()
{
}

Path::Path( Modifier::shared_ptr modifier, unify::Seconds duration )
: m_modifier( modifier ), m_duration( duration )
{
}

Path::~Path()
{
}

float Path::Update( unify::Seconds totalElapsed, unify::Frame * frame )
{
	float excessTime( 0 );
	if( totalElapsed > m_duration )
	{
		excessTime = totalElapsed - m_duration;
	}

	if( m_modifier && frame )
	{
		m_modifier->AtTime( (totalElapsed - excessTime) / m_duration, frame );
	}

	return excessTime;
}

void Restart::operator()( PathController * controller, unify::Seconds excess )
{
	controller->Restart( excess );
}
