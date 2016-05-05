// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <vector>
#include <dxi/animation/Frame.h>

namespace dxi
{
	namespace animation
	{
		class Group;
		typedef unsigned int FrameIndex;
		typedef std::vector< Frame > FrameList;

		class Sequence
		{
		public:
			Sequence( Group * parent, bool loops = true );
			~Sequence();
			void Add( const Frame & frame );
			const Frame & GetBegin() const;
			const Frame & GetFrameByIndex( FrameIndex index ) const;

			// Modifies time so that it matches this Sequences expectations (clamps to its timeline).
			const Frame & GetFrameByTime( unify::Seconds & time ) const;
			unsigned int FrameCount() const;
			unify::Seconds Duration() const;
			void SetLoops( bool loops );
			bool Loops() const;
		private:
			Group * m_parent;
			FrameList m_frames;
			unify::Seconds m_totalDuration;
			bool m_loops;
		};
	} // namespace animation
} // namespace dxi