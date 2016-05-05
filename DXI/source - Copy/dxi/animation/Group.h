// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/animation/Sequence.h>
#include <list>
#include <map>
#include <memory>

namespace dxi
{
	namespace animation
	{
		class SpriteManager;
		typedef std::string SequenceName;
		typedef std::vector< std::shared_ptr< Sequence > > SequenceArray;
		typedef std::map< const SequenceName, unsigned int > SequenceMap;

		class Group
		{
			SpriteManager * m_parent;
			SequenceArray m_sequenceArray;
			SequenceMap m_sequenceMap;
		public:
			Group( SpriteManager * parent );
			~Group();
			void Add( const SequenceName & name, std::shared_ptr< Sequence > sequence );
			bool SequenceExists( const SequenceName & name ) const;
			const std::shared_ptr< Sequence > & FindSequence( const SequenceName & name ) const;
			const std::shared_ptr< Sequence > & GetSequence( unsigned int index ) const;
			const SequenceName & GetSequenceName( unsigned int index ) const;
			unsigned int GetSequenceIndex( const SequenceName & name ) const;
			unsigned int Count() const;
		};
	} // namespace animation
} // namespace dxi