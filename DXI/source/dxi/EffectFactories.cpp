// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/EffectFactories.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/core/Game.h>
#include <qxml/Document.h>

using namespace dxi;

Effect * dxi::ProduceEffect( const qxml::Element * childNode )
{
	auto textureManager = dxi::core::Game::GetGameInstance()->GetManager< Texture >();
	auto pixelShaderManager = dxi::core::Game::GetGameInstance()->GetManager< PixelShader >();
	auto vertexShaderManager = dxi::core::Game::GetGameInstance()->GetManager< VertexShader >();

	Effect * effect = new Effect;

	while( childNode )
	{
		//void SetName( const std::string & sName );
		//void SetFlags( unsigned int dwFlags );

		if( childNode->IsTagName( "texture" ) )
		{
			std::string name = childNode->GetStringAttribute( "name" );
			unsigned char stage = static_cast< unsigned char >(childNode->GetIntegerAttributeElse( "stage", 0 ));
			unify::Path source = childNode->GetDocument()->GetPath().DirectoryOnly() + childNode->GetStringAttribute( "source" );
			effect->SetTexture( stage, textureManager->Add( name, source ) );
		}
		else if( childNode->IsTagName( "blend" ) )
		{
			std::string blend = childNode->GetText();
			effect->SetBlend( Blend( blend ) );
		}

		//void SetCulling( unsigned int dwValue );
		//void SetLighting( unsigned int dwValue );
		else if( childNode->IsTagName( "pixelshader" ) )
		{
			std::string name = childNode->GetStringAttribute( "name" );
			unify::Path source = childNode->GetDocument()->GetPath().DirectoryOnly() + childNode->GetStringAttribute( "source" );
			std::string entry = childNode->GetStringAttribute( "entry" );
			std::string profile = childNode->GetStringAttribute( "profile" );
			qjson::Object json = { { "name", name } , { "source", source.ToString() } , { "entry", entry }, { "profile", profile } };
			effect->SetPixelShader( pixelShaderManager->Add( json ) );
		}
		else if( childNode->IsTagName( "vertexshader" ) )
		{
			std::string name = childNode->GetStringAttribute( "name" );
			unify::Path source = childNode->GetDocument()->GetPath().DirectoryOnly() + childNode->GetStringAttribute( "source" );
			std::string entry = childNode->GetStringAttribute( "entry" );
			std::string profile = childNode->GetStringAttribute( "profile" );
			qjson::Object vd( std::string( childNode->FindFirstElement( "vd" )->GetText() ) );
			qjson::Object json = { { "name", name } ,{ "source", source.ToString() } ,{ "entry", entry },{ "profile", profile }, { "vd", vd } };
			effect->SetVertexShader( vertexShaderManager->Add( json ) );
		}
		//void AddFrame( size_t frameIndex, float influence );

		childNode = childNode->GetNext();
	}

	return effect;
}

Effect * EffectSourceFactory::Produce( unify::Path path )
{
	qxml::Document doc( path );
	const qxml::Element * childNode = doc.GetRoot()->GetFirstChild();
	return ProduceEffect( childNode );
}