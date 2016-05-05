// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <map>
#include <memory>
#include <memory>
#include <dxi/TextureManager.h>
#include <dxi/animation/Group.h>

namespace dxi
{
	namespace animation
	{
		typedef std::string GroupName;
		typedef std::map< GroupName, std::shared_ptr< Group > > GroupMap;

		class SpriteManager
		{
		public:
			typedef std::shared_ptr< SpriteManager > shared_ptr;
			typedef std::weak_ptr< SpriteManager > weak_ptr;

			SpriteManager( TextureManager::shared_ptr textureManager );
			~SpriteManager();
			void LoadFromFile( const unify::Path & filePath );
			void AddGroup( const GroupName & name, std::shared_ptr< Group > group );
			void RemoveGroup( const GroupName & name );
			bool GroupExists( const GroupName & name ) const;
			const std::shared_ptr< Group > & FindGroup( const GroupName & name ) const;
			const GroupName & GetGroupName( const unsigned int index ) const;
			unsigned int Count() const;
		private:
			TextureManager::weak_ptr m_textureManager;
			GroupMap m_groups;
		};
	} // namespace animation
} // namespace dxi