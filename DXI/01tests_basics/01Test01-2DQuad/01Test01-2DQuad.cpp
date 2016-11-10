// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "The Basics"
/// goals and requirements:
/// * Illustrate the core framework components.
/// * Use low level objects for limited unit-style testing.
/// </summary>

#include <me/Game.h>
#include <dxi/win/DXILib.h>
#include <me/RenderMethod.h>
#include <DXIWinMain.h>

using namespace me;

class MyGame : public Game
{
	Effect::ptr effect;
	IVertexBuffer::ptr vertexBuffer;

public:
	void Startup() override;
	void Update( me::RenderInfo & renderInfo ) override;
	void Render( int renderer, const me::RenderInfo & renderInfo, const me::Viewport & viewport ) override;
	void Shutdown() override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	const float width = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().width;
	const float height = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().height;

	effect = GetManager< Effect>()->Add( "color2d", "media/EffectColor2D.effect" );

#pragma region Create vertex raw data as a C-style struct...
	float depth = 0.5f;
	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
	};

	Vertex vbRaw[] =
	{
		{ 0, 0, depth, 1.0f, 0.0f, 0.0f, 1.0f },
		{ width, 0, depth, 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0, height, depth, 0.0f, 0.0f, 1.0f, 1.0f },
		{ width, height, depth, 1.0f, 1.0f, 1.0f, 1.0f }
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );

	vertexBuffer = GetOS()->GetRenderer(0)->ProduceVB( { numberOfVertices, effect->GetVertexShader()->GetVertexDeclaration(), 0, vbRaw, BufferUsage::Default } );
}

void MyGame::Update( RenderInfo & renderInfo )
{
	const float width = (float)renderInfo.GetRenderer()->GetDisplay().GetSize().width;
	const float height = (float)renderInfo.GetRenderer()->GetDisplay().GetSize().height;
	renderInfo.SetProjectionMatrix( unify::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.01f, 100.0f ) );
}

void MyGame::Render( int renderer, const RenderInfo & renderInfo, const me::Viewport & viewport )
{
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleStrip( 0, 2, effect ) );
	method.Render( renderInfo );
}

void MyGame::Shutdown()
{
	effect.reset();
	vertexBuffer.reset();
}

