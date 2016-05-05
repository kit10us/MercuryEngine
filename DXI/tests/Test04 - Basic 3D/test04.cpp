// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "Basic 3D"
/// goals and requirements:
/// * Illustrate small 3D implementation to prove 3D pipeline works.
/// * Limit the use of unnecessary complexity which migh hinder this test by making defect isolation more difficult.
/// </summary>

#include <dxi/core/Game.h>
#include <dxi/RenderState.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/VertexShaderFactory.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	unify::Matrix m_projection;
	unify::Matrix m_view;
	unify::Angle m_rotation;

	VertexBuffer m_triangleVB;

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

		// Color effect
		qjson::Object colorVDJson;
		colorVDJson.Add( { "Position", "Float3" } );
		colorVDJson.Add( { "Diffuse", "D3DCOLOR" } );
		GetManager< PixelShader >()->Add( MakePixelShaderJson( "color_3d", "media/shaders/color3d.shader", "ps_main", "ps_1_1" ) );
		GetManager< VertexShader >()->Add( MakeVertexShaderJson( "color_3d", "media/shaders/color3d.shader", "vs_main", "vs_1_1", colorVDJson ) );
		Effect::shared_ptr effect = GetManager< Effect >()->Add( "color_3d", new Effect );
		effect->SetPixelShader( GetManager< PixelShader >()->Find( "color_3d" ) );
		effect->SetVertexShader( GetManager< VertexShader >()->Find( "color_3d" ) );

		// Default projection matrix...
		m_projection = unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS()->GetResolution().AspectRatioHW(), 1, 1000 );

		// Create view matrix for use as camera....
		m_view.BecomeIdentity();
		m_view.Translate( unify::V3< float >( 0, 0, 10 ) );
		m_view.LookAtLH( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

		RenderState::Set( RenderState::CullMode, RenderState::CullModeValues::None );

		// Create a demonstration of 3d via a 3d rotating triangle...
		m_triangleVB.Create( 6, VertexDeclaration( colorVDJson ) );
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
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eyePosition( 0, 0, -7 );
		unify::Matrix rotation = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), m_rotation );
		rotation.TransformCoord( eyePosition );
		m_view.SetPosition( eyePosition );
		m_view.LookAtLH( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
		m_rotation += unify::Angle::AngleInRadians( elapsed );
		m_rotation.Fix360();
	
		return Game::Update( elapsed, input );
	}

	void Render()
	{
		RenderInfo renderInfo;
		renderInfo.SetFinalMatrix( m_view.Inverse() * m_projection );
		Effect::shared_ptr effect = GetManager< Effect >()->Find( "color_3d" );
		effect->Use( renderInfo );
		m_triangleVB.Use();
		m_triangleVB.GetVertexDeclaration().Use();
		win::DX::GetDxDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	}

	void Shutdown()
	{
		Game::Shutdown();
	}
} game;

