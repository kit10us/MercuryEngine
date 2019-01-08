// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <Windows.h>
#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace unify;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	effectBorg = GetManager< Effect>()->Add( "borg", unify::Path( "EffectTexture.effect" ) );
	effect4 = GetManager< Effect>()->Add( "4", unify::Path( "Effect4.effect" ) );

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

	vertexBuffer = GetOS()->GetRenderer( 0 )->ProduceVB( { effectBorg->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	static Angle rotation( AngleInRadians( 0.0f ) );
	static int axisIndex = 0;

	HRESULT result = S_OK;

	const float width = GetOS()->GetRenderer(0)->GetViewport().GetSize().width;
	const float height = GetOS()->GetRenderer( 0 )->GetViewport().GetSize().height;

	rotation += AngleInDegrees( params.GetDelta().GetSeconds() * 90.0f );
	if( rotation.Normalize() != 0 )
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

	params.renderInfo.SetViewMatrix( MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MainScene::OnRender( RenderGirl renderGirl )
{
	vertexBuffer->Use();

	render::Params params = *renderGirl.GetParams();

	{
		RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effectBorg ) );

		unify::Matrix matrices[] =
		{
			Matrix( q ) * MatrixTranslate( V3< float >( -15, 15, 10 ) ),
			Matrix( q ) * MatrixTranslate( V3< float >( 15, 15, 10 ) )
		};

		render::MatrixFood_Matrices food( matrices, 2 );

		render::MatrixFeed feed( food, 1 );

		params.renderer->Render( method, params.renderInfo, feed );
	}

	{
		RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect4 ) );

		unify::Matrix matrices[] =
		{
			Matrix( q ) * MatrixTranslate( V3< float >( -15, -15, 10 ) ),
			Matrix( q ) * MatrixTranslate( V3< float >( 15, -15, 10 ) )
		};

		render::MatrixFood_Matrices food( matrices, 2 );

		render::MatrixFeed feed( food, 1 );

		params.renderer->Render( method, params.renderInfo, feed );
	}



	/*
	unify::Matrix instance1{ Matrix( q ) * MatrixTranslate( V3< float >( -15, 15, 10 ) ) };
	unify::Matrix instance2{ Matrix( q ) * MatrixTranslate( V3< float >( 15, 15, 10 ) ) };

	render::MatrixFeed feed[] = {
	{ MatrixFood_Matrices{ &instance1, 1 }, 1 },
	{ MatrixFood_Matrices{ &instance2, 1 }, 1 }
	};

	{
		RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effectBorg ) );
		render::MatrixFeed food( MatrixFood_Matrices{ &instance1, 1 }, 1 );
		params.renderer->Render( method, params.renderInfo, feed[ 0 ] );
	}
	{
		RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect4 ) );
		render::MatrixFeed food( MatrixFood_Matrices{ &instance2, 1 }, 1 );
		params.renderer->Render( method, params.renderInfo, feed[ 1 ] );	
	}
	*/
}
