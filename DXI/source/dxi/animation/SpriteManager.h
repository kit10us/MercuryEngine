// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <dxi/animation/Group.h>
#include <map>
#include <memory>

namespace dxi
{
	namespace animation
	{
		typedef std::string GroupName;
		typedef std::map< GroupName, std::shared_ptr< Group > > GroupMap;

		class SpriteManager
		{
		public:
			typedef std::shared_ptr< SpriteManager > ptr;

			SpriteManager( me::IGame * game );
			~SpriteManager();

			me::IGame * GetGame();
			const me::IGame * GetGame() const;

			void LoadFromFile( const unify::Path & filePath );
			void AddGroup( const GroupName & name, std::shared_ptr< Group > group );
			void RemoveGroup( const GroupName & name );
			bool GroupExists( const GroupName & name ) const;
			const std::shared_ptr< Group > & FindGroup( const GroupName & name ) const;
			const GroupName & GetGroupName( const unsigned int index ) const;
			unsigned int Count() const;
		private:
			me::IGame * m_game;
			GroupMap m_groups;
		};
	} // namespace animation
} // namespace dxi