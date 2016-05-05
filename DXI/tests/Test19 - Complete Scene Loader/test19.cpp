// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/scene/SceneManager.h>
#include <bi/Scene.h>
#include <bi/XMLConvert.h>
#include <dxi/GeometryGroup.h>
#include <dxi/shapes/ShapeCreators.h>

using namespace dxi;
using namespace core;


class BISceneLF : public loader::SceneLoader::LoaderFunctor
{
public:
    void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
};

class MyGame : public Win32Game
{
protected:
	boost::shared_ptr< scene::SceneManager > m_sceneManager;

public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;


void MyGame::Startup()
{
	Win32Game::Startup();

	// Create managers...
	TextureManager::shared_ptr textureManager( new TextureManager() );
	PixelShaderManager::shared_ptr pixelShaderManager( new PixelShaderManager() );
	VertexShaderManager::shared_ptr vertexShaderManager( new VertexShaderManager() );
	GeometryManager::shared_ptr geometryManager( new GeometryManager( textureManager, pixelShaderManager, vertexShaderManager ) );

	m_sceneManager.reset( new scene::SceneManager );
    m_sceneManager->GetSceneLoader().AddLoaderFunctor( "biscene", loader::SceneLoader::LoaderFunctor::shared_ptr( new BISceneLF ) );
	m_sceneManager->SetGeometryManager( geometryManager );
	m_sceneManager->LoadScene( "main", "media/scene1.xml" );


    // HUD
    struct CompassUpdater : public events::Listener
    {
        void operator()( boost::any & eventData )
        {
			scene::Object::EventData::OnUpdate & actualData = boost::any_cast< scene::Object::EventData::OnUpdate & >( eventData );
        }
    };

    scene::Scene * HUDScene = new scene::Scene;
    HUDScene->SetGeometryManager( geometryManager );
    m_sceneManager->AddScene( "HUD", HUDScene );
    scene::Object::shared_ptr compassObject( new scene::Object );
    GeometryGroup * compassGroup = new GeometryGroup;
    compassObject->SetGeometry( Geometry::shared_ptr( compassGroup ) );
    compassObject->GetListenerMap().Add( "OnUpdate", events::Listener::shared_ptr( new CompassUpdater ) );
    HUDScene->Add( "compass", compassObject );
    HUDScene->SetSize( unify::Size< float >( 200, 200 ) );
    m_sceneManager->AddScene( "hud", HUDScene );

    shapes::CubeParameters cubeParameters;
    cubeParameters.SetSize( unify::Size3< float >( 1, 2, 2 ) );
	compassGroup->Add( Geometry::shared_ptr( CreateShape( cubeParameters ) ) );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	m_sceneManager->Update( elapsed, input );
	return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
	m_sceneManager->Render();
}

void MyGame::Shutdown()
{
	m_sceneManager.reset();
	Win32Game::Shutdown();
}


void BISceneLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	scene->SetPhysicsScene( boost::shared_ptr< bi::Scene >( new bi::Scene ) );
	const qxml::Element * subNode = element->GetFirstChild();
	while( subNode )
	{
		if( subNode->GetTagName() == "shape" )
		{
			std::string name = subNode->GetStringAttribute( "name" );

			bi::Scene * biScene = dynamic_cast< bi::Scene * >( scene->GetPhysicsScene() );
			if( ! biScene )
			{
				throw std::exception( "Attempted to add BI Shape with no BI Scene!" );
			}

			boost::shared_ptr< btCollisionShape > shape;
			if( ! bi::XMLConvertShape( subNode, shape ) )
			{
				throw std::exception( "Unable to process physic's shape from XML element!" );
			}

			biScene->AddShape( name, shape );
		}
		subNode = subNode->GetNext();
	}
}
