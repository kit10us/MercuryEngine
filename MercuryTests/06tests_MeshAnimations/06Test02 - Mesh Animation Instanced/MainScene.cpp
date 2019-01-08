// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>

using namespace me;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	using namespace render;

	// Load resources...
	AddResources( unify::Path{ "resources/MeshAnimation.xml" } );

	auto effect = Effect::ptr{ new me::render::Effect( 
		GetAsset< IVertexShader >( "ColorAnimInst3d" ), 
		GetAsset< IPixelShader >( "ColorAnimInst3d" )
	) };

	// Create mesh...
	m_mesh.reset( new Mesh( GetOS()->GetRenderer( 0 ) ) );

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

  		vbRaw[ i * 4 + 0 ] = { -x,  y, -z, unify::ColorRed( color ),		result_infl0, result_infl1, result_infl2, 0 };
		vbRaw[ i * 4 + 1 ] = {  x,  y, -z, unify::ColorGreen( color ),	result_infl0, result_infl1, result_infl2, 0 };
		vbRaw[ i * 4 + 2 ] = {  x,  y, z, unify::ColorBlue( color ),		result_infl0, result_infl1, result_infl2, 0 };
		vbRaw[ i * 4 + 3 ] = { -x,  y, z, unify::ColorWhite( color ),	result_infl0, result_infl1, result_infl2, 0 };
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

	auto & bs = m_mesh->GetPrimitiveList().AddBufferSet();
	bs.AddVertexBuffer( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	bs.AddIndexBuffer( { { { numberOfIndices, &ibRaw[0] } } } );
	bs.AddMethod( RenderMethod::CreateTriangleListIndexed( numberOfVertices, numberOfIndices, 0, 0, effect ) );

	// Create animation...
	using namespace frameanimation;		
	m_animation.reset( new Animation( "test", 1.0f ) );
	m_animation->AddRotationKey( 0, RotationKey( 0.25f, unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 180 ) ) ) );
	m_animation->AddRotationKey( 0, RotationKey( 0.5f, unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 0 ) ) ) );
	m_animation->AddRotationKey( 0, RotationKey( 0.75f, unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( -180 ) ) ) );
	m_animation->AddRotationKey( 0, RotationKey( 1.0f, unify::Quaternion( unify::V3< float >( 0, 0, 1 ), unify::AngleInDegrees( 0 ) ) ) );
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	static unify::Angle rotation( unify::AngleInRadians( 0.0f ) );
	static int axisIndex = 1;

	const float width = (float)params.renderer->GetViewport().GetSize().width;
	const float height = (float)params.renderer->GetViewport().GetSize().height;

	rotation += unify::AngleInDegrees( params.GetDelta().GetSeconds() * 90.0f );

	unify::V3< float > eye( 0.0f, 1.0f, -50.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );

	unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );
	
	m_q = unify::Quaternion( axis, rotation );
	m_q = unify::QuaternionIdentity();
	
	params.renderInfo.SetViewMatrix( unify::MatrixLookAtLH( eye, at, up ) );
	params.renderInfo.SetProjectionMatrix( unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, width / height, 0.01f, 100.0f ) );
}

void MainScene::OnRender( scene::RenderGirl renderGirl )
{
	// Per-mesh instance constants.
	class Source : public render::IMatrixSource
	{
	private:
		unify::FrameLite m_frame;
		unify::Matrix m_matrix;
	public:
		Source( unify::FrameLite frame, unify::Matrix matrix )
			: m_frame{ frame }
			, m_matrix{ matrix }
		{
		}

		size_t Count() const
		{
			return 2;
		}

		unify::Matrix GetMatrix( size_t i ) const
		{
			switch ( i )
			{
			case 0:
				//return m_frame.GetMatrix();
			case 1:
				//return m_matrix;
			default:
				return unify::MatrixIdentity();
			}
		}

		void CopyMatrices( unify::Matrix * matrices ) const
		{
			matrices[0] = m_frame.GetMatrix();
			matrices[1] = m_matrix;
		}
	};

	// Apply animation...
	static float progress = 0.0f;
	unify::FrameSet frameSet;
	frameSet.Add( unify::MatrixIdentity(), 0 );
	
	unify::FrameSetInstance frameSetInstance1( &frameSet );
	unify::FrameSetInstance frameSetInstance2( &frameSet );
	m_animation->ApplyToFrames( progress, frameSetInstance1 );
	m_animation->ApplyToFrames( progress, frameSetInstance2 );
	frameSetInstance1.UpdateLocals();
	frameSetInstance2.UpdateLocals();
	
	render::Params params = *renderGirl.GetParams();
	progress += params.GetDelta().GetSeconds();

	Source sorces_source[] = {
		//{ { m_q, unify::V3< float >( -10, 0, 0 ) }, unify::MatrixIdentity() /*frameSetInstance1.Local(0)*/ }, 
		//{ { m_q, unify::V3< float >( 10, 0, 0 ) }, unify::MatrixIdentity() /*frameSetInstance2.Local(0)*/ } 
		Source{ unify::MatrixTranslate( { 0, 0, 0 } ), unify::MatrixTranslate( { -10, 0, 0 } ) },
		Source{ unify::MatrixTranslate( { 0, 0, 0 } ), unify::MatrixTranslate( { 10, 0, 0 } ) }
	};

	std::vector< render::IMatrixSource * > sources{ &sorces_source[0], &sorces_source[1] };

	/*
	{
	RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effectBorg ) );

	unify::Matrix instance{ Matrix( q ) * MatrixTranslate( V3< float >( -15, 15, 10 ) ) };
	params.renderer->Render( method, params.renderInfo, render::MatrixFeed( { &instance, 1 }, 1 ) );
	}
	{
	RenderMethod method( RenderMethod::CreateTriangleList( 0, 12, effect4 ) );

	unify::Matrix instance{ Matrix( q ) * MatrixTranslate( V3< float >( 15, 15, 10 ) ) };
	params.renderer->Render( method, params.renderInfo, render::MatrixFeed( { &instance, 1 }, 1 ) );
	}
	*/

	std::vector< const unify::FrameLite * > frames;
	unify::FrameLite frame( m_q, unify::V3< float >( 0, 0, 0 ) );
	frames.push_back( &frame );
	m_mesh->Render( params, nullptr, render::MatrixFeed( render::MatrixFood_Frames{ &frames[0], frames.size() }, 1 ) );


	/*
	m_mesh->Render( 
		params, nullptr, render::MatrixFeed( 
			{ &sources[0], sources.size() }, // food (starting chunk)
			sources[0]->Count() // stride (from chunk to chunk)
		) 
	);
	*/
}
