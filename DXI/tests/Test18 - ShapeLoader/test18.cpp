// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/Camera.h>
#include <dxi/geo/Mesh.h>
#include <dxi/scene/ObjectGroup.h>
#include <dxi/loader/ShapeLoader.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
    scene::ObjectGroup m_group;
	float m_rotation;
	scene::Object::shared_ptr m_camera;
	scene::Camera m_cameraController;

public:

	void Startup()
	{
		Game::Startup();

		// Create managers...

        m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeCube.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapePointField.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapePointRing.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeDashRing.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapePyramid.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeCircle.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeSphere.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeCylinder.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeTube.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapePlane.xml", GetTextureManager() ) ) ) ) );
		m_group.Add( scene::Object::shared_ptr( new scene::Object( Geometry::shared_ptr( loader::ShapeLoader( "media/ShapeCone.xml", GetTextureManager() ) ) ) ) );

        struct ArangeObjects : scene::ObjectGroup::IObjectFunctor
        {
            void operator()( scene::Object & object, size_t index, size_t count )
            {
                int rows = (int)ceil( sqrt( (float)count ) );
                int columns = (int)floor( sqrt( (float)count ) );
                int v = index / rows;
                int h = index % rows;
                float seperation = 3;
                unify::V3< float > origin( seperation * rows * -0.40f, seperation * columns * 0.40f, 0 );
                object.GetFrame().SetPosition( origin + unify::V3< float >( h * seperation, v * -seperation, 0 ) );
            }
        } functor;

        m_group.ForEach( functor );


		m_camera.reset( new scene::Object );
		m_cameraController.SetObject( m_camera );

		m_rotation = 0;
	}

	bool Update( RenderInfo & renderInfo, IInput & input )
	{
		unify::V3< float > eye( 20.0f * sin( m_rotation ), 20.0f * sin( m_rotation ), 20.0f * cos( m_rotation ) );
		m_camera->GetFrame().SetPosition( eye );
		m_camera->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
		m_cameraController.Use();

		m_rotation += 0.01f;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		return Game::Update( renderInfo, input );
	}

	void Render()
	{
		RenderInfo renderInfo;
		m_group.Render( renderInfo );
	}

	void Shutdown()
	{
		m_group.Clear();
		Game::Shutdown();
	}
} game;

