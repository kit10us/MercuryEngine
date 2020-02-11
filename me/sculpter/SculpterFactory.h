// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/sculpter/IShapeCreator.h>
#include <me/render/Mesh.h>
#include <rm/ResourceManager.h>
#include <me/game/Game.h>

namespace me
{
	namespace sculpter
	{
		class SculpterFactory : public rm::ISourceFactory< me::render::Geometry >
		{
		public:
			typedef std::shared_ptr< rm::ISourceFactory< me::render::Geometry> > ptr;

			SculpterFactory( me::game::Game * gameInstance );

			void AddShapeCreator( std::string name, IShapeCreator::ptr creator );
			IShapeCreator* FindShapeCreator( std::string name ) const;

		public: // rm::ISourceFactory
			me::render::Geometry::ptr Produce( unify::Path source, unify::Parameters parameters ) override;
			me::render::Geometry::ptr Produce( unify::Parameters parameters ) override;

		private:
			me::game::Game *m_game;
			std::map< std::string, IShapeCreator::ptr, unify::string::CaseInsensitiveLessThanTest > m_creators;
		};
	}
}