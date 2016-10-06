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
	Effect::ptr effect;
	VertexBuffer::ptr vertexBuffer;

	scene::Object::shared_ptr m_cameraObject;
	scene::Camera m_camera;

public:
	void Startup() override;
	bool Update( RenderInfo & renderInfo, IInput & input ) override;
	void Render( const RenderInfo & renderInfo ) override;
	void Shutdown() override;
} game;

void MyGame::Startup()
{
	Game::Startup();

	effect = GetManager< Effect>()->Add( "textured_3d", "media/EffectTextured.xml" );

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

	vertexBuffer.reset( new VertexBuffer( numberOfVertices, effect->GetVertexShader()->GetVertexDeclaration(), vbRaw, BufferUsage::Default ) );

	m_cameraObject.reset( new scene::Object() );
	m_cameraObject->GetFrame().SetPosition( unify::V3< float >( 0.0f, 1.0f, -50.0f ) );
	m_camera.SetObject( m_cameraObject );

	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	m_camera.GetObject()->GetFrame().LookAt( at, up );

	const float width = (float)GetOS().GetResolution().width;
	const float height = (float)GetOS().GetResolution().height;
	m_camera.SetProjection( unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

bool MyGame::Update( RenderInfo & renderInfo, IInput & input )
{
	static unify::Angle rotation( unify::Angle::AngleInRadians( 0.0f ) );
	static unify::Angle changeOverTime( unify::Angle::AngleInRadians( 0.00525f *0.5f ) );
	static int axisIndex = 0;

	rotation += changeOverTime;
	if( rotation.Fix360() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	unify::Matrix worldMatrix = unify::Matrix::MatrixRotationAboutAxis( axis, rotation );

	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	m_camera.GetObject()->GetFrame().LookAt( at, up );

	renderInfo.SetWorldMatrix( worldMatrix );
	renderInfo.SetViewMatrix( m_camera.GetMatrix().Inverse() );
	renderInfo.SetProjectionMatrix( m_camera.GetProjection() );
	
	return true;
}

void MyGame::Render( const RenderInfo & renderInfo )
{
	//vertexBuffer->Use();

	//RenderMethod method( PrimitiveType::TriangleList, 0, vertexBuffer->GetLength(), 12, effect );
	//method.Render( renderInfo );

	Game::Render( renderInfo );
}

void MyGame::Shutdown()
{
	effect.reset();
	vertexBuffer.reset();

	Game::Shutdown();
}

