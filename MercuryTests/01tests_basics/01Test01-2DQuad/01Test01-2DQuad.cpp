// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/RenderMethod.h>
#include <MEWinMain.h>

#include <me/VertexUtil.h>

using namespace me;

class MyGame : public Game
{
	Effect::ptr effect;
	IVertexBuffer::ptr vertexBuffer;

public:
	void Startup() override;
	void Update( IRenderer * renderer, me::RenderInfo & renderInfo ) override;
	void Render( IRenderer * renderer, const me::RenderInfo & renderInfo ) override;
	void Shutdown() override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	const float width = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().width;
	const float height = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().height;

	effect = GetManager< Effect>()->Add( "color2d", "EffectColor2D.effect" );

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

void MyGame::Update( IRenderer * renderer, RenderInfo & renderInfo )
{
	const float width = (float)renderer->GetDisplay().GetSize().width;
	const float height = (float)renderer->GetDisplay().GetSize().height;
	renderInfo.SetProjectionMatrix( unify::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.01f, 100.0f ) );
}

void MyGame::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleStrip( 0, 2, effect ) );

	unify::Matrix instance{ unify::MatrixIdentity() };
	renderer->Render( method, renderInfo, &instance, 1 );
}

void MyGame::Shutdown()
{
	effect.reset();
	vertexBuffer.reset();
}

