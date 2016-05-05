// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/LoaderUtils.h>

using namespace dxi;
using namespace loader;

bool TextureModeConvertor::operator()( const qxml::Element * element, unify::Parameters & parameters )
{
	TextureMode mode = TM_TEXTURECORRECT;
	if ( unify::StringIs( unify::CleanWhitespace( element->GetText() ), "TextureWrapped" ) )
	{
		mode = TM_TEXTUREWRAPPED;
	}
	parameters.Set< TextureMode >( element->GetTagName(), mode );
	return true;
}

TextureConvertor::TextureConvertor( std::weak_ptr< TextureManager > textureManager )
: m_textureManager( textureManager )
{
}

bool TextureConvertor::operator()( const qxml::Element * element, unify::Parameters & parameters )
{
	if ( m_textureManager.expired() )
	{
        return false;
    }

	TextureManager::shared_ptr textureManager = m_textureManager.lock();
    
    std::string name = element->GetStringAttribute( "name" );
	unify::Path source( element->GetDocument()->GetPath() );
	source.ChangeFilename( element->GetStringAttributeElse("source", std::string() ) );
    unsigned int flags = 0;
    if ( element->HasAttributes( "lockable" ) )
    {
        flags |= TEXTURE_LOCKABLE;
    }
    if ( element->HasAttributes( "norender" ) )
    {
        flags |= TEXTURE_NORENDER;
    }
	Texture::shared_ptr texture( textureManager->LoadFromFile( name, source, flags ) );
    parameters.Set< Texture::shared_ptr >( element->GetTagName(), texture );
    return true;
}

bool BoolConvertor::operator()( const qxml::Element * element, unify::Parameters & parameters )
{
	bool value = unify::Cast< bool >( element->GetText() );
	parameters.Set< bool >( element->GetTagName(), value );
	return true;
}
