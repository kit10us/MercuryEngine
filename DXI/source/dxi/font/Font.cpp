// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/Font.h>
#include <dxi/factory/EffectFactories.h>
#include <dxi/factory/GeometryFactory.h>
#include <dxi/core/Game.h>
#include <qxml/Document.h>

using namespace dxi;
using namespace font;

Font::Font()
{
}

Font::Font( const unify::Path & filePath, animation::SpriteManager::shared_ptr spriteManager )
{
	CreateFromFile( filePath, spriteManager );
}

Font::~Font() throw ()
{
}

void Font::CreateFromFile( const unify::Path & filePath, animation::SpriteManager::shared_ptr spriteManager )
{
	// Check if the file exists...
	qxml::Document doc( filePath );

	auto textureManager = core::Game::GetInstance()->GetManager< Texture >();
	auto effectManager = core::Game::GetInstance()->GetManager< Effect >();
	auto geometryManager = core::Game::GetInstance()->GetManager< Geometry >();

	for( const auto & leaf : doc.GetRoot()->FindFirstElement( "font" )->Children() )
	{
        if ( leaf.IsTagName( "effect" ) )
        {
			m_effect = effectManager->Add( leaf );
			assert( 0 ); // TODO:
			//std::shared_ptr< VertexBuffer > scratchVB( new VertexBuffer( 6, m_effect->GetVertexShader()->GetVertexDeclaration() ) );
			//m_effect->SetScratchVertexBuffer( scratchVB );
        }
		else if( leaf.IsTagName( "character" ) )
		{
			// General attributes...
			CharacterKey key( leaf.GetAttributeElse< std::string >( "key", "" ) );
			if( key.empty() )
			{
				throw unify::Exception( "Font key is empty!" );
			}

			Character character;

			// Textured...
			if( leaf.HasAttributes( "texture" ) )
			{
				// Animation...
				if( leaf.HasAttributes( "group,sequence" ) )
				{
					std::string textureName( leaf.GetAttribute( "texture" )->GetString() );
					if( ! textureManager->Exists( textureName ) )
					{
						throw unify::Exception( "Specfied texture for character font does not exist!" );
					}

					auto texture = { textureManager->Find( textureName ) };

					std::string groupName( leaf.GetAttribute( "group" )->GetString() );
					if( ! spriteManager->GroupExists( groupName ) )
					{
						throw unify::Exception( "Specfied animation group for character font does not exist!" );
					}

					std::string sequenceName( leaf.GetAttribute( "sequence" )->GetString() );
					if( ! spriteManager->FindGroup( groupName )->SequenceExists( sequenceName ) )
					{
						throw unify::Exception( "Specfied animation sequece for character font does not exist!" );
					}

					animation::Instance animationInstance( spriteManager->FindGroup( groupName )->FindSequence( sequenceName ) );
					character = Character( animationInstance, m_effect );
				}
				// Single frame...
				else if( leaf.HasAttributes( "sprite" ) )
				{
					std::string textureName( leaf.GetAttribute( "texture" )->GetString() );
					if( ! textureManager->Exists( textureName ) )
					{
						throw unify::Exception( "Specfied texture for character font does not exist!" );
					}

					Texture::ptr texture( textureManager->Find( textureName ) );
					unsigned int sprite = leaf.GetAttributeElse< unsigned int >( "sprite", 0 );

					animation::Frame frame( texture, texture->GetSprite( sprite ) );
					character = Character( frame, m_effect );
				}
				else
				{
					throw unify::Exception( "Not enough parameters for a textured based font character!" );
				}

			}
			// Geometry...
			else if( leaf.HasAttributes( "geometry" ) )
			{
				if( ! leaf.HasAttributes( "width" ) )
				{
					throw unify::Exception( "Geometry font missing 'width' attribute!" );
				}
				if( ! leaf.HasAttributes( "height" ) )
				{
					throw unify::Exception( "Geometry font missing 'height' attribute!" );
				}

				unify::Size< float > size;
				size.width = leaf.GetAttribute( "width" )->Get< float >();
				size.height = leaf.GetAttribute( "height" )->Get< float >();

				float geometryPostScale2D = leaf.GetAttributeElse< float >( "scale2d", 1 );
				unify::V3< float > geometryPostOffset2D;
				geometryPostOffset2D.x = leaf.GetAttributeElse< float >( "offset2dx", 0 );
				geometryPostOffset2D.y = leaf.GetAttributeElse< float >( "offset2dy", 0 );
				geometryPostOffset2D.z = leaf.GetAttributeElse< float >( "offset2dz", 0 );

				float geometryPostScale3D = leaf.GetAttributeElse< float >( "scale3d", 1 );
				unify::V3< float > geometryPostOffset3D;
				geometryPostOffset3D.x = leaf.GetAttributeElse< float >( "offset3dx", 0 );
				geometryPostOffset3D.y = leaf.GetAttributeElse< float >( "offset3dy", 0 );
				geometryPostOffset3D.z = leaf.GetAttributeElse< float >( "offset3dz", 0 );

				unify::Path geometryPath( filePath); 
				geometryPath.ChangeFilename( leaf.GetAttribute("geometry")->GetString() );
				std::string geometryName = geometryPath.FilenameNoExtension();
				Geometry::shared_ptr geometry( geometryManager->Add( geometryName, geometryPath ) );
				character = Character( geometry, size, geometryPostScale2D, geometryPostOffset2D, geometryPostScale3D, geometryPostOffset3D );
			}
			else
			{
				throw unify::Exception( "Font character is not well formed!" );
			}
			if( key == CharacterKey( "invalid" ) )
			{
				m_invalidCharacter = character;
			}
			else
			{
				m_characterMap[ key ] = character;
			}		
		}
	}

	m_filePath = filePath;
}

Character Font::GetCharacter( const CharacterKey & key ) const
{
	CharacterMap::const_iterator itr= m_characterMap.find( key );
	if( itr == m_characterMap.end() )
	{
		if( ! m_invalidCharacter.empty() )
		{
			return m_invalidCharacter;
		}
		throw unify::Exception( "Character not found by key!" );
	}
	return itr->second;
}

unsigned int Font::CharacterCount() const
{
	return m_characterMap.size();
}