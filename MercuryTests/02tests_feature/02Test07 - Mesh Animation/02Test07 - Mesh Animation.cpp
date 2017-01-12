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
	auto effect = GetManager< Effect>()->Add( "effect", "EffectColorAnimInst3d.effect" );

	// Create mesh...
	mesh.reset( new Mesh( GetOS()->GetRenderer( 0 ) ) );

	struct Vertex
	{
		float x, y, z;
		unify::Color diffuse;
		float m0, m1, m2, m3; // Influences
	};

	size_t segments = 30;
	size_t vertexPerTriangle = 3;
	size_t trianglesPerSegment = 8;
	size_t indicesPerTriangle = 3;
	unsigned int numberOfVertices = (segments + 1) * 4;
	unsigned int numberOfIndices = segments * trianglesPerSegment * indicesPerTriangle;
	Vertex * vbRaw = new Vertex[ numberOfVertices ];
	std::vector< Index32 > ibRaw( numberOfIndices );

	class MatrixInfluence
	{
		int m_index;
		float m_influence; 
	public:
		MatrixInfluence(int index = -1, float influence = 1.0f)
			: m_index( index )
			, m_influence( influence )
		{														
		}

		/// <summary>
		/// Matrix index, if negative, then ignored.
		/// </summary>
		void SetIndex( int index = -1 )
		{
			m_index = index;
		}

		void SetInfluence(float influence = 1.0f)
		{
			m_influence = std::min( influence, 1.0f );
			m_influence = std::max( m_influence, 0.0f );
		}

		float Result() const
		{	
			if (m_index < 0.0f || m_influence < 0.0001f) return -1.0f;
			float influence = m_influence * 0.1f;
			return m_index + influence;
		}
	};

	for( size_t i = 0; i < segments + 1; ++i )
	{
		float x = 1.5f * sin(((float)(segments - i) / (segments) ) * 3.14159f * 0.5f);
		float z = 1.5f * sin(((float)(segments - i) / (segments) ) * 3.14159f * 0.5f);
		float y = -10 + 20.0f * ((float)i / (segments) );
		unsigned char color = (unsigned char )(255.0f * ((float)i / (segments) ));
		
		MatrixInfluence infl0( 0, 1.0f );
		MatrixInfluence infl1( 1, (float)i / (segments) );
		MatrixInfluence infl2( 2, 0.0f );

		float result_infl0 = infl0.Result();
		float result_infl1 = infl1.Result();
		float result_infl2 = infl2.Result();

  		vbRaw[ i * 4 + 0 ] = { -x,  y, -z, unify::Color::ColorRed( color ),		result_infl0, result_infl1, result_infl2, 0 };
		vbRaw[ i * 4 + 1 ] = {  x,  y, -z, unify::Color::ColorGreen( color ),	result_infl0, result_infl1, result_infl2, 0 };
		vbRaw[ i * 4 + 2 ] = {  x,  y, z, unify::Color::ColorBlue( color ),		result_infl0, result_infl1, result_infl2, 0 };
		vbRaw[ i * 4 + 3 ] = { -x,  y, z, unify::Color::ColorWhite( color ),	result_infl0, result_infl1, result_infl2, 0 };
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
	bs.AddVertexBuffer( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	bs.AddIndexBuffer( { { { numberOfIndices, &ibRaw[0] } } } );
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

	static size_t animState = 0;
	static float animTime = 0;
	static float animTimeLimit = 1.0f;
	unify::Matrix animMatrix;
	animTime += renderInfo.GetDelta() * 1.3f;
	if ( animTime >= animTimeLimit )
	{
		animTime = 0.0f;
		animState++;
		if ( animState > 3 )
		{
			animState = 0;
		}
	}

	switch( animState )
	{
	case 0:
		animMatrix = unify::MatrixRotationZ( unify::AngleInDegrees( 180 * (0.0f + (animTime / animTimeLimit ) ) ) );
		break;
	case 1:
		animMatrix = unify::MatrixRotationZ( unify::AngleInDegrees( 180 * (1.0f - (animTime / animTimeLimit ) ) ) );
		break;
	case 2:
		animMatrix = unify::MatrixRotationZ( unify::AngleInDegrees( -180 * (0.0f + (animTime / animTimeLimit ) ) ) );
		break;
	case 3:
		animMatrix = unify::MatrixRotationZ( unify::AngleInDegrees( -180 * (1.0f - (animTime / animTimeLimit ) ) ) );
		break;
	};


	class Source : public IMatrixSource
	{
	private:
		const unify::FrameLite & m_frame;
		const unify::Matrix & m_matrix;
	public:
		Source( const unify::FrameLite & frame, const unify::Matrix & matrix )
			: m_frame{ frame }
			, m_matrix{ matrix }
		{
		}

		size_t Count() const
		{
			return 1;
		}

		unify::Matrix GetMatrix( size_t i ) const
		{
			switch( i )
			{
			case 0:
				return m_frame.GetMatrix();
			case 1:
				return m_matrix;
			default:
				return unify::MatrixIdentity();
			}
		}

		void CopyMatrices( unify::Matrix * matrices ) const
		{
			matrices[ 0 ] = m_frame.GetMatrix();
			matrices[ 1 ] = m_matrix;
		}
	} source( frame, animMatrix );

	mesh->Render( renderer, renderInfo, nullptr, &source, 1, true );
}
