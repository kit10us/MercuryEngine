// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/pathing/Path.h>
#include <dxi/pathing/PathController.h>
#include <qxml/Document.h>
#include <map>
#include <string>

namespace dxi
{
	namespace pathing
	{
		class PathSystem
		{
		public:
			class PathSource
			{
			public:
				virtual ~PathSource() {}

				virtual bool FindPosition( const std::string & name, const unify::V3< float > * & position ) const = 0;
				virtual bool FindAxis( const std::string & name, const unify::Quaternion * & axis ) const = 0;
				virtual bool FindFrame( const std::string & name, const unify::Frame * & frame ) const = 0;
				virtual void ListPositions( std::vector< std::string > & list ) const = 0;
				virtual void ListAxes( std::vector< std::string > & list ) const = 0;
				virtual void ListFrames( std::vector< std::string > & list ) const = 0;
			};

			PathSystem();
			~PathSystem();

			void AddSource( PathSource * source );
			void RemoveSource( PathSource * source );

			void AddPosition( const std::string & name, const unify::V3< float > & position );
			void AddAxis( const std::string & name, const unify::Quaternion & axis );
			void AddFrame( const std::string & name, const unify::Frame & frame );
			void AddPath( const std::string & name, Path::shared_ptr path );
			bool FindPosition( const std::string & name, const unify::V3< float > * & position ) const;
			bool FindAxis( const std::string & name, const unify::Quaternion * & axis ) const;
			bool FindFrame( const std::string & name, const unify::Frame * & frame ) const;
			Path::shared_ptr GetPath( const std::string & name ) const;
			void ListPositions( std::vector< std::string > & list ) const;
			void ListAxes( std::vector< std::string > & list ) const;
			void ListFrames( std::vector< std::string > & list ) const;
			void ListPaths( std::vector< std::string > & list ) const;
			void LoadFromFile( const unify::Path & path );
			PathController::shared_ptr CreatePathController( const std::string & pathName, bool looping = false ) const;

		private:
			Modifier::shared_ptr FactoryModifierFromXMLElement( const qxml::Element * node ) const;
			Modifier::shared_ptr CreateModifierParallelFromXMLElement( const qxml::Element * node ) const;
			Modifier::shared_ptr CreateModifierSplitFromXMLElement( const qxml::Element * node ) const;
			Modifier::shared_ptr CreateModifierRotateAboutFromXMLElement( const qxml::Element * node ) const;
			Modifier::shared_ptr CreateModifierAttachToFromXMLElement( const qxml::Element * node ) const;
			Modifier::shared_ptr CreateModifierLookAtFromXMLElement( const qxml::Element * node ) const;

			std::map< std::string, unify::V3< float > >  m_positions;
			std::map< std::string, unify::Quaternion >  m_axes;
			std::map< std::string, unify::Frame > m_frames;
			std::map< std::string, Path::shared_ptr > m_paths;
			std::list< PathSource * > m_sources;
		};
	}
}