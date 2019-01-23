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
	m_borg.reset( new BufferSet( GetOS()->GetRenderer( 0 ), GetManager< Effect>()->Add( "borg", unify::Path( "EffectTexture.effect" ) ) ) );
	m_effect4.reset( new BufferSet( GetOS()->GetRenderer( 0 ), GetManager< Effect>()->Add( "4", unify::Path( "Effect4.effect" ) ) ) );

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

	m_borg->AddVertexBuffer( { m_borg->GetEffect()->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	m_borg->AddMethod( RenderMethod::CreateTriangleList( 0, 12 ) );

	m_effect4->AddVertexBuffer( { m_effect4->GetEffect()->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	m_effect4->AddMethod( RenderMethod::CreateTriangleList( 0, 12 ) );
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
	const render::Params params = *renderGirl.GetParams();
	{
		unify::Matrix instance { Matrix( q ) * MatrixTranslate( V3< float >( -15, 15, 10 ) ) };
		m_borg->Render( params, render::MatrixFeed( MatrixFood_Matrices{ &instance, 1 }, 1 ) );
	}
	{
		unify::Matrix instance { Matrix( q ) * MatrixTranslate( V3< float >( 15, 15, 10 ) ) };
		m_effect4->Render( params, render::MatrixFeed( MatrixFood_Matrices{ &instance, 1 }, 1 ) );
	}
}
