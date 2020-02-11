// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/sculpter/SculpterFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/render/VertexUtil.h>
#include <unify/String.h>
#include <unify/FrameSet.h>
#include <unify/ColorUnit.h>

using namespace me;
using namespace sculpter;
using namespace render;

SculpterFactory::SculpterFactory( me::game::Game * gameInstance)
: m_game( gameInstance )
{
}

void SculpterFactory::AddShapeCreator( std::string name, IShapeCreator::ptr creator )
{
	m_creators[ name ] = creator;
}

IShapeCreator* SculpterFactory::FindShapeCreator( std::string name ) const
{
	auto itr = m_creators.find( name );
	if ( itr == m_creators.end() )
	{
		return nullptr;
	}
	return itr->second.get();
}

Geometry::ptr SculpterFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	game::Game & gameInstance = *m_game;

	// Managers to store sub-resources.
	auto textureManager = gameInstance.GetManager< ITexture >();
	auto pixelShaderManager = gameInstance.GetManager< IPixelShader >();
	auto vertexShaderManager = gameInstance.GetManager< IVertexShader >();
	auto effectManager = gameInstance.GetManager< Effect >();

	Mesh * mesh = new Mesh( m_game->GetOS()->GetRenderer( 0 ) );
	PrimitiveList & primitiveList = mesh->GetPrimitiveList();

	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );
	return Geometry::ptr( mesh );
}

Geometry::ptr SculpterFactory::Produce( unify::Parameters parameters )
{
	game::Game & gameInstance = *m_game;

	// Managers to store sub-resources.
	auto textureManager = gameInstance.GetManager< ITexture >();
	auto pixelShaderManager = gameInstance.GetManager< IPixelShader >();
	auto vertexShaderManager = gameInstance.GetManager< IVertexShader >();
	auto effectManager = gameInstance.GetManager< Effect >();

	Mesh * mesh = new Mesh( m_game->GetOS()->GetRenderer( 0 ) );
	PrimitiveList & primitiveList = mesh->GetPrimitiveList();

	std::string type = parameters.Get< std::string >( "type" );
	auto creator = FindShapeCreator( type );

	if( creator == nullptr )
	{
		throw me::exception::FailedToCreate( "Could not find shape creator for type \"" + type + "\"." );
	}
	creator->Create( primitiveList, parameters );

	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );
	return Geometry::ptr( mesh );
}

