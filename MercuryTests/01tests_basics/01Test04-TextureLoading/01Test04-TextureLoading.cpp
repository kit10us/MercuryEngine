// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "The Basics"
/// goals and requirements:
/// * Illustrate the core framework components.
/// * Use low level objects for limited unit-style testing.
/// </summary>

#include <me/Game.h>
#include <me/RenderMethod.h>
#include <MEWinMain.h>

using namespace unify;
using namespace me;

class MyGame : public Game
{
	Effect::ptr effectBorg;
	Effect::ptr effect4;
	IVertexBuffer::ptr vertexBuffer;
	Quaternion q;

public:
	void Startup() override;
	void Update( IRenderer * renderer, RenderInfo & renderInfo ) override;
	void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;
	void Shutdown() override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	effectBorg = GetManager< Effect>()->Add( "borg", "media/EffectTextured.effect" );
	effect4 = GetManager< Effect>()->Add( "4", "media/Effect4.effect" );

#pragma region Create vertex raw data as a C-style struct...
	float xscalar = 10.0f;
	float yscalar = 10.0f;
	float zscalar = 10.0f;

	struct Vertex
	{
		float x, y, z;
		float u, v;
	};

	Vertex vbRaw[] =
	{
		// Z-Near...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },

		// Z-Near...
		{ xscalar, yscalar, -zscalar, 0.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, -zscalar, 1.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },

		// X-Min...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, -zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },

		// X-Max...
		{ xscalar, yscalar, -zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 0.0f, 1.0f },

		// Y-Min...
		{ -xscalar, yscalar, zscalar, 0.0f, 0.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ -xscalar, yscalar, -zscalar, 0.0f, 1.0f },
		{ xscalar, yscalar, zscalar, 1.0f, 0.0f },
		{ xscalar, yscalar, -zscalar, 1.0f, 1.0f },
		{ -xscalar, yscalar, -zscalar, 0.0f, 1.0f },

		// Y-Max...
		{ -xscalar, -yscalar, -zscalar, 0.0f, 0.0f },
		{ xscalar, -yscalar, -zscalar, 1.0f, 0.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f },
		{ xscalar, -yscalar, -zscalar, 1.0f, 0.0f },
		{ xscalar, -yscalar, zscalar, 1.0f, 1.0f },
		{ -xscalar, -yscalar, zscalar, 0.0f, 1.0f }
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );

	vertexBuffer = GetOS()->GetRenderer(0)->ProduceVB( { numberOfVertices, effectBorg->GetVertexShader()->GetVertexDeclaration(), 0, vbRaw, BufferUsage::Default } );
}

void MyGame::Update( IRenderer * renderer, RenderInfo & renderInfo )
{
	static Angle rotation( AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	HRESULT result = S_OK;

	const float width = GetOS()->GetRenderer(0)->GetViewport().GetSize().width;
	const float height = GetOS()->GetRenderer( 0 )->GetViewport().GetSize().height;

	rotation += AngleInDegrees( renderInfo.GetDelta() * 90.0f );
	if( rotation.Fix360() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}

	V3< float > eye( 0.0f, 1.0f, -50.0f );
	V3< float > at( 0.0f, 0.0f, 0.0f );
	V3< float > up( 0.0f, 1.0f, 0.0f );

	V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	
	q = Quaternion( axis, rotation );

	renderInfo.SetViewMatrix( MatrixLookAtLH( eye, at, up ) );
	renderInfo.SetProjectionMatrix( MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MyGame::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	vertexBuffer->Use();

	{
		RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effectBorg ) );

		unify::Matrix instance { Matrix( q ) * MatrixTranslate( V3< float >( -15, 15, 10 ) ) };
		renderer->Render( method, renderInfo, &instance, 1 );
	}
	{
		RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect4 ) );

		unify::Matrix instance { Matrix( q ) * MatrixTranslate( V3< float >( 15, 15, 10 ) ) };
		renderer->Render( method, renderInfo, &instance, 1 );
	}
}

void MyGame::Shutdown()
{
	effect4.reset();
	effectBorg.reset();
	vertexBuffer.reset();
}

