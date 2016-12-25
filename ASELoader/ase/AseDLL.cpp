// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <ase/AseDLL.h>
#include <ase/GeometryFactory.h>
#include <memory.h>
#include <me/IRenderer.h>
#include <me/Game.h>

using namespace ase;
using namespace me;

void Deleter( GeometryFactory * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc );

__declspec(dllexport) bool DXILoader( me::IGame * _game, const qxml::Document * doc )
{
	using namespace me;

	auto game = dynamic_cast<Game *>(_game);

	if( doc == nullptr ) 
	{
		game->ReportError( me::ErrorLevel::Failure, "ASELoader", "Configuraiton file missing!" );
	}

	const auto texturePS = doc->GetRoot()->FindFirstElement( "textureps" );
	std::string texturePSName = texturePS->GetAttribute< std::string >( "name" );
	unify::Path texturePSPath = texturePS->GetAttribute< std::string >( "source" );
	IPixelShader::ptr ps = game->GetManager< IPixelShader >()->Add( texturePSName, texturePSPath );
	
	const auto textureVS = doc->GetRoot()->FindFirstElement( "texturevs" );
	std::string textureVSName = textureVS->GetAttribute< std::string >( "name" );
	unify::Path textureVSPath = textureVS->GetAttribute< std::string >( "source" );
	IVertexShader::ptr vs = game->GetManager< IVertexShader >()->Add( textureVSName, textureVSPath );

	// Setup ASE factories.
	GeometryFactory * factory = new GeometryFactory( game );
	factory->SetVertexShader( vs );
	factory->SetPixelShader( ps );

	game->GetManager< me::Geometry >()->AddFactory( "ase", GeometryFactoryPtr( factory ) );


	return true;
}

