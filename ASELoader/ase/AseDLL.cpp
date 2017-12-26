// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <ase/AseDLL.h>
#include <ase/GeometryFactory.h>
#include <memory.h>
#include <me/render/IRenderer.h>
#include <me/game/Game.h>

using namespace ase;
using namespace me;
using namespace render;

void Deleter( GeometryFactory * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::game::IGame * gameBase, const qxml::Element * element )
{
	using namespace me;

	auto gameInstance = dynamic_cast< game::Game * >( gameBase );

	const auto * texturedPS = element->FindFirstElement( "texturedps" );
	
	gameInstance->Debug()->Assert( texturedPS != nullptr );

	std::string texturedPSName = texturedPS->GetAttribute< std::string >( "name" );
	unify::Path texturedPSPath( texturedPS->GetAttribute< std::string >( "source" ) );
	IPixelShader::ptr ps = gameInstance->GetManager< IPixelShader >()->Add( texturedPSName, texturedPSPath );
	
	const auto texturedVS = element->FindFirstElement( "texturedvs" );
	std::string texturedVSName = texturedVS->GetAttribute< std::string >( "name" );
	unify::Path texturedVSPath( texturedVS->GetAttribute< std::string >( "source" ) );
	IVertexShader::ptr vs = gameInstance->GetManager< IVertexShader >()->Add( texturedVSName, texturedVSPath );

	// Setup ASE factories.
	GeometryFactory * factory = new GeometryFactory( gameInstance );
	factory->SetVertexShader( vs );
	factory->SetPixelShader( ps );

	gameInstance->GetManager< Geometry >()->AddFactory( "ase", GeometryFactoryPtr( factory ) );


	return true;
}

