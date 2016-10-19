// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/animation/SpriteManager.h>
#include <qxml/Document.h>
#include <dxi/core/Game.h>

using namespace dxi;
using namespace animation;

SpriteManager::SpriteManager( dxi::core::IGame * game )
	: m_game( game )
{
}

SpriteManager::~SpriteManager()
{
}

core::IGame * SpriteManager::GetGame()
{
	return m_game;
}

const core::IGame * SpriteManager::GetGame() const
{
	return m_game;
}

void SpriteManager::LoadFromFile( const unify::Path & filePath )
{
	auto textureManager = ((core::Game*)GetGame())->GetManager< Texture >();

	bool defaultLoop = true;
	qxml::Document doc( filePath );

	// Table of texture mappings.
	typedef std::string Name;
	std::map< Name, Texture::ptr > textureMapping;

	const qxml::Element * animation = doc.GetRoot()->FindFirstElement( "animation" );
	for( const auto leaf : animation->Children() )
	{
		if( leaf.IsTagName( "texture" ) )
		{
			Texture::ptr texture = textureManager->Find( leaf.GetAttribute( "source" )->GetString() );
			textureMapping[ leaf.GetAttribute( "name" )->GetString() ] = texture;
		}
		else if( leaf.IsTagName( "group" ) )
		{
			std::shared_ptr< animation::Group > group( new Group( this ) );
			for( const auto sequenceLeaf : leaf.Children() )
			{
				if( sequenceLeaf.IsTagName( "sequence" ) )
				{
					bool loop = sequenceLeaf.GetAttributeElse< bool >( "loop", defaultLoop );
					std::shared_ptr< animation::Sequence > sequence( new Sequence( group.get(), loop ) );
					for( const auto frameLeaf : sequenceLeaf.Children() )
					{
						if( frameLeaf.IsTagName( "frame" ) )
						{
							Texture::ptr texture = textureMapping[frameLeaf.GetAttribute( "texture" )->GetString() ];
							unify::TexArea area( unify::TexArea::Full() );
							if( frameLeaf.HasAttributes( "sprite" ) )
							{
								int spriteIndex = frameLeaf.GetAttribute( "sprite" )->Get< int >();
								std::string arrayName = frameLeaf.GetAttributeElse< std::string >( "array", "" );
								if( ! arrayName.empty() )
								{
									area = texture->GetSprite( arrayName, spriteIndex );
								}
								else
								{
									area = texture->GetSprite( spriteIndex );
								}
							}
							unify::Seconds duration = static_cast< unify::Seconds >( frameLeaf.GetAttributeElse< float >( "duration", 0 ) );
							animation::Frame frame( texture, area, duration );
							sequence->Add( frame );
						}
					}
					group->Add( sequenceLeaf.GetAttribute< std::string >( "name" ), sequence );
				}
			}
			AddGroup( leaf.GetAttribute< std::string >( "name" ), group );
		}
	}
}

void SpriteManager::AddGroup( const GroupName & name, std::shared_ptr< Group > group )
{
	m_groups[ name ] = group;
}

void SpriteManager::RemoveGroup( const GroupName & name )
{
	m_groups.erase( name );
}

bool SpriteManager::GroupExists( const GroupName & name ) const
{
	GroupMap::const_iterator itrGroup = m_groups.find( name );
	return ( itrGroup == m_groups.end() ) ? false : true;
}

const std::shared_ptr< Group > & SpriteManager::FindGroup( const GroupName & name ) const
{
	GroupMap::const_iterator itrGroup = m_groups.find( name );
	if( itrGroup == m_groups.end() )
	{
		throw unify::Exception( "Group not found!" );
	}
	else
	{
		return itrGroup->second;
	}
}

const GroupName & SpriteManager::GetGroupName( const unsigned int index ) const
{
	if( index >= Count() )
	{
		throw unify::Exception( "Group index out of bounds!" );
	}

	unsigned int currentIndex = 0;
	GroupMap::const_iterator itrGroup = m_groups.begin();
	for( itrGroup; itrGroup != m_groups.end() && currentIndex != index; ++itrGroup, ++currentIndex )
	{
		// empty.
	}
	return itrGroup->first;
}

unsigned int SpriteManager::Count() const
{
	return static_cast< unsigned int >( m_groups.size() );
}

