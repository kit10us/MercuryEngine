// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	effect = GetManager< Effect>()->Add( "texture3d", unify::Path( "EffectColor.effect" ) );

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
		{ -xscalar, yscalar, zscalar, unify::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },

		// Z-Near...
		{ xscalar, yscalar, -zscalar, unify::ColorRed() },
		{ -xscalar, yscalar, -zscalar, unify::ColorGreen() },
		{ xscalar, -yscalar, -zscalar, unify::ColorBlue() },
		{ -xscalar, yscalar, -zscalar, unify::ColorGreen() },
		{ -xscalar, -yscalar, -zscalar, unify::ColorWhite() },
		{ xscalar, -yscalar, -zscalar, unify::ColorBlue() },

		// X-Min...
		{ -xscalar, yscalar, zscalar, unify::ColorRed() },
		{ -xscalar, yscalar, -zscalar, unify::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },
		{ -xscalar, yscalar, -zscalar, unify::ColorGreen() },
		{ -xscalar, -yscalar, -zscalar, unify::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },

		// X-Max...
		{ xscalar, yscalar, -zscalar, unify::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ xscalar, -yscalar, -zscalar, unify::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::ColorWhite() },
		{ xscalar, -yscalar, -zscalar, unify::ColorBlue() },

		// Y-Min...
		{ -xscalar, yscalar, zscalar, unify::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ -xscalar, yscalar, -zscalar, unify::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ xscalar, yscalar, -zscalar, unify::ColorWhite() },
		{ -xscalar, yscalar, -zscalar, unify::ColorBlue() },

		// Y-Max...
		{ -xscalar, -yscalar, -zscalar, unify::ColorRed() },
		{ xscalar, -yscalar, -zscalar, unify::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },
		{ xscalar, -yscalar, -zscalar, unify::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() }
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );

	vertexBuffer = GetOS()->GetRenderer( 0 )->ProduceVB( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	const float width = params.renderer->GetViewport().GetSize().width;
	const float height = params.renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( params.GetDelta().GetSeconds() * 360.0f );
	if( rotation.Normalize() != 0 )
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

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );

	unify::Matrix worldMatrixA = unify::MatrixIdentity();
	worldMatrixA *= unify::MatrixRotationAboutAxis( axis, rotation );

	q = unify::Quaternion( axis, rotation );

	params.renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MainScene::OnRender( RenderGirl renderGirl )
{
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect ) );

	unify::Matrix instance = unify::Matrix( q );
	renderGirl.GetParams()->renderer->Render( method, renderGirl.GetParams()->renderInfo, render::MatrixFeed( render::MatrixFood_Matrices{ &instance, 1 }, 1 ) );
}
