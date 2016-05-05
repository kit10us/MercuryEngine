// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "PrimitiveList"
/// goals and requirements:
/// * Illustrate the PrimitiveList, as its use is frequent, and critical.
/// </summary>

#include <dxi/core/Game.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/VertexShaderFactory.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	PrimitiveList m_primitiveList;
	PrimitiveList m_primitiveListA;
	PrimitiveList m_primitiveListB;
	PrimitiveList m_primitiveListAB;

public:
	bool Setup( core::IOS & os )
	{
		os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
		os.SetFullscreen( false );
		return true;
	}

	void Startup()
	{
		Game::Startup();

		qjson::Object vertexTextured2DVDJson = { { "Position", "Float3" }, { "Tex1", "Float2" } };
		VertexDeclaration texVd( vertexTextured2DVDJson );

		GetManager< PixelShader >()->Add( MakePixelShaderJson( "textured_2d", "media/shaders/textured2d.shader", "ps_main", "ps_1_1" ) );
		GetManager< VertexShader >()->Add( MakeVertexShaderJson( "textured_2d", "media/shaders/textured2d.shader", "vs_main", "vs_1_1", vertexTextured2DVDJson ) );

		qjson::Object vertexPlain2DVDJson = { { "Position", "Float3" }, { "Diffuse", "D3DCOLOR" } };
		VertexDeclaration colVd( vertexPlain2DVDJson );

		GetManager< PixelShader >()->Add( MakePixelShaderJson( "color_2d", "media/shaders/color2d.shader", "ps_main", "ps_1_1" ) );
		GetManager< VertexShader >()->Add( MakeVertexShaderJson( "color_2d", "media/shaders/color2d.shader", "vs_main", "vs_1_1", vertexPlain2DVDJson ) );

		// Effect with Texture, no color...
		{
			Texture::shared_ptr texture = GetManager< Texture >()->Add( "texture", new Texture( "media/Sig10_1024.jpg" ) );
			Effect::shared_ptr effect = GetManager< Effect >()->Add( "textured_2d", new Effect );
			effect->SetPixelShader( GetManager< PixelShader >()->Find( "textured_2d" ) );
			effect->SetVertexShader( GetManager< VertexShader >()->Find( "textured_2d" ) );
			effect->SetTexture( 0, texture );
		}

		// Effect with color only, no texture...
		{
			Effect::shared_ptr effect = GetManager< Effect >()->Add( "color_2d", new Effect );
			effect->SetPixelShader( GetManager< PixelShader >()->Find( "color_2d" ) );
			effect->SetVertexShader( GetManager< VertexShader >()->Find( "color_2d" ) );
		}

		WORD stream = 0;
		D3DVERTEXELEMENT9 positionE = {};
		positionE.Stream = stream;
		positionE.Type = D3DDECLTYPE_FLOAT3;
		positionE.Usage = D3DDECLUSAGE_POSITION;
		positionE.UsageIndex = 0;

		D3DVERTEXELEMENT9 diffuseE = {};
		diffuseE.Stream = stream;
		diffuseE.Type = D3DDECLTYPE_D3DCOLOR;
		diffuseE.Usage = D3DDECLUSAGE_COLOR;
		diffuseE.UsageIndex = 0;

		D3DVERTEXELEMENT9 texE = {};
		texE.Stream = stream;
		texE.Type = D3DDECLTYPE_FLOAT2;
		texE.Usage = D3DDECLUSAGE_TEXCOORD;
		texE.UsageIndex = 0;

		// Shared values...
		float z = 3.0f;

		// First two...
		{
			Effect::shared_ptr effect = GetManager< Effect >()->Find( "textured_2d" );

			BufferSet & set = m_primitiveList.AddBufferSet();

			VertexBuffer & vb = set.GetVertexBuffer();
			vb.Create( 10, texVd, BufferUsage::Staging );

			IndexBuffer & ib = set.GetIndexBuffer();
			ib.Create( 6 );

			unify::DataLock lock;
			vb.Lock( lock );

			// First method...
			set.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 6, 2, effect, false ) );

			texVd.WriteVertex( lock, 0, positionE, unify::V3< float >( 10, 10, z ) );
			texVd.WriteVertex( lock, 0, texE, unify::TexCoords( 0, 0 ) );
			texVd.WriteVertex( lock, 1, positionE, unify::V3< float >( 300, 10, z ) );
			texVd.WriteVertex( lock, 1, texE, unify::TexCoords( 1, 0 ) );
			texVd.WriteVertex( lock, 2, positionE, unify::V3< float >( 10, 300, z ) );
			texVd.WriteVertex( lock, 2, texE, unify::TexCoords( 0, 1 ) );
			texVd.WriteVertex( lock, 3, positionE, unify::V3< float >( 300, 10, z ) );
			texVd.WriteVertex( lock, 3, texE, unify::TexCoords( 1, 0 ) );
			texVd.WriteVertex( lock, 4, positionE, unify::V3< float >( 300, 300, z ) );
			texVd.WriteVertex( lock, 4, texE, unify::TexCoords( 1, 1 ) );
			texVd.WriteVertex( lock, 5, positionE, unify::V3< float >( 10, 300, z ) );
			texVd.WriteVertex( lock, 5, texE, unify::TexCoords( 0, 1 ) );

			// Second method...
			set.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 6, 4, 0, 2, effect, true ) );

			texVd.WriteVertex( lock, 6, positionE, unify::V3< float >( 310, 10, z ) );
			texVd.WriteVertex( lock, 6, texE, unify::TexCoords( 0, 0 ) );
			texVd.WriteVertex( lock, 7, positionE, unify::V3< float >( 620, 10, z ) );
			texVd.WriteVertex( lock, 7, texE, unify::TexCoords( 1, 0 ) );
			texVd.WriteVertex( lock, 8, positionE, unify::V3< float >( 310, 300, z ) );
			texVd.WriteVertex( lock, 8, texE, unify::TexCoords( 0, 1 ) );
			texVd.WriteVertex( lock, 9, positionE, unify::V3< float >( 620, 300, z ) );
			texVd.WriteVertex( lock, 9, texE, unify::TexCoords( 1, 1 ) );

			IndexLock indexLock;
			ib.Lock( indexLock );
			indexLock.SetIndex( 0, 6 );
			indexLock.SetIndex( 1, 7 );
			indexLock.SetIndex( 2, 8 );
			indexLock.SetIndex( 3, 7 );
			indexLock.SetIndex( 4, 9 );
			indexLock.SetIndex( 5, 8 );

			vb.Unlock();
			ib.Unlock();
		}

		// A...
		{
			Effect::shared_ptr effect = GetManager< Effect >()->Find( "color_2d" );

			BufferSet & set = m_primitiveListA.AddBufferSet();
			VertexBuffer & vb = set.GetVertexBuffer();
			vb.Create( 6, colVd, BufferUsage::Staging );

			unify::DataLock lock;
			vb.Lock( lock );

			set.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 6, 2, effect, false ) );

			colVd.WriteVertex( lock, 0, positionE, unify::V3< float >( 10, 310, z ) );
			colVd.WriteVertex( lock, 0, diffuseE, unify::Color::ColorRed() );
			colVd.WriteVertex( lock, 1, positionE, unify::V3< float >( 110, 310, z ) );
			colVd.WriteVertex( lock, 1, diffuseE, unify::Color::ColorRed() );
			colVd.WriteVertex( lock, 2, positionE, unify::V3< float >( 10, 410, z ) );
			colVd.WriteVertex( lock, 2, diffuseE, unify::Color::ColorRed() );
			colVd.WriteVertex( lock, 3, positionE, unify::V3< float >( 110, 310, z ) );
			colVd.WriteVertex( lock, 3, diffuseE, unify::Color::ColorRed() );
			colVd.WriteVertex( lock, 4, positionE, unify::V3< float >( 110, 410, z ) );
			colVd.WriteVertex( lock, 4, diffuseE, unify::Color::ColorRed() );
			colVd.WriteVertex( lock, 5, positionE, unify::V3< float >( 10, 410, z ) );
			colVd.WriteVertex( lock, 5, diffuseE, unify::Color::ColorRed() );

			vb.Unlock();
		}

		// B...
		{
			Effect::shared_ptr effect = GetManager< Effect >()->Find( "color_2d" );

			BufferSet & set = m_primitiveListB.AddBufferSet();

			VertexBuffer & vb = set.GetVertexBuffer();
			vb.Create( 6, colVd, BufferUsage::Staging );

			unify::DataLock lock;
			vb.Lock( lock );

			set.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 6, 2, effect, false ) );

			colVd.WriteVertex( lock, 0, positionE, unify::V3< float >( 10 + 110, 310, z ) );
			colVd.WriteVertex( lock, 0, diffuseE, unify::Color::ColorGreen() );
			colVd.WriteVertex( lock, 1, positionE, unify::V3< float >( 110 + 110, 310, z ) );
			colVd.WriteVertex( lock, 1, diffuseE, unify::Color::ColorGreen() );
			colVd.WriteVertex( lock, 2, positionE, unify::V3< float >( 10 + 110, 410, z ) );
			colVd.WriteVertex( lock, 2, diffuseE, unify::Color::ColorGreen() );
			colVd.WriteVertex( lock, 3, positionE, unify::V3< float >( 110 + 110, 310, z ) );
			colVd.WriteVertex( lock, 3, diffuseE, unify::Color::ColorGreen() );
			colVd.WriteVertex( lock, 4, positionE, unify::V3< float >( 110 + 110, 410, z ) );
			colVd.WriteVertex( lock, 4, diffuseE, unify::Color::ColorGreen() );
			colVd.WriteVertex( lock, 5, positionE, unify::V3< float >( 10 + 110, 410, z ) );
			colVd.WriteVertex( lock, 5, diffuseE, unify::Color::ColorGreen() );

			vb.Unlock();
		}

		// AB...
		{
			m_primitiveListAB.Append( m_primitiveListA );
			m_primitiveListAB.Append( m_primitiveListB );

			// Offset X coordinate to ensure we can compare the results.
			unify::DataLock lock;
			m_primitiveListAB.GetBufferSet( 0 ).GetVertexBuffer().Lock( lock );
			unify::DataLock::iterator< unify::V2< float > > itr = lock.begin< unify::V2< float > >();
			for( ; itr != lock.end< unify::V2< float > >(); ++itr )
			{
				(*itr).x += 300;
			}
			m_primitiveListAB.GetBufferSet( 0 ).GetVertexBuffer().Unlock();
		}
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		return Game::Update( elapsed, input );
	}

	void Render()
	{
		RenderInfo renderInfo;
		unify::Matrix projectionMatrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, static_cast< float >( GetOS()->GetResolution().width ), static_cast< float >( GetOS()->GetResolution().height ), 0, 1, 1000 );
		renderInfo.SetFinalMatrix( projectionMatrix );

		m_primitiveList.Render( renderInfo );
		m_primitiveListA.Render( renderInfo );
		m_primitiveListB.Render( renderInfo );
		m_primitiveListAB.Render( renderInfo );
	}

	void Shutdown()
	{
		m_primitiveList.Destroy();
		m_primitiveListA.Destroy();
		m_primitiveListB.Destroy();
		m_primitiveListAB.Destroy();
		Game::Shutdown();
	}
} game;

