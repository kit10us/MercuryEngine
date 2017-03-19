// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>

using namespace me;
using namespace scene;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
	const float width = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().width;
	const float height = (float)GetOS()->GetRenderer( 0 )->GetDisplay().GetSize().height;

	effect = GetManager< Effect>()->Add( "color2d", "EffectColor2D.effect" );

	auto vd = effect->GetVertexShader()->GetVertexDeclaration();

	unsigned int vertexCount = 4;
	size_t sizeOfBufferInBytes = vd->GetSizeInBytes( 0 ) * vertexCount;

	std::shared_ptr< unsigned char > vertices( new unsigned char[ sizeOfBufferInBytes ] );

	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

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

	vertexBuffer = GetOS()->GetRenderer( 0 )->ProduceVB( { effect->GetVertexShader()->GetVertexDeclaration(), { { vertexCount, vertices.get() } }, BufferUsage::Default } );
}

void MainScene::OnUpdate( UpdateParams params )
{
	const float width = (float)params.renderer->GetDisplay().GetSize().width;
	const float height = (float)params.renderer->GetDisplay().GetSize().height;
	params.renderInfo.SetProjectionMatrix( unify::MatrixOrthoOffCenterLH( 0, width, height, 0, 0.01f, 100.0f ) );
}

void MainScene::OnRender( RenderParams params )
{
	vertexBuffer->Use();

	RenderMethod method( RenderMethod::CreateTriangleStrip( 0, 2, effect ) );

	unify::Matrix instance{ unify::MatrixIdentity() };
	params.renderer->Render( method, params.renderInfo, MatrixFeed( &instance, 1, 1 ) );
}
