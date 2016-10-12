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
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/win/DXILib.h>
#include <dxi/win/DXDevice.h>
#include <DXIWinMain.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	unify::Matrix m_view;
	unify::Angle m_rotation;

	VertexBuffer m_triangleVB;

public:
	bool Setup( core::IOS & os ) override
	{
		/*
		// TODO:
		os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
		os.SetFullscreen( false );
		*/
		return true;
	}

	void Startup() override
	{
		Effect::ptr effect = GetManager< Effect >()->Add( "color3d", "media/EffectColor.effect" );
		effect->SetPixelShader( GetManager< PixelShader >()->Find( "color3d" ) );
		effect->SetVertexShader( GetManager< VertexShader >()->Find( "color3d" ) );

		// Create view matrix for use as camera....
		m_view.BecomeIdentity();
		m_view.Translate( unify::V3< float >( 0, 0, 10 ) );
		m_view.LookAtLH( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

		GetOS().GetRenderer( 0 )->SetCullMode( CullMode::None );

		VertexDeclaration::ptr vd( GetManager< VertexShader >()->Find( "color3d" )->GetVertexDeclaration() );

		// Create a demonstration of 3d via a 3d rotating triangle...
		struct MyVertex
		{
			float x, y, z;
			unsigned int color;
		};
		MyVertex aTriangle[] = {
			{ -2.0f, -4.0f, 0, 0xffff0000 },
			{ 0.0f, 4.0f, 0, 0xff0000ff },
			{ 2.0f, -4.0f, 0, 0xff00ff00 },

			{ 0, -4.0f, -2.0f, 0xffff0000 },
			{ 0, 4.0f, 0.0f, 0xff00ff00 },
			{ 0, -4.0f, 2.0f, 0xff0000ff }
		};
		m_triangleVB.Create( 6, vd, aTriangle );
	}

	bool Update( RenderInfo & renderInfo ) override
	{
		unify::V3< float > eyePosition( 0, 0, -7 );
		unify::Matrix rotation = unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ), m_rotation );
		rotation.TransformCoord( eyePosition );
		m_view.SetPosition( eyePosition );
		m_view.LookAtLH( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
		m_rotation += unify::Angle::AngleInRadians( renderInfo.GetDelta() );
		m_rotation.Fix360();

		renderInfo.SetWorldMatrix( unify::Matrix::MatrixIdentity() );
		renderInfo.SetViewMatrix( m_view );
		renderInfo.SetProjectionMatrix( unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, 
			// TODO: GetOS().GetResolution().AspectRatioWH()
			800.0f/600.0f
			, 1.0f, 1000.0f ) );

		return Game::Update( renderInfo );
	}

	void Render( int renderer, const RenderInfo & renderInfo, const Viewport & viewport ) override
	{
		Effect::ptr effect = GetManager< Effect >()->Find( "color3d" );
		m_triangleVB.Use();

		RenderMethod method = RenderMethod::CreateTriangleList( 0, 2, effect );
		method.Render( renderInfo );
	}

	void Shutdown()	override
	{
	}
} game;

RegisterGame( game );


