// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/EffectLoader.h>
#include <dxi/loader/TextureLoader.h>
#include <dxi/loader/PixelShaderLoader.h>
#include <dxi/loader/VertexShaderLoader.h>
#include <qxml/Document.h>

using namespace dxi;

void loader::EffectLoader( Effect & effect, const qxml::Element * node, Managers managers )
{
	const qxml::Element * childNode = node->GetFirstChild();
	while( childNode )
	{
		//void SetName( const std::string & sName );
		//void SetFlags( unsigned int dwFlags );

        if ( childNode->IsTagName( "texture" ) )
        {
            unsigned char stage = static_cast< unsigned char >( childNode->GetIntegerAttributeElse( "stage", 0 ) );
			effect.SetTexture( stage, loader::TextureLoader( childNode, managers.GetTextureManager() ) );
        }
        else if ( childNode->IsTagName( "blend" ) )
        {
            effect.SetBlend( Blend( childNode->GetText() ) );
        }

		//void SetCulling( unsigned int dwValue );
		//void SetLighting( unsigned int dwValue );
        else if ( childNode->IsTagName( "pixelshader" ) )
        {
			effect.SetPixelShader( loader::PixelShaderLoader( childNode, managers.GetPixelShaderManager() ) );
        }
        else if ( childNode->IsTagName( "vertexshader" ) )
        {
			effect.SetVertexShader( loader::VertexShaderLoader( childNode, managers.GetVertexShaderManager() ) );
        }
		//void AddFrame( size_t frameIndex, float influence );

		childNode = childNode->GetNext();
	}
}

Effect::shared_ptr loader::EffectLoader( const std::string & name, const qxml::Element * node, Managers managers )
{
	// Return the effect if it already exists in the manager.
	if ( managers.GetEffectManager()->Exists( name ) )
	{
		return managers.GetEffectManager()->Find( name );
	}

	Effect * effect = new Effect();
	EffectLoader( *effect, node, managers );

	return managers.GetEffectManager()->Add( name, effect );
}

Effect::shared_ptr loader::EffectLoader( const std::string & name, const unify::Path & source, Managers managers )
{
	qxml::Document document( source );
	return loader::EffectLoader( name, document.GetRoot(), managers );
}
