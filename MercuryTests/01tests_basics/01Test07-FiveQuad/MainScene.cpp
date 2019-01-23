// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>

using namespace me;
using namespace scene;
using namespace render;

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	// Get the display's width and height.
	const float screenWidth = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().width;
	const float screenHeight = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().height;	
	
	// Add a color effect.
	auto colorEffect = GetManager< Effect >()->Add( "color_2d", unify::Path( "EffectColor2D.effect" ) );
	auto textureEffect = GetManager< Effect>()->Add( "texture_2d", unify::Path( "EffectTexture.effect" ) );
	auto textureAndColorEffect = GetManager< Effect>()->Add( "texture_and_color_2d", unify::Path( "TextureAndColor2D.effect" ) );
	auto programmaticTextureEffect( *colorEffect );

	// Calculate vertex count and buffer size.
	unsigned int vertexCount = 4;

	m_quads.reset( new PrimitiveList( GetOS()->GetRenderer( 0 ) ) );

	// Quad positions.
	std::vector< unify::V2< float > > quadPositions =
		{ 
			{  0.0f, 0.0f },				{ screenWidth * 0.5f, 0.0f },
			{ 0.0f, screenHeight * 0.5f },	{ screenWidth * 0.5f, screenHeight * 0.5f },
					{ screenWidth * 0.25f, screenHeight * 0.15f }
		};
	
	// Quad effects.
	m_quads->AddBufferSet(
		BufferSet::ptr(
			new BufferSet( GetOS()->GetRenderer( 0 ), colorEffect )
		) );

	m_quads->AddBufferSet(
		BufferSet::ptr(
			new BufferSet( GetOS()->GetRenderer( 0 ), textureEffect )
		) );

	m_quads->AddBufferSet(
		BufferSet::ptr(
			new BufferSet( GetOS()->GetRenderer( 0 ), textureAndColorEffect )
		) );

	m_quads->AddBufferSet(
		BufferSet::ptr(
			new BufferSet( GetOS()->GetRenderer( 0 ), textureAndColorEffect )
		) );

	m_quads->AddBufferSet(
		BufferSet::ptr(
			new BufferSet( GetOS()->GetRenderer( 0 ), textureAndColorEffect )
		) );

	/*
	m_quads = std::vector< Quad >( 5 );

	m_quads[ 0 ].effect = render::Effect::ptr( new render::Effect( *colorEffect ) );			// Per-vertex color.
	m_quads[ 0 ].vertexCB = m_quads[0].effect->GetVertexShader()->CreateConstantBuffer( BufferUsage::Dynamic );
	m_quads[ 0 ].pixelCB = m_quads[0].effect->GetPixelShader()->CreateConstantBuffer( BufferUsage::Dynamic );

	m_quads[ 1 ].effect = render::Effect::ptr( new render::Effect( *textureEffect ) );			// Programmatic colored.
	m_quads[ 1 ].vertexCB = m_quads[0].effect->GetVertexShader()->CreateConstantBuffer( BufferUsage::Dynamic );
	m_quads[ 1 ].pixelCB = m_quads[0].effect->GetPixelShader()->CreateConstantBuffer( BufferUsage::Dynamic );

	m_quads[ 2 ].effect = render::Effect::ptr( new render::Effect( *textureEffect ) );			// Textured.
	m_quads[ 2 ].vertexCB = m_quads[0].effect->GetVertexShader()->CreateConstantBuffer( BufferUsage::Dynamic );
	m_quads[ 2 ].pixelCB = m_quads[0].effect->GetPixelShader()->CreateConstantBuffer( BufferUsage::Dynamic );

	m_quads[ 3 ].effect = render::Effect::ptr( new render::Effect( *textureAndColorEffect ) );	// Textured and colored.
	m_quads[ 3 ].vertexCB = m_quads[0].effect->GetVertexShader()->CreateConstantBuffer( BufferUsage::Dynamic );
	m_quads[ 3 ].pixelCB = m_quads[0].effect->GetPixelShader()->CreateConstantBuffer( BufferUsage::Dynamic );

	m_quads[ 4 ].effect = render::Effect::ptr( new render::Effect( *textureAndColorEffect ) );	// Textured, colored, and transparencies.
	m_quads[ 4 ].vertexCB = m_quads[0].effect->GetVertexShader()->CreateConstantBuffer( BufferUsage::Dynamic );
	m_quads[ 4 ].pixelCB = m_quads[0].effect->GetPixelShader()->CreateConstantBuffer( BufferUsage::Dynamic );

	// Quad depth.
	m_quads[ 0 ].depth = 0.5f;
	m_quads[ 1 ].depth = 0.5f;
	m_quads[ 2 ].depth = 0.5f;
	m_quads[ 3 ].depth = 0.5f;
	m_quads[ 4 ].depth = 0.25f;
	*/
	float quadDepth[] =
	{
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.25f
	};

	// Programmatic texture.
	{
		TextureParameters parameters;
		parameters.lockable = true;
		parameters.renderable = false;
		parameters.size = unify::Size< unsigned int >( 512, 512 );

		auto programmaticTexture = GetOS()->GetRenderer( 0 )->ProduceT( parameters );
		
		TextureLock lock;
		programmaticTexture->LockRect( 0, lock, 0, unify::DataLockAccess::ReadWrite );

		unify::ColorUnit * colorBuffer = reinterpret_cast< unify::ColorUnit * >(lock.pBits);
		for( unsigned int y = 0; y < lock.uHeight; y++ )
		{
			for( unsigned int x = 0; x < lock.uWidth; x++ ) 
			{
				colorBuffer[ x + y * lock.uWidth ] = 
					unify::ColorUnitRGBA( 
						(float)x / (float)lock.uWidth,
						(float)y / (float)lock.uWidth,
						(float)x / (float)lock.uWidth * (float)y / (float)lock.uWidth,
						1.0f
					);
			}
		}

		programmaticTexture->UnlockRect( 0 );

		m_quads->GetBufferSet( 1 ).GetEffect()->SetTexture( 0, programmaticTexture );
	}

	for ( unsigned int i = 0; i < 5; i++ )
	{
		using namespace unify;

		auto & bs = m_quads->GetBufferSet( i );

		// Get a vertex declarations.
		auto vd = bs.GetEffect()->GetVertexShader()->GetVertexDeclaration();

		// Set a depth value to be used with all vertices.
		float depth = quadDepth[ i ];

		size_t sizeOfBufferInBytes = vd->GetSizeInBytes( 0 ) * vertexCount;

		// Allocate a temporary buffer for vertices.
		std::shared_ptr< unsigned char > vertices( new unsigned char[ sizeOfBufferInBytes ] );

		// Lock buffer for writing.
		unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, unify::DataLockAccess::ReadWrite, 0 );


		// Vertex elements allow us to write to vertices.
		// Note that WriteVertex usese the vertex declaration to know which elements it can set, allowing
		//    us to arbitrarily set a vertices values, even if it just ingores them (share creation code).
		VertexElement positionE = CommonVertexElement::Position(0);
		VertexElement colorE = CommonVertexElement::Diffuse(0);
		VertexElement textureE = CommonVertexElement::TexCoords(0);

		// Write to the vertex buffer elements...

		WriteVertex( *vd, lock, 0, positionE, V3< float >( quadPositions[i], depth ) );
		WriteVertex( *vd, lock, 0, colorE, ColorUnit( 1, 0, 0, 1 ) );
		WriteVertex( *vd, lock, 0, textureE, TexCoords( 0, 0 ) );

		WriteVertex( *vd, lock, 1, positionE, unify::V3< float >( quadPositions[i] + unify::V2< float >( screenWidth, 0 ) * 0.5f, depth ) );
		WriteVertex( *vd, lock, 1, colorE, unify::ColorUnit( 0, 1, 0, 1 ) );
		WriteVertex( *vd, lock, 1, textureE, TexCoords( 1, 0 ) );

		WriteVertex( *vd, lock, 2, positionE, unify::V3< float >( quadPositions[i] + unify::V2< float >( 0, screenHeight ) * 0.5f, depth ) );
		WriteVertex( *vd, lock, 2, colorE, unify::ColorUnit( 0, 0, 1, 1 ) );
		WriteVertex( *vd, lock, 2, textureE, TexCoords( 0, 1 ) );

		WriteVertex( *vd, lock, 3, positionE, unify::V3< float >( quadPositions[i] + unify::V2< float >{ screenWidth, screenHeight } * 0.5f, depth ) );
		WriteVertex( *vd, lock, 3, colorE, unify::ColorUnit( 1, 1, 1, 1 ) );
		WriteVertex( *vd, lock, 3, textureE, TexCoords( 1, 1 ) );

		// Create a vertex buffer from our temporary buffer...
		bs.AddVertexBuffer( 
		{ 
			bs.GetEffect()->GetVertexShader()->GetVertexDeclaration(),
			{ 
				{ vertexCount, vertices.get() } 
			}, 
			BufferUsage::Default 
		} );

		bs.AddMethod( RenderMethod::CreateTriangleStrip( 0, 2 ) );
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	const float width = (float)params.renderer->GetDisplay().GetSize().width;
	const float height = (float)params.renderer->GetDisplay().GetSize().height;
	params.renderInfo.SetProjectionMatrix( unify::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.01f, 100.0f ) );
}

void MainScene::OnRender( me::scene::RenderGirl renderGirl )
{
	unify::Matrix instance{ unify::MatrixIdentity() };
	m_quads->Render( *renderGirl.GetParams(), render::MatrixFeed( render::MatrixFood_Matrices{ &instance, 1 }, 1 ) );
}
