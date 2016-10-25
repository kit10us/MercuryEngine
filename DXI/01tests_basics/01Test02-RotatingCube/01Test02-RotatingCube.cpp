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
#include <dxi/RenderMethod.h>
#include <DXIWinMain.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
	Effect::ptr effect;
	VertexBuffer::ptr vertexBuffer;

public:
	void Startup() override;
	bool Update( RenderInfo & renderInfo ) override;
	void Render( int renderer, const RenderInfo & renderInfo, const Viewport & viewport ) override;
	void Shutdown() override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	effect = GetManager< Effect>()->Add( "textured_3d", "media/EffectColor.effect" );

#pragma region Create vertex raw data as a C-style struct...
	float xscalar = 10.0f;
	float yscalar = 10.0f;
	float zscalar = 10.0f;

	struct Vertex
	{
		float x, y, z;
		unify::Color diffuse;
	};

	Vertex vbRaw[] =
	{
		// Z-Near...
		{ -xscalar, yscalar, zscalar, unify::Color::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::Color::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::Color::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::Color::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::Color::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::Color::ColorBlue() },

		// Z-Near...
		{ xscalar, yscalar, -zscalar, unify::Color::ColorRed() },
		{ -xscalar, yscalar, -zscalar, unify::Color::ColorGreen() },
		{ xscalar, -yscalar, -zscalar, unify::Color::ColorBlue() },
		{ -xscalar, yscalar, -zscalar, unify::Color::ColorGreen() },
		{ -xscalar, -yscalar, -zscalar, unify::Color::ColorWhite() },
		{ xscalar, -yscalar, -zscalar, unify::Color::ColorBlue() },

		// X-Min...
		{ -xscalar, yscalar, zscalar, unify::Color::ColorRed() },
		{ -xscalar, yscalar, -zscalar, unify::Color::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::Color::ColorBlue() },
		{ -xscalar, yscalar, -zscalar, unify::Color::ColorGreen() },
		{ -xscalar, -yscalar, -zscalar, unify::Color::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::Color::ColorBlue() },

		// X-Max...
		{ xscalar, yscalar, -zscalar, unify::Color::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::Color::ColorGreen() },
		{ xscalar, -yscalar, -zscalar, unify::Color::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::Color::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::Color::ColorWhite() },
		{ xscalar, -yscalar, -zscalar, unify::Color::ColorBlue() },

		// Y-Min...
		{ -xscalar, yscalar, zscalar, unify::Color::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::Color::ColorGreen() },
		{ -xscalar, yscalar, -zscalar, unify::Color::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::Color::ColorGreen() },
		{ xscalar, yscalar, -zscalar, unify::Color::ColorWhite() },
		{ -xscalar, yscalar, -zscalar, unify::Color::ColorBlue() },

		// Y-Max...
		{ -xscalar, -yscalar, -zscalar, unify::Color::ColorRed() },
		{ xscalar, -yscalar, -zscalar, unify::Color::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::Color::ColorBlue() },
		{ xscalar, -yscalar, -zscalar, unify::Color::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::Color::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::Color::ColorBlue() }
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );

	vertexBuffer.reset( new VertexBuffer( GetOS()->GetRenderer( 0 ), numberOfVertices, effect->GetVertexShader()->GetVertexDeclaration(), vbRaw, BufferUsage::Default ) );
}

bool MyGame::Update( RenderInfo & renderInfo )
{
	static unify::Angle rotation( unify::Angle::AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	const float width = GetOS()->GetRenderer( 0 )->GetViewport().GetSize().width;
	const float height = GetOS()->GetRenderer( 0 )->GetViewport().GetSize().height;

	rotation += unify::Angle::AngleInDegrees( renderInfo.GetDelta() * 360.0f );
	if( rotation.Fix360() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}

	unify::V3< float > eye( 0.0f, 1.0f, -50.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	unify::Matrix worldMatrix = unify::Matrix::MatrixIdentity();
	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	worldMatrix *= unify::Matrix::MatrixRotationAboutAxis( axis, rotation );
	renderInfo.SetWorldMatrix( worldMatrix );
	renderInfo.SetViewMatrix( unify::Matrix::MatrixLookAtLH( eye, at, up ) );
	renderInfo.SetProjectionMatrix( unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
	return true;
}

void MyGame::Render( int renderer, const RenderInfo & renderInfo, const Viewport & viewport )
{
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect ) );
	method.Render( renderInfo );
}

void MyGame::Shutdown()
{
	effect.reset();
	vertexBuffer.reset();
}

