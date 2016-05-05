#pragma once

#include <unify/Frame.h>
#include <dxi/pathing/Modifier.h>
#include <unify/TimeDelta.h>
#include <unify/Unify.h>
//#include <memory>	

namespace dxi
{
	namespace pathing
	{
		class OnDone
		{
		public:
			typedef std::shared_ptr< OnDone > shared_ptr;

			virtual void operator()( class PathController * controller, unify::Seconds excess ) = 0;
		};

		class Path
		{
		public:
			typedef std::shared_ptr< Path > shared_ptr;

			Path();
			Path( Modifier::shared_ptr modifier, unify::Seconds duration );
			virtual ~Path();
			float Update( unify::Seconds totalElapsed, unify::Frame * frame );

		private:
			unify::Seconds m_duration;
			unify::Seconds m_totalElapsed;
			Modifier::shared_ptr m_modifier;
			std::shared_ptr< OnDone > m_onDone;
		};

		class Restart : public OnDone
		{
		public:
			void operator()( PathController * controller, unify::Seconds excess );
		};
	}
}