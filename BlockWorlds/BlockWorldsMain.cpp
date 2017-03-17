// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/Mesh.h>
#include <me/scene/SceneManager.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <MEWinMain.h>

#include <me/object/BBoxRendererComponent.h>
#include <me/object/CameraComponent.h>
#include <me/scene/SceneManager.h>
#include <me/object/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

#include <memory>

using namespace me;
using namespace object;

namespace bw
{
	class BlockDescription
	{
	public:
		typedef std::shared_ptr< BlockDescription > ptr;
		BlockDescription( std::string name, Geometry::ptr geometry );

		std::string GetName();

		Geometry::ptr GetGeometry();
		
	private:
		std::string m_name;
		Geometry::ptr m_geometry;
	};

	BlockDescription::BlockDescription( std::string name, Geometry::ptr geometry )
		: m_name( name )
		, m_geometry( geometry )
	{
	}

	std::string BlockDescription::GetName()
	{
		return m_name;
	}

	Geometry::ptr BlockDescription::GetGeometry()
	{
		return m_geometry;
	}


	class BlockLibrary
	{
	public:
		BlockLibrary()
			: m_library( 1000 )
		{
		}

		size_t Add( BlockDescription::ptr blockDescription )
		{
			m_library.push_back( blockDescription );
			return m_library.size();
		}

	private:
		std::vector< BlockDescription::ptr > m_library;
	};

	class Block
	{
	public:
		Block()
		{
		}

	private:
	};

	class WorldChunk
	{
	public:
		typedef std::shared_ptr< WorldChunk > ptr;

		WorldChunk( size_t size )
		{
		}

	private:
		size_t m_size;
	};

	class World
	{		  
	public:
		World( size_t chunkSize );

		BlockLibrary & GetBlockLibrary();

	private:
		BlockLibrary m_blockLibrary;
		size_t m_chunkSize;
		unify::V3< float > m_worldOffset;
		std::vector< WorldChunk::ptr > m_activeChunks;
		std::vector< WorldChunk::ptr > m_inactiveChunks; // To be stored.
	};

	World::World( size_t chunkSize )
		: m_chunkSize{ chunkSize }
		, m_worldOffset{ 0.0f, 0.0f, 0.0f }
	{
	}

	BlockLibrary & World::GetBlockLibrary()
	{
		return m_blockLibrary;
	}

}

using namespace bw;

class MyGame : public Game
{
protected:
	std::shared_ptr< World > m_world;

public:
	MyGame() : Game( "setup.xml" ) {}
	void Startup() override;
	void Update( UpdateParams params ) override;
} game;

RegisterGame( game );




void MyGame::Startup()
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager" ).get());
	Effect::ptr testEffect = GetManager< Effect >()->Add( "Test", "GrassBlock00.effect" );
		
	Scene::ptr scene = sceneManager->AddScene( "scene" );

	// Add a camera...
	Object * camera = scene->GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, GetOS()->GetRenderer(0)->GetDisplay().GetSize().AspectRatioWH(), 1.0f, 1000.0f ) );


	m_world.reset( new World( 100 ) );

	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( testEffect );
    cubeParameters.SetSize( unify::Size3< float >( 10, 10, 10 ) );
	cubeParameters.SetDiffuse( unify::Color::ColorBlue() );
	cubeParameters.SetFaceUV( 
	{ 
		// Top
		{ unify::TexCoords( 0.0f, 0.0f ), unify::TexCoords( 0.25f, 0.25f ) },

		// Bottom
		{ unify::TexCoords( 0.0f, 0.25f ), unify::TexCoords( 0.25f, 0.5f ) },

		// Front
		{ unify::TexCoords( 0.25f, 0.0f ), unify::TexCoords( 0.5f, 0.25f ) }, 

		// Back
		{ unify::TexCoords( 0.25f, 0.25f ), unify::TexCoords( 0.5f, 0.5f ) }, 

		// Left
		{ unify::TexCoords( 0.5f, 0.0f ), unify::TexCoords( 0.75f, 0.25f ) }, 
		
		// Right
		{ unify::TexCoords( 0.5f, 0.25f ), unify::TexCoords( 0.75f, 0.5f ) } 
	} 
	);
	Geometry::ptr geo1( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );
	m_world->GetBlockLibrary().Add( BlockDescription::ptr( new BlockDescription{ "grass", geo1 } ) );

	// Test...
	auto object = scene->GetObjectAllocator()->NewObject( "geo" );
	AddGeometryComponent( object, geo1 );

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	object->GetFrame().SetPosition( unify::V3< float >( x, y, z ) );

	// Camera is based on number of objects
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 10.0f, 0 - 10.0f * 2.0f ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
																 
	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( this ) );
	scene->AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( this, font2 ) ) );
}

void MyGame::Update( UpdateParams params )
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager" ).get());

	// Use of camera controls to simplify camera movement...
	Object * camera = sceneManager->FindScene( "scene" )->FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.renderInfo.GetDelta() ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
