// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/Effect.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
	// We intend to keep these and use them directly, in an attempt to illustrate their usages.
	Effect::shared_ptr m_effect;
	VertexBuffer m_VB;
	IndexBuffer m_IB;

public:
	bool Setup( core::IOS & os );
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;

bool MyGame::Setup( core::IOS & os )
{
	os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
	os.SetFullscreen( false );
	return true;
}

namespace VertexAdapter
{
	template< typename T >
	class IWriter
	{
	public:
		virtual void operator()( VertexLock & lock, size_t index, const T & form ) = 0;
	};

	template< typename T >
	class GenericWriter : public IWriter< T >
	{
	private:
		size_t m_offset;
	public:
		GenericWriter( size_t offset )
			: m_offset( offset )
		{
		}

		virtual void operator()( VertexLock & lock, size_t index, const T & element )
		{
			assert( index < lock.Count() );
			assert( m_offset + sizeof( T ) <= lock.Stride() );
			T * place = (T*)((unsigned char*)lock.GetItem( index ) + m_offset);
			*place = element;
		}
	};
}

void MyGame::Startup()
{
	Game::Startup();

	// Load a texture...
	Texture::shared_ptr texture = GetTextureManager()->Add( "image", new Texture( "media/Sig10_1024.jpg" ) );

	// Create a vertex declaration...	
	VertexFormat vertexFormat = FVF::XYZ | FVF::Tex1;
	VertexDeclaration::shared_ptr vertexDeclaration( new VertexDeclaration( vertexFormat ) );

	// Load shaders...
	PixelShader::shared_ptr ps = GetPixelShaderManager()->LoadFromFile( "textured_2d", "media/shaders/textured2d.shader", "ps_main", "ps_1_1" );
	VertexShader::shared_ptr vs = GetVertexShaderManager()->LoadFromFile( "textured_2d", "media/shaders/textured2d.shader", "vs_main", "vs_1_1", vertexDeclaration );

	// Create an effect...
	m_effect = GetEffectManager()->Add( "default", new Effect );
	m_effect->SetCulling( CullNone );
	m_effect->SetTexture( 0, texture );
	m_effect->SetPixelShader( ps );
	m_effect->SetVertexShader( vs );

	// Create a vertex buffer...
	m_VB.Create( 10, vertexDeclaration, BufferUsage::Default );

	VertexLock vertexLock;
	m_VB.Lock( vertexLock );

	VertexAdapter::GenericWriter< unify::V3< float > > positionWriter( 0 );
	VertexAdapter::GenericWriter< unify::TexCoords > texCoordWriter( sizeof( unify::V3< float > ) );

	positionWriter( vertexLock, 0, unify::V3< float >( 10, 10, 0 ) );
	texCoordWriter( vertexLock, 0, unify::TexCoords( 0, 0 ) );
	positionWriter( vertexLock, 1, unify::V3< float >( 300, 10, 0 ) );
	texCoordWriter( vertexLock, 1, unify::TexCoords( 1, 0 ) );
	positionWriter( vertexLock, 2, unify::V3< float >( 10, 300, 0 ) );
	texCoordWriter( vertexLock, 2, unify::TexCoords( 0, 1 ) );
	vertexLock.CopyItemFromTo( 1, 3 );
	positionWriter( vertexLock, 4, unify::V3< float >( 300, 300, 0 ) );
	texCoordWriter( vertexLock, 4, unify::TexCoords( 1, 1 ) );
	vertexLock.CopyItemFromTo( 2, 5 );

	positionWriter( vertexLock, 6, unify::V3< float >( 310, 10, 0 )  );
	texCoordWriter( vertexLock, 6, unify::TexCoords( 0, 0 ) );
	positionWriter( vertexLock, 7, unify::V3< float >( 620, 10, 0 ) );
	texCoordWriter( vertexLock, 7, unify::TexCoords( 1, 0 ) );
	positionWriter( vertexLock, 8, unify::V3< float >( 310, 300, 0 ) );
	texCoordWriter( vertexLock, 8, unify::TexCoords( 0, 1 ) );
	positionWriter( vertexLock, 9, unify::V3< float >( 620, 300, 0 ) );
	texCoordWriter( vertexLock, 9, unify::TexCoords( 1, 1 ) );

	struct M
	{
		unify::V3< float > v;
		unify::TexCoords coords;
	};
	M * m = (M*)vertexLock.GetData();

	m_VB.Unlock();

	m_IB.Create( 6, IndexFormat::Index16, BufferUsage::Default );
	IndexLock indexLock;
	m_IB.Lock( indexLock );
	indexLock.SetIndex( 0, 6 );
	indexLock.SetIndex( 1, 7 );
	indexLock.SetIndex( 2, 8 );
	indexLock.SetIndex( 3, 7 );
	indexLock.SetIndex( 4, 9 );
	indexLock.SetIndex( 5, 8 );
	m_IB.Unlock();
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	// -- Update code goes here. --

	// Return this.
	return Game::Update( elapsed, input );
}

void MyGame::Render()
{
	// -- Render code goes here. --
	RenderInfo renderInfo;
	m_effect->Use( renderInfo );

	RenderMethod methodNoIB = RenderMethod( PrimitiveType::TriangleList, 0, 3, 2, m_effect );
	m_VB.Use();
	methodNoIB.Render( renderInfo );

	RenderMethod methodWithIB = RenderMethod(  PrimitiveType::TriangleList, 0, 6, 4, 0, 2, m_effect );
	m_IB.UseIB();
	methodWithIB.Render( renderInfo );

	Game::Render();
}

void MyGame::Shutdown()
{
	// -- Shutdown code goes here. --

	m_VB.Release();

	// Do this last.
	Game::Shutdown();
}

