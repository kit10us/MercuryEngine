// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/pathing/Modifier.h>	
#include <dxi/pathing/Path.h>
#include <unify/TimeDelta.h>

namespace dxi
{
	namespace pathing
	{
		/// A PathController is an instance of a path. It links the Path and the Frame being controlled by the path, and all data pertaining to the relationship.
		class PathController
		{
		public:
			typedef std::shared_ptr< PathController > shared_ptr;

			explicit PathController( Path::shared_ptr path = Path::shared_ptr(), unify::Frame * frame = 0, OnDone::shared_ptr onDone = OnDone::shared_ptr() );
			virtual ~PathController();
			void SetFrame( unify::Frame * frame );
			void SetPath( Path::shared_ptr path );
			void SetOnDone( OnDone::shared_ptr onDone );
			void SetLooping( bool looping );
			bool GetLooping() const;
			void Update( unify::Seconds elapsed );
			void Restart( unify::Seconds elapsed = 0 );
			void Pause();
			void Resume();

		private:
			unify::Frame * m_frame;
			Path::shared_ptr m_path;
			OnDone::shared_ptr m_onDone;
			bool m_active;
			bool m_looping;
			unify::Seconds m_totalElapsed;
		};
	}
}