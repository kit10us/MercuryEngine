// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/GeometryGroup.h>
#include <dxi/Terra.h>
#include <bi/Scene.h>
#include <boost/cast.hpp>
#include <dxi/Input.h>

using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
protected:
	TextureManager::shared_ptr m_textureManager;
	GeometryManager::shared_ptr m_geometryManager;
	scene::Scene m_scene1;
	boost::shared_ptr< bi::Scene > m_physicsScene;
	scene::Object::shared_ptr m_camera1;
	float m_rotation;

public:

	void Startup()
	{
		Win32Game::Startup();

		// Create managers...
		m_textureManager.reset( new TextureManager() );
		m_geometryManager.reset( new GeometryManager( m_textureManager ) );
		m_scene1.SetGeometryManager( m_geometryManager );

		m_textureManager->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
		m_textureManager->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
		m_textureManager->Add( "water_cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );
	
		// Physics
		m_physicsScene.reset( new bi::Scene );
		m_scene1.SetPhysicsScene( m_physicsScene );
		m_physicsScene->AddShape( "cube", boost::shared_ptr< btCollisionShape >( new btBoxShape( btVector3( 1, 1, 1 ) ) ) );
		m_physicsScene->AddShape( "sphere", boost::shared_ptr< btCollisionShape >( new btSphereShape( 1 ) ) ) ;

		scene::Object::shared_ptr object;
        unify::Parameters parameters;
		Geometry::shared_ptr geometryBox;
		Geometry::shared_ptr geometrySphere;

		// Graphics...
        shapes::CubeParameters cubeParameters;
        cubeParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
        cubeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
        cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
        cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		geometryBox = m_geometryManager->Add( "cube",  new Mesh( cubeParameters ) );
		
        shapes::SphereParameters sphereParameters;
        sphereParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
        sphereParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1.0f );
		sphereParameters.SetDiffuse( unify::Color::ColorRed() );
		geometrySphere = m_geometryManager->Add( "sphere", new Mesh( sphereParameters ) );

		bool box = true;
		int objectNumber = 0;
		for( unsigned int d = 0; d < 2; ++d )
		{
			for( unsigned int c = 0; c < 6; ++c )
			{
				for( unsigned int r = 0; r < 6; ++r )
				{
					std::string name = std::string( "object" ) + boost::lexical_cast< std::string >( objectNumber );
					scene::Object::shared_ptr object;
					object = m_scene1.CreateObject( name, box ? "cube" : "sphere", box ? "cube" : "sphere" );
					boost::shared_ptr< btCollisionShape > collisionShape;
					if( box )
					{
						collisionShape.reset( new btBoxShape( btVector3( 1, 1, 1 ) ) );
					}
					else
					{
						collisionShape.reset( new btSphereShape( 1 ) );
					}
					bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
					object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );
					m_physicsScene->Add( object->GetPhysics() );

					object->GetFrame().SetPosition( unify::V3< float >( -6.0f + r * 3, 13.0f + c * 3, -3.0f + d * 3 ) );
					object->GetPhysics()->SyncPhysicsFromObject();
					box = !box;
				}
				box = !box;
			}
			box = !box;
		}

		{
			GeometryGroup * geometryGroup = new GeometryGroup();
			Terra * terra;

            Terra::Parameters terraParameters;
            terraParameters.SetSize( unify::Size< float >( 20, 14 ) );
            terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
            terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
            terraParameters.SetConstant( 0 );
            terraParameters.SetTexArea( unify::TexArea::Full() );
			terra = new Terra( terraParameters );
			dxi::ColorOp depthOp;
			depthOp.r = 0.5f;
			depthOp.g = 0.25f;
			depthOp.b = -1.0f;
			terra->ApplyHeightMap( m_textureManager->Find( "earth" ), depthOp );
			terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, m_textureManager->Find( "earth" ) );
			terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::False ) );
			geometryGroup->Add( Geometry::shared_ptr( terra ) );

            terraParameters.Reset();
            terraParameters.SetSize( unify::Size< float >( 20, 14 ) );
            terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
            terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
            terraParameters.SetConstant( 0.1f );
            terraParameters.SetTexArea( unify::TexArea::Full() );
			terra = new Terra( terraParameters );
			dxi::ColorOp noDrawOp;
			noDrawOp.r = 1.0f;
			terra->ApplyAlphaMap( m_textureManager->Find( "water_cutout" ), noDrawOp );
			terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, m_textureManager->Find( "earth" ) );
			terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
			geometryGroup->Add( Geometry::shared_ptr( terra ) );
			object.reset( new scene::Object( Geometry::shared_ptr( geometryGroup ) ) );
			m_scene1.Add( "terrain", object );
		}

		m_camera1.reset( new scene::Object() );
		m_scene1.Add( "camera1", m_camera1 );
		m_scene1.SetCamera( "camera1" );
		m_scene1.SetSize( GetResolution() );

		m_rotation = 0;
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		if( m_camera1 )
		{
			unify::V3< float > eye( 30.0f * sin( m_rotation ), 30 + 10.0f * sin( m_rotation ), 30.0f * cos( m_rotation ) );
			m_camera1->GetFrame().SetPosition( eye );
			m_camera1->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
		}

		m_rotation += elapsed;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		m_scene1.Update( elapsed, input );

		return Win32Game::Update( elapsed, input );
	}

	void Render()
	{
		m_scene1.Render();
	}

	void Shutdown()
	{
		m_scene1.Clear();
		m_geometryManager.reset();
		m_textureManager.reset();
		Win32Game::Shutdown();
	}
} game;
