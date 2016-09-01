// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "The Basics"
/// goals and requirements:
/// * Illustrate the core framework components.
/// * Use low level objects for limited unit-style testing.
/// </summary>

#include <dxi/core/Game.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
	// We intend to keep these and use them directly, in an attempt to illustrate their usages.
	// Typically, we would use a higher level object, such as a PrimitiveList, which encapsulates these.
	Effect::shared_ptr m_effect;
	VertexBuffer m_VB;
	IndexBuffer m_IB;

public:
	bool Setup( core::IOS & os );
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;

bool MyGame::Setup( core::IOS & os )
{
	// Note: We automatically load "setup.xml" after this function, which is likely a better place to do this.
	// Values in setup.xml will supercede these, thus, these might make for okay defaults.

	os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
	os.SetFullscreen( false );
	return Game::Setup( os );
}

void MyGame::Startup()
{
	Game::Startup();

	// Load effect...
	m_effect = GetManager< Effect >()->Add( "textured_2d", "media/EffectTextured2D.xml" );

	// The particular effect we are loading is generic, and has no texture, so let's add one.
	// Load a texture...
	Texture::shared_ptr texture = GetManager< Texture >()->Add( "image", new Texture( "media/Sig10_1024.jpg" ) );
	m_effect->SetTexture( 0, texture );

	// Get the vertex declaration...
	VertexDeclaration vd = m_effect->GetVertexShader()->GetVertexDeclaration();

	// Create a vertex buffer...
	m_VB.Create( 10, vd, BufferUsage::Default );

	unify::DataLock vertexLock;
	m_VB.Lock( vertexLock );

	float z = 3.0f;
	VertexElement positionE = {};
	if ( ! vd.GetElement( "position", positionE ) )
	{
		throw new std::exception( "Positiong element not found!" );
	}

	VertexElement texE = {};
	if ( ! vd.GetElement( "tex", texE ) )
	{
		{
			throw new std::exception( "Positiong element not found!" );
		}
	}

	// Shape 1 - no IB
	vd.WriteVertex( vertexLock, 0, positionE, unify::V3< float >( 10, 10, z ) );
	vd.WriteVertex( vertexLock, 0, texE, unify::TexCoords( 0, 0 ) );

	vd.WriteVertex( vertexLock, { 1, 3 }, positionE, unify::V3< float >( 300, 10, z ) );
	vd.WriteVertex( vertexLock, { 1, 3 },  texE, unify::TexCoords( 1, 0 ) );


	vd.WriteVertex( vertexLock, { 2, 5 }, positionE, unify::V3< float >( 10, 300, z ) );
	vd.WriteVertex( vertexLock, { 2, 5 }, texE, unify::TexCoords( 0, 1 ) );

	vd.WriteVertex( vertexLock, 4, positionE, unify::V3< float >( 300, 300, z ) );
	vd.WriteVertex( vertexLock, 4, texE, unify::TexCoords( 1, 1 ) );
	
	// Shape 2 - with IB
	vd.WriteVertex( vertexLock, 6, positionE, unify::V3< float >( 310, 10, z )  );
	vd.WriteVertex( vertexLock, 6, texE, unify::TexCoords( 0, 0 ) );
	vd.WriteVertex( vertexLock, 7, positionE, unify::V3< float >( 620, 10, z ) );
	vd.WriteVertex( vertexLock, 7, texE, unify::TexCoords( 1, 0 ) );
	vd.WriteVertex( vertexLock, 8, positionE, unify::V3< float >( 310, 300, z ) );
	vd.WriteVertex( vertexLock, 8, texE, unify::TexCoords( 0, 1 ) );
	vd.WriteVertex( vertexLock, 9, positionE, unify::V3< float >( 620, 300, z ) );
	vd.WriteVertex( vertexLock, 9, texE, unify::TexCoords( 1, 1 ) );

	m_VB.Unlock();

	m_IB.Create( 6, IndexFormat::Index16, BufferUsage::Default );
	IndexLock indexLock;
	m_IB.Lock( indexLock );
	indexLock.SetIndex( 0, 6 );
	indexLock.SetIndex( 1, 7 );
	indexLock.SetIndex( 2, 8 );
	indexLock.SetIndex( 3, 7 );
	indexLock.SetIndex( 4, 9 );
	indexLock.SetIndex( 5, 8 );
	m_IB.Unlock();
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	// -- Update code goes here. --

	// Return this.
	return Game::Update( elapsed, input );
}

void MyGame::Render()
{
	// -- Render code goes here. --
	RenderInfo renderInfo;

	unify::Matrix projectionMatrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, static_cast< float >( GetOS().GetResolution().width ), static_cast< float >( GetOS().GetResolution().height ), 0, 1, 1000 );
	renderInfo.SetFinalMatrix( projectionMatrix );

	m_VB.Use();
	m_effect->Use( renderInfo );

	RenderMethod methodNoIB = RenderMethod( PrimitiveType::TriangleList, 0, 3, 2, m_effect );
	methodNoIB.Render( renderInfo );

	RenderMethod methodWithIB = RenderMethod(  PrimitiveType::TriangleList, 0, 6, 4, 0, 2, m_effect );
	m_IB.Use();
	methodWithIB.Render( renderInfo );

	Game::Render();
}

void MyGame::Shutdown()
{
	// -- Shutdown code goes here. --

	m_VB.Release();

	// Do this last.
	Game::Shutdown();
}

