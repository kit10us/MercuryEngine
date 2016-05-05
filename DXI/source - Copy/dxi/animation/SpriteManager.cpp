// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/animation/SpriteManager.h>
#include <qxml/Document.h>

using namespace dxi;
using namespace animation;

SpriteManager::SpriteManager( TextureManager::shared_ptr textureManager )
: m_textureManager( textureManager )
{
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::LoadFromFile( const unify::Path & filePath )
{
	bool defaultLoop = true;
	qxml::Document doc( filePath );

	// Table of texture mappings.
	typedef std::string Name;
	std::map< Name, Texture::shared_ptr > textureMapping;

	qxml::Element * leaf = doc.GetRoot()->GetFirstChild();
	while( leaf )
	{
		if( leaf->IsTagName( "texture" ) )
		{
			Texture::shared_ptr texture = m_textureManager.lock()->Find( leaf->GetAttribute( "source" )->GetString() );
			textureMapping[ leaf->GetAttribute( "name" )->GetString() ] = texture;
		}
		else if( leaf->IsTagName( "group" ) )
		{
			std::shared_ptr< animation::Group > group( new Group( this ) );
			qxml::Element * sequenceLeaf = leaf->GetFirstChild();
			while( sequenceLeaf )
			{
				if( sequenceLeaf->IsTagName( "sequence" ) )
				{
					bool loop = sequenceLeaf->GetBooleanAttributeElse( "loop", defaultLoop );
					std::shared_ptr< animation::Sequence > sequence( new Sequence( group.get(), loop ) );
					qxml::Element * frameLeaf = sequenceLeaf->GetFirstChild();
					while( frameLeaf )
					{
						if( frameLeaf->IsTagName( "frame" ) )
						{
							Texture::shared_ptr texture = textureMapping[ frameLeaf->GetAttribute( "texture" )->GetString() ];
							unify::TexArea area( unify::TexArea::Full() );
							if( frameLeaf->HasAttributes( "sprite" ) )
							{
								int spriteIndex = frameLeaf->GetAttribute( "sprite" )->GetInteger();
								std::string arrayName = frameLeaf->GetStringAttributeElse( "array", "" );
								if( ! arrayName.empty() )
								{
									area = texture->GetSprite( arrayName, spriteIndex );
								}
								else
								{
									area = texture->GetSprite( spriteIndex );
								}
							}
							unify::Seconds duration = static_cast< unify::Seconds >( frameLeaf->GetFloatAttributeElse( "duration", 0 ) );
							animation::Frame frame( texture, area, duration );
							sequence->Add( frame );
						}
						frameLeaf = frameLeaf->GetNext();
					}
					group->Add( sequenceLeaf->GetAttribute( "name" )->GetString(), sequence );
				}
				sequenceLeaf = sequenceLeaf->GetNext();
			}
			AddGroup( leaf->GetAttribute( "name" )->GetString(), group );
		}

		leaf = leaf->GetNext();
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

