// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <map>
#include <vector>
#include <unify/Size.h>
#include <unify/TexArea.h>
#include <memory>
#include <dxi/animation/Frame.h>

namespace dxi
{
	namespace animation
	{
		class Sequence;

		class Instance
		{
			friend Sequence;
			unify::Seconds m_currentTime;
			std::shared_ptr< Sequence > m_sequence;
			Frame m_frame;
		public:
			Instance();
			Instance( const Instance & instance );
			Instance( const Frame & frame );
			Instance( std::shared_ptr< Sequence > sequence, unify::Seconds time = 0 );
			~Instance();
			bool Update( unify::Seconds elapsed );
			const Frame & GetFrame() const;
		};
	} // namespace animation
} // namespace dxi