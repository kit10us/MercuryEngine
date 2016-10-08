// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <ase/AseDLL.h>
#include <ase/GeometryFactory.h>
#include <memory.h>
#include <dxi/win/DXILib.h>
#include <dxi/win/DXDevice.h>
#include <dxi/win/DXRenderer.h>

using namespace ase;

void Deleter( GeometryFactory * factory )
{
	delete factory;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * doc )
{
	using namespace dxi;
	using namespace core;

	dxi::win::DXRenderer * dxRenderer = (dxi::win::DXRenderer *)(void*)game->GetOS().GetRenderer( 0 );
	dxi::win::DX::SetDxDevice( dxRenderer->GetDxDevice() );

	PixelShader::ptr ps = game->GetManager< PixelShader >()->Add( "texture3d", "media/shaders/texture3d.xml" );
	VertexShader::ptr vs = game->GetManager< VertexShader >()->Add( "texture3d", "media/shaders/texture3d.xml" );

	// Setup ASE factories.
	GeometryFactory * factory = new GeometryFactory( game );
	factory->SetVertexShader( vs );
	factory->SetPixelShader( ps );

	game->GetManager< dxi::Geometry >()->AddFactory( "ase", GeometryFactoryPtr( factory ) );


	return true;
}

