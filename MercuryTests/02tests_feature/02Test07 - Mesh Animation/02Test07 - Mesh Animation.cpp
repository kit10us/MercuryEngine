// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/Mesh.h>
#include <MEWinMain.h>

using namespace me;

class MyGame : public Game
{
	std::shared_ptr< Mesh > mesh;
	unify::Quaternion q;
public:
	void Startup() override;
	void Update( IRenderer * renderer, RenderInfo & renderInfo ) override;
	void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	// Load effect...
	//auto effect = GetManager< Effect>()->Add( "effect", "media/EffectColor.effect" );
	auto effect = GetManager< Effect>()->Add( "effect", "media/EffectColorAnimInst3d.effect" );

	// Create mesh...
	mesh.reset( new Mesh( GetOS()->GetRenderer( 0 ) ) );

	struct Vertex
	{
		float x, y, z;
		unify::Color diffuse;
		float m0, m1, m2, m3; // Influences
	};

	size_t segments = 10;
	size_t vertexPerTriangle = 3;
	size_t trianglesPerSegment = 8;
	size_t indicesPerTriangle = 3;
	unsigned int numberOfVertices = (segments + 1) * 4;
	unsigned int numberOfIndices = segments * trianglesPerSegment * indicesPerTriangle;
	Vertex * vbRaw = new Vertex[ numberOfVertices ];
	std::vector< Index32 > ibRaw( numberOfIndices );
			  
	for( size_t i = 0; i < segments + 1; ++i )
	{
		float x = 2.5f * sin(((float)(segments - i) / (segments) ) * 3.14159f * 0.5f);
		float z = 2.5f * sin(((float)(segments - i) / (segments) ) * 3.14159f * 0.5f);
		float y = -10 + 20.0f * ((float)i / (segments) );
		unsigned char color = (unsigned char )(255.0f * ((float)i / (segments) ));

		vbRaw[ i * 4 + 0 ] = { -x,  y, -z, unify::Color::ColorRed( color ), 0.1f, 0, 0, 0 };
		vbRaw[ i * 4 + 1 ] = {  x,  y, -z, unify::Color::ColorGreen( color ), 0.1f, 0, 0, 0 };
		vbRaw[ i * 4 + 2 ] = {  x,  y, z, unify::Color::ColorBlue( color ), 0.1f, 0, 0, 0 };
		vbRaw[ i * 4 + 3 ] = { -x,  y, z, unify::Color::ColorWhite( color ), 0.1f, 0, 0, 0 };
	};

	size_t index = 0;
	for(  size_t i = 0; i < segments; ++i )
	{
		unsigned int base = i * 4;
		auto AddTriangle = [&] ( auto a, auto b, auto c ) { 
			ibRaw[ index++ ] = base + a;
			ibRaw[ index++ ] = base + b;
			ibRaw[ index++ ] = base + c;
 		};

		AddTriangle( 0, 4, 1 );
		AddTriangle( 4, 5, 1 );

		AddTriangle( 1, 5, 2 );
		AddTriangle( 5, 6, 2 );

		AddTriangle( 2, 6, 3 );
		AddTriangle( 6, 7, 3 );

		AddTriangle( 3, 7, 0 );
		AddTriangle( 7, 4, 0 );
	}

	auto & bs = mesh->GetPrimitiveList().AddBufferSet();
	bs.AddVertexBuffer( VertexBufferParameters{ numberOfVertices, effect->GetVertexShader()->GetVertexDeclaration(), 0, vbRaw, BufferUsage::Default } );
	bs.AddIndexBuffer( IndexBufferParameters{ numberOfIndices, &ibRaw[0] } );
	bs.AddMethod( RenderMethod::CreateTriangleListIndexed( numberOfVertices, numberOfIndices, 0, 0, effect ) );
}

void MyGame::Update( IRenderer * renderer, RenderInfo & renderInfo )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 1;

	HRESULT result = S_OK;

	const float width = (float)renderer->GetViewport().GetSize().width;
	const float height = (float)renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( renderInfo.GetDelta() * 90.0f );
	/*
	if( rotation.Fix360() != 0 )
	{
		++axisIndex;
		if( axisIndex >= 3 )
		{
			axisIndex = 0;
		}
	}
	*/

	unify::V3< float > eye( 0.0f, 1.0f, -50.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	
	q = unify::Quaternion( axis, rotation );
	
	renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MyGame::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	std::vector< const unify::FrameLite * > frames;
	unify::FrameLite frame( q, unify::V3< float >( 0, 0, 0 ) );
	frames.push_back( &frame );
	mesh->Render( renderer, renderInfo, nullptr, frames );
}
