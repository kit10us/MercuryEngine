// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <dxi/win/DXILib.h>
#include <me/RenderMethod.h>
#include <DXIWinMain.h>

#include <me/VertexUtil.h>

using namespace me;

class MyGame : public Game
{
	Effect::ptr effect;
	IVertexBuffer::ptr vertexBuffer;

public:
	void Startup() override;
	void Update( me::RenderInfo & renderInfo ) override;
	void Render( const me::RenderInfo & renderInfo, const me::Viewport & viewport ) override;
	void Shutdown() override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	const float width = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().width;
	const float height = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().height;

	effect = GetManager< Effect>()->Add( "color2d", "media/EffectColor2D.effect" );

	auto vd = effect->GetVertexShader()->GetVertexDeclaration();

	unsigned int vertexCount = 4;
	size_t sizeOfBufferInBytes = vd->GetSize( 0 ) * vertexCount;

	std::shared_ptr< unsigned char > vertices( new unsigned char[ sizeOfBufferInBytes ] );

	unify::DataLock lock( vertices.get(), vd->GetSize( 0 ), vertexCount, false, 0 );

	float depth = 0.5f;

	VertexElement positionE = CommonVertexElement::Position(0);
	VertexElement colorE = CommonVertexElement::Diffuse(0);

	WriteVertex( *vd, lock, 0, positionE, unify::V3< float >( 0, 0, depth ) );
	WriteVertex( *vd, lock, 0, colorE, unify::ColorUnit( 1, 0, 0, 1 ) );

	WriteVertex( *vd, lock, 1, positionE, unify::V3< float >( width, 0, depth ) );
	WriteVertex( *vd, lock, 1, colorE, unify::ColorUnit( 0, 1, 0, 1 ) );

	WriteVertex( *vd, lock, 2, positionE, unify::V3< float >( 0, height, depth ) );
	WriteVertex( *vd, lock, 2, colorE, unify::ColorUnit( 0, 0, 1, 1 ) );

	WriteVertex( *vd, lock, 3, positionE, unify::V3< float >( width, height, depth ) );
	WriteVertex( *vd, lock, 3, colorE, unify::ColorUnit( 1, 1, 1, 1 ) );

	vertexBuffer = GetOS()->GetRenderer(0)->ProduceVB( { vertexCount, effect->GetVertexShader()->GetVertexDeclaration(), 0, vertices.get(), BufferUsage::Default } );
}

void MyGame::Update( RenderInfo & renderInfo )
{
	const float width = (float)renderInfo.GetRenderer()->GetDisplay().GetSize().width;
	const float height = (float)renderInfo.GetRenderer()->GetDisplay().GetSize().height;
	renderInfo.SetProjectionMatrix( unify::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.01f, 100.0f ) );
}

void MyGame::Render(  const RenderInfo & renderInfo, const me::Viewport & viewport )
{
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleStrip( 0, 2, effect ) );
	method.Render( renderInfo );
}

void MyGame::Shutdown()
{
	effect.reset();
	vertexBuffer.reset();
}

