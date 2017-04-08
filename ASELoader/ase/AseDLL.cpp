// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <ase/AseDLL.h>
#include <ase/GeometryFactory.h>
#include <memory.h>
#include <me/render/IRenderer.h>
#include <me/Game.h>

using namespace ase;
using namespace me;
using namespace render;

void Deleter( GeometryFactory * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::IGame * _game, const qxml::Element * element )
{
	using namespace me;

	auto game = dynamic_cast<Game *>(_game);

	const auto texturePS = element->FindFirstElement( "textureps" );
	std::string texturePSName = texturePS->GetAttribute< std::string >( "name" );
	unify::Path texturePSPath( texturePS->GetAttribute< std::string >( "source" ) );
	IPixelShader::ptr ps = game->GetManager< IPixelShader >()->Add( texturePSName, texturePSPath );
	
	const auto textureVS = element->FindFirstElement( "texturevs" );
	std::string textureVSName = textureVS->GetAttribute< std::string >( "name" );
	unify::Path textureVSPath( textureVS->GetAttribute< std::string >( "source" ) );
	IVertexShader::ptr vs = game->GetManager< IVertexShader >()->Add( textureVSName, textureVSPath );

	// Setup ASE factories.
	GeometryFactory * factory = new GeometryFactory( game );
	factory->SetVertexShader( vs );
	factory->SetPixelShader( ps );

	game->GetManager< Geometry >()->AddFactory( "ase", GeometryFactoryPtr( factory ) );


	return true;
}

