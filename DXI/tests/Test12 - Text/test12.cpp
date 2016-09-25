// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

//
// A set of tests for texts.
//

#include <dxi/core/Game.h>
#include <dxi/Texture.h>
#include <dxi/scene/Camera.h>
#include <dxi/scene/Object.h>
#include <dxi/animation/SpriteManager.h>
#include <dxi/font/FontManager.h>
#include <dxi/Sprite.h>
#include <dxi/SpriteList.h>
#include <dxi/font/TextBox.h>
#include <dxi/font/TextBox3D.h>
#include <dxi/win/DxDevice.h>
#include <dxi/RenderState.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	unify::Matrix m_view;
	unify::Angle m_rotation;

	Texture::shared_ptr m_sprites;
	animation::SpriteManager::shared_ptr m_spriteManager;
	font::FontManager::shared_ptr m_fontManager;
	VertexBuffer m_triangleVB;
	std::shared_ptr< font::TextBox > m_textBoxSimple;
	std::shared_ptr< font::IText > m_textBoxSimpleWith3DCharacters;
	std::shared_ptr< font::TextBox3D > m_textBoxSimpleIn3D;
	std::shared_ptr< font::TextBox3D > m_textBoxSimpleWidth3DCharactersIn3D;

public:
	void Startup()
	{
		Game::Startup();

		// Color effect
		qjson::Object colorVDJson = { { "Position", "float3" },{ "Diffuse", "Color" } };
		VertexDeclaration colorVD( colorVDJson ); 
		GetManager< PixelShader >()->Add( MakePixelShaderJson( "color_3d", "media/shaders/color3d.shader", "ps_main", "ps_1_1" ) );
		GetManager< VertexShader >()->Add( MakeVertexShaderJson( "color_3d", "media/shaders/color3d.shader", "vs_main", "vs_1_1", colorVDJson ) );
		Effect::shared_ptr effect = GetManager< Effect >()->Add( "color_3d", new Effect );
		effect->SetPixelShader( GetManager< PixelShader >()->Find( "color_3d" ) );
		effect->SetVertexShader( GetManager< VertexShader >()->Find( "color_3d" ) );

		// Textured 2D
		qjson::Object textured2DVDJson = { { "Position", "float4" }, { "Tex1", "float2" } };
		VertexDeclaration textured2DVD( textured2DVDJson );
		GetManager< PixelShader >()->Add( MakePixelShaderJson( "textured_2d", "media/shaders/textured2d.shader", "ps_main", "ps_1_1" ) );
		GetManager< VertexShader >()->Add( MakeVertexShaderJson( "textured_2d", "media/shaders/textured2d.shader", "vs_main", "vs_1_1", textured2DVDJson ) );

		// Plain (color) 2D
		qjson::Object plain2DVDJson = { { "Position", "float4" },{ "Diffuse", "Color" } };
		VertexDeclaration plain2DVD( plain2DVDJson );
		GetManager< PixelShader >()->Add( MakePixelShaderJson( "color_2d", "media/shaders/color2d.shader", "ps_main", "ps_1_1" ) );
		GetManager< VertexShader >()->Add( MakeVertexShaderJson( "color_2d", "media/shaders/color2d.shader", "vs_main", "vs_1_1", plain2DVDJson ) );

		// Create view matrix for use as camera....
		m_view.BecomeIdentity();
		m_view.Translate( unify::V3< float >( 0, 5, -10 ) );
		m_view.LookAtLH( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

		// Create a demonstration of 3d via a 3d rotating triangle...
		m_triangleVB.Create( 6, colorVD );
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

		// Create managers...
		m_spriteManager = animation::SpriteManager::shared_ptr( new animation::SpriteManager( this ) );
		m_fontManager = font::FontManager::shared_ptr( new font::FontManager( m_spriteManager ) );

		m_fontManager->LoadFromFile( "font1_2D", "media/font1_2D.xml" );
		m_fontManager->LoadFromFile( "font1_3D", "media/font1_3D.xml" );

		m_textBoxSimple.reset( new font::TextBox( m_fontManager->FindFont( "font1_2D" ), unify::V2< float >( 30, 30 ), "Hello, world!" ) );
		m_textBoxSimpleIn3D.reset( new font::TextBox3D( new font::TextBox( m_fontManager->FindFont( "font1_3D" ), unify::V2< float >( 0, 0 ), "Hello, world!", 0.01f ) ) );
		

		/* TODO:
		m_fontManager->LoadFromFile( "font2", "media/font3D/font3D.xml" );
		boost::shared_ptr< font::Font > font2 = m_fontManager->FindFont( "font2" );
		m_textBoxSimpleWith3DCharacters.reset( 
			new font::TextBox( font2, unify::V2< float >( 30, 60 ), "ABCDEF" )
			);
		m_textBoxSimpleWidth3DCharactersIn3D.reset( new font::TextBox3D(
			new font::TextBox( font2, unify::V2< float >( 0, 40 ), "ABCDEF", 0.01f )
			) );
		*/
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eyePosition( 0, 5, -10 );
		unify::Matrix rotation = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), m_rotation );
		rotation.TransformCoord( eyePosition );
		m_view.SetPosition( eyePosition );
		m_view.LookAtLH( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
		m_rotation += unify::Angle::AngleInRadians( elapsed );
		m_rotation.Fix360();

		if ( m_textBoxSimple ) m_textBoxSimple->Update( elapsed );
		if ( m_textBoxSimpleWith3DCharacters ) m_textBoxSimpleWith3DCharacters->Update( elapsed );
		if ( m_textBoxSimpleIn3D ) m_textBoxSimpleIn3D->Update( elapsed, 0 );
		if ( m_textBoxSimpleWidth3DCharactersIn3D ) m_textBoxSimpleWidth3DCharactersIn3D->Update( elapsed, 0 );

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		RenderInfo renderInfo3D;
		renderInfo3D.SetViewMatrix( m_view.Inverse() );
		renderInfo3D.SetProjectionMatrix( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
		Effect::shared_ptr effect = GetManager< Effect >()->Find( "color_3d" );
		effect->Use( renderInfo3D );
		m_triangleVB.Use();
		m_triangleVB.GetVertexDeclaration().Use();
		win::DX::GetDxDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		RenderInfo renderInfo2D;
		unify::Matrix projectionMatrix = unify::Matrix::MatrixOrthoOffCenterLH( 0, static_cast< float >( GetOS().GetResolution().width ), static_cast< float >( GetOS().GetResolution().height ), 0, 1, 1000 );
		renderInfo2D.SetProjectionMatrix( projectionMatrix );

		if ( m_textBoxSimple ) 	m_textBoxSimple->Render( renderInfo2D, false );
		if ( m_textBoxSimpleIn3D ) m_textBoxSimpleIn3D->Render( renderInfo3D, 0 );
		if ( m_textBoxSimpleWidth3DCharactersIn3D )m_textBoxSimpleWidth3DCharactersIn3D->Render( renderInfo3D, 0 );
		if ( m_textBoxSimpleWith3DCharacters ) m_textBoxSimpleWith3DCharacters->Render( renderInfo3D );

        // Background
		/*
        struct BackgroundVertices {
            float x, y, z;
            unsigned int color;
        } backgroundVertices[] =
        {
            { -2,  2, 2, 0xFF333333 },
            {  2,  2, 2, 0xFF666666 },
            { -2, -2, 2, 0xFF666666 },
            {  2, -2, 2, 0xFF999999 }
        };
        win::DX::GetDxDevice()->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
        win::DX::GetDxDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, backgroundVertices, sizeof( BackgroundVertices ) );
		*/
	}

	void Shutdown()
	{
		Game::Shutdown();
	}
} game;

