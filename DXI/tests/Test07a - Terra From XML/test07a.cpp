// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/loader/TextureLoader.h>
#include <dxi/loader/EffectLoader.h>
#include <dxi/loader/TerraLoader.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	geo::GeometryGroup m_geometryGroup;
	float m_rotation;
	scene::Object::shared_ptr m_camera;
	scene::Camera m_cameraController;

public:

	void Startup()
	{
		Game::Startup();

		// TODO: What is the point of this test?

		// Attempting to load each piece individually into managers.
		// Typically, one would instead use the SceneLoader to let it load all parts automatically.
        qxml::Document document( "media/TerraEarth.xml" );

		/*
		// "land" texture.
		const qxml::Element * landTextureNode = document.GetRoot()->FindFirstElement( "texture", "name='land'" );
		assert( landTextureNode );
		loader::TextureLoader( landTextureNode, GetTextureManager() );
		*/

		VertexFormat defaultVertexFormat = FVF::XYZ | FVF::Normal | FVF::Tex1;
		/*
		// "cutout" texture.
		const qxml::Element * cutoutTextureNode = document.GetRoot()->FindFirstElement( "texture", "name='cutout'" );
		assert( cutoutTextureNode );
		loader::TextureLoader( cutoutTextureNode, GetTextureManager() );
		*/

		/*
		// "land" effect.
		const qxml::Element * landEffectNode = document.GetRoot()->FindFirstElement( "effect", "name='land'" );
		assert( landEffectNode );
		loader::EffectLoader( "land", landEffectNode, GetManagers() );
		*/

		/*
		// "cutout" effect.
		const qxml::Element * cutoutEffectNode = document.GetRoot()->FindFirstElement( "effect", "name='cutout'" );
		assert( cutoutEffectNode );
		loader::EffectLoader( "cutout", cutoutEffectNode, GetManagers() );
		*/

		// "earthland" terra.
        const qxml::Element * earthlandTerraNode = document.GetRoot()->FindFirstElement( "terra", "name='earthland'" );
        assert( earthlandTerraNode );
		geo::Terra::shared_ptr landTerra( loader::TerraLoader( earthlandTerraNode, GetTextureManager(), GetEffectManager() ) );
		m_geometryGroup.Add( geo::Geometry::shared_ptr( landTerra ) );

		// "earthwater" terra.
        const qxml::Element * earthwaterTerraNode = document.GetRoot()->FindFirstElement( "terra", "name='earthwater'" );
        assert( earthwaterTerraNode );
		geo::Terra::shared_ptr waterTerra( loader::TerraLoader( earthwaterTerraNode, GetTextureManager(), GetEffectManager() ) );
		m_geometryGroup.Add( geo::Geometry::shared_ptr( waterTerra ) );

		m_camera.reset( new scene::Object );
		m_cameraController.SetObject( m_camera );

		m_rotation = 0;
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eye( 10.0f * sin( m_rotation ), 7 + 5.0f * sin( m_rotation ), 10.0f * cos( m_rotation ) );
		m_camera->GetFrame().SetPosition( eye );
		m_camera->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
		// TODO: ?      m_cameraController.Use();

		m_rotation += 0.01f;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		RenderInfo renderInfo;
		m_geometryGroup.Render( renderInfo, 0 );
	}

	void Shutdown()
	{
		m_geometryGroup.Clear();
		Game::Shutdown();
	}
} game;

