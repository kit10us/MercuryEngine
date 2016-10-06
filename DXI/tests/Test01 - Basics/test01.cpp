// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "The Basics"
/// goals and requirements:
/// * Illustrate the core framework components.
/// * Use low level objects for limited unit-style testing.
/// </summary>

#include <dxi/core/Game.h>
#include <dxi/win/DXILib.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
	// We intend to keep these and use them directly, in an attempt to illustrate their usages.
	// Typically, we would use a higher level object, such as a PrimitiveList, which encapsulates these.
	Effect::ptr m_effect;
	VertexBuffer m_VB;
	IndexBuffer m_IB;

public:
	void Startup() override;
	bool Update( RenderInfo & renderInfo, IInput & input ) override;
	void Render( const RenderInfo & renderInfo ) override;
	void Shutdown() override;
} game;

void MyGame::Startup()
{
	Game::Startup();

	// Load effect...
	m_effect = GetManager< Effect >()->Add( "textured_2d", "media/EffectTextured2D.xml" );

	// The particular effect we are loading is generic, and has no texture, so let's add one.
	// Load a texture...
	Texture::ptr texture = GetManager< Texture >()->Add( "image", new Texture( "media/Sig10_1024.jpg" ) );
	m_effect->SetTexture( 0, texture );

	// Get the vertex declaration, which describes the layout for the vertex...
	VertexDeclaration::ptr vd = m_effect->GetVertexShader()->GetVertexDeclaration();

	size_t numberOfVertices = 10;
	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * numberOfVertices] );

	unify::DataLock vertexLock( vertices.get(), vd->GetSize(), numberOfVertices, false );

	float z = 3.0f;
	VertexElement positionE = {};
	if( !vd->GetElement( "position", positionE ) )
	{
		throw new std::exception( "Positiong element not found!" );
	}

	VertexElement texE = {};
	if( !vd->GetElement( "tex", texE ) )
	{
		{
			throw new std::exception( "Positiong element not found!" );
		}
	}

	// Shape 1 - no IB
	vd->WriteVertex( vertexLock, 0, positionE, unify::V3< float >( 10, 10, z ) );
	vd->WriteVertex( vertexLock, 0, texE, unify::TexCoords( 0, 0 ) );

	vd->WriteVertex( vertexLock, { 1, 3 }, positionE, unify::V3< float >( 300, 10, z ) );
	vd->WriteVertex( vertexLock, { 1, 3 }, texE, unify::TexCoords( 1, 0 ) );

	vd->WriteVertex( vertexLock, { 2, 5 }, positionE, unify::V3< float >( 10, 300, z ) );
	vd->WriteVertex( vertexLock, { 2, 5 }, texE, unify::TexCoords( 0, 1 ) );

	vd->WriteVertex( vertexLock, 4, positionE, unify::V3< float >( 300, 300, z ) );
	vd->WriteVertex( vertexLock, 4, texE, unify::TexCoords( 1, 1 ) );

	// Shape 2 - with IB
	vd->WriteVertex( vertexLock, 6, positionE, unify::V3< float >( 310, 10, z ) );
	vd->WriteVertex( vertexLock, 6, texE, unify::TexCoords( 0, 0 ) );
	vd->WriteVertex( vertexLock, 7, positionE, unify::V3< float >( 620, 10, z ) );
	vd->WriteVertex( vertexLock, 7, texE, unify::TexCoords( 1, 0 ) );
	vd->WriteVertex( vertexLock, 8, positionE, unify::V3< float >( 310, 300, z ) );
	vd->WriteVertex( vertexLock, 8, texE, unify::TexCoords( 0, 1 ) );
	vd->WriteVertex( vertexLock, 9, positionE, unify::V3< float >( 620, 300, z ) );
	vd->WriteVertex( vertexLock, 9, texE, unify::TexCoords( 1, 1 ) );

	// Create a vertex buffer...
	m_VB.Create( numberOfVertices, vd, vertices.get() );
	vertices.reset();

	size_t numberOfIndices = 6;
	std::vector< Index32 > indices( numberOfIndices );

	indices[0] = 6;
	indices[1] = 7;
	indices[2] = 8;
	indices[3] = 7;
	indices[4] = 9;
	indices[5] = 8;

	m_IB.Create( numberOfIndices, &indices[0] );
}

bool MyGame::Update( RenderInfo & renderInfo, IInput & input )
{
	// -- Update code goes here. --

	// Return this.
	return Game::Update( renderInfo, input );
}

void MyGame::Render( const RenderInfo & renderInfo )
{
	// -- Render code goes here. --
	RenderInfo renderInfo2D( renderInfo );

	unify::Matrix projectionMatrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, static_cast< float >(GetOS().GetResolution().width), static_cast< float >(GetOS().GetResolution().height), 0, 1, 1000 );
	renderInfo2D.SetProjectionMatrix( projectionMatrix );

	m_VB.Use();
	m_effect->Use( renderInfo2D );

	RenderMethod methodNoIB = RenderMethod::CreateTriangleList( 0, 2, m_effect );
	methodNoIB.Render( renderInfo2D );

	size_t numberOfVertices = 10;
	RenderMethod methodWithIB = RenderMethod::CreateTriangleListIndexed( numberOfVertices, 6, 0, 0, m_effect );
	m_IB.Use();
	methodWithIB.Render( renderInfo2D );

	Game::Render( renderInfo );
}

void MyGame::Shutdown()
{
	// -- Shutdown code goes here. --

	m_VB.Destroy();

	// Do this last.
	Game::Shutdown();
}

