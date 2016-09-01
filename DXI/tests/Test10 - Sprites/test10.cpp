// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/animation/SpriteManager.h>
#include <dxi/Sprite.h>
#include <dxi/SpriteList.h>
#include <dxi/win/DXDevice.h>
#include <dxi/RenderState.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	animation::SpriteManager::shared_ptr m_spriteManager;
	VertexBuffer m_triangleVB;
	Sprite m_spriteStatic;
	Sprite m_spriteAnimated;
	Sprite m_spriteAnimatedSized;
	SpriteList m_spriteList;
	scene::Camera::shared_ptr m_camera;
	scene::Object::shared_ptr m_cameraObject;

public:
	void Startup()
	{
		Game::Startup();
	
		// Setup camera...
		// Create an object to use as a camera, and a camera interface. Set the projection to a default projection.
		m_cameraObject.reset( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
		m_camera.reset( new scene::Camera( m_cameraObject ) );
		m_camera->SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
		m_camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 0, 10 ) );

		// Create managers...
		m_spriteManager = animation::SpriteManager::shared_ptr( new animation::SpriteManager );

		// Color 3D effect...
		Effect::shared_ptr color3DEffect = GetManager< Effect >()->Add( "color_3d", "media/EffectColor.xml" );

		// Textured 2D effect...
		Effect::shared_ptr textured2DEffect = GetManager< Effect >()->Add( "textured_2d", "media/EffectTextured2D.xml" );
		
		VertexBuffer::shared_ptr scratchVBForTextured2D( new VertexBuffer( 6, textured2DEffect->GetVertexShader()->GetVertexDeclaration() ) );
		textured2DEffect->SetScratchVertexBuffer( scratchVBForTextured2D );

		RenderState::Set( RenderState::CullMode, RenderState::CullModeValues::None );

		// Setup a 3D geometry for relative comparisons.
		m_triangleVB.Create( 6, color3DEffect->GetVertexShader()->GetVertexDeclaration() );
		unify::DataLock vertexLock;
		m_triangleVB.Lock( vertexLock );
		struct MyVertex
		{
			float x,y,z;
			unsigned int color;
		};
		MyVertex aTriangle[ ] = { 
			{ -2.0f, -2.0f, 0, 0xffff0000 }, 
			{ 0.0f, 2.0f, 0, 0xff0000ff }, 
			{ 2.0f, -2.0f, 0, 0xff00ff00 },

			{ 0, -2.0f, -2.0f, 0xffff0000 }, 
			{ 0, 2.0f, 0.0f, 0xff00ff00 }, 
			{ 0, -2.0f, 2.0f, 0xff0000ff } 
		};
		vertexLock.CopyBytesFrom( aTriangle, 0, sizeof( aTriangle ) );
		m_triangleVB.Unlock();

		GetManager< Texture >()->Add( "sprites", "media/mainnpcs.dds" );
		m_spriteManager->LoadFromFile( "media/npcs.xml" );
		Texture::shared_ptr sprites = GetManager< Texture >()->Find( "sprites" );

		// Create a static/non-animated sprite without using the SpriteManager...
		animation::Frame frame( sprites, sprites->GetSprite( "cecil", 3 ) );
		m_spriteStatic = Sprite( textured2DEffect, unify::V2< float>( 100, 100 ), unify::Size< float >( 100, 100 ), frame );

		// Create an animated sprite via the SpriteManager...
		animation::Instance animationInstance( m_spriteManager->FindGroup( "cecil" )->FindSequence( "cheer" ) );
		m_spriteAnimated = Sprite( textured2DEffect, unify::V2< float>( 300, 100 ), unify::Size< float >( 100, 100 ), animationInstance );
		
		// Create an animated sprite of the exact size of the image...
		m_spriteAnimatedSized = Sprite( textured2DEffect, unify::V2< float>( 500, 100 ), animationInstance );

		const auto & cecilGroup = m_spriteManager->FindGroup( "cecil" );
		const auto & dragoonGroup = m_spriteManager->FindGroup( "dragoon" );
		for( unsigned int i = 0; i < 24; ++i )
		{
			const std::shared_ptr< animation::Group > & randomGroup = ( rand() % 2 ) ? cecilGroup : dragoonGroup;
			m_spriteList.AddSprite( 
				Sprite( textured2DEffect, unify::V2< float >( 50 + i * 30.0f, 200 + i * 10.0f),
					unify::V2< float >( 2.0f, 2.0f ),
					animation::Instance( randomGroup->GetSequence( rand() % randomGroup->Count() ) )
					) 
				);
		}
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		m_camera->GetObject()->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) );
		m_camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

		m_spriteStatic.Update( elapsed );
		m_spriteAnimated.Update( elapsed );
		m_spriteAnimatedSized.Update( elapsed );
		m_spriteList.Update( elapsed );

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		Game::Render();

		// Render the 3D screen comparison object...
		RenderInfo renderInfo;
		renderInfo.SetFinalMatrix( m_camera->GetMatrix() );
		Effect::shared_ptr effect = GetManager< Effect >()->Find( "color_3d" );
		effect->Use( renderInfo );
		m_triangleVB.Use();
		m_triangleVB.GetVertexDeclaration().Use();
		win::DX::GetDxDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 3 );

		// Render the sprite objects...
		RenderInfo renderInfo2D;
		unify::Matrix projectionMatrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, static_cast< float >( GetOS().GetResolution().width ), static_cast< float >( GetOS().GetResolution().height ), 0, 1, 1000 );
		renderInfo2D.SetFinalMatrix( projectionMatrix );
		m_spriteStatic.Render( renderInfo2D );
		m_spriteAnimated.Render( renderInfo2D );
		m_spriteAnimatedSized.Render( renderInfo2D );
		m_spriteList.Render( renderInfo2D );
	}

	void Shutdown()
	{
		m_spriteManager.reset();
		Game::Shutdown();
	}
} game;

