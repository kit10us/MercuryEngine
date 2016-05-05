// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/GeometryGroup.h>
#include <dxi/Terra.h>
#include <unify/Size3.h>
#include <PxPhysicsAPI.h>
#include <PxExtensionsAPI.h>
#include <PxDefaultAllocator.h>
#include <PxDefaultErrorCallback.h>
#include <PxDefaultSimulationFilterShader.h>

// LIBs
#pragma comment( lib, "PhysX3_x86.lib" )
#pragma comment( lib, "PxTask.lib" )
#pragma comment( lib, "Foundation.lib" )
#pragma comment( lib, "LowLevel.lib" )
#pragma comment( lib, "PhysX3Common.lib" )
#pragma comment( lib, "PhysX3Extensions.lib" )
#pragma comment( lib, "PhysX3Cooking_x86.lib" )


using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
protected:
	TextureManager::shared_ptr m_textureManager;
	GeometryManager::shared_ptr m_geometryManager;
	scene::Scene m_scene1;
	scene::Object::shared_ptr m_camera1;
	float m_rotation;
    PxPhysics * m_physics;
    PxDefaultAllocator m_allocator;
    PxDefaultErrorCallback m_errorCallback;
    PxCooking * m_cooking;
    physx::pxtask::CpuDispatcher * m_cpuDispatcher;
    physx::pxtask::GpuDispatcher * m_cudaContextManager;
    PxSimulationFilterShader m_defaultFilterShader;
    PxScene * m_scenePhysics;

public:
    MyGame();
    void Startup();
    bool Update( unify::Seconds elapsed, IInput & input );
    void Render();
    void Shutdown();
} game;

MyGame::MyGame()
: m_physics( 0 )
, m_cooking( 0 )
, m_cpuDispatcher( 0 )
, m_cudaContextManager( 0 )
, m_scenePhysics( 0 )
, m_defaultFilterShader( PxDefaultSimulationFilterShader )
{
}

void MyGame::Startup()
{
	Win32Game::Startup();

    // PhysX general...

    m_physics = PxCreatePhysics( PX_PHYSICS_VERSION, m_allocator, m_errorCallback, PxTolerancesScale(), true );
    if ( ! m_physics )
    {
        throw std::exception( "PxCreatePhysics failed!" );
    }

    if ( ! PxInitExtensions( *m_physics ) )
    {
        throw std::exception( "PxInitExtensions failed!" );
    }

    m_cooking = PxCreateCooking( PX_PHYSICS_VERSION, &m_physics->getFoundation(), PxCookingParams() );
    if ( ! m_cooking )
    {
        throw std::exception( "PxCreateCooking failed!" );
    }

    // PhysX scene...

    PxSceneDesc sceneDesc( m_physics->getTolerancesScale() );
    sceneDesc.gravity = PxVec3( 0.0f, -9.81f, 0.0f );
    //customizeSceneDesc( sceneDesc );
    if ( ! sceneDesc.cpuDispatcher )
    {
        m_cpuDispatcher = PxDefaultCpuDispatcherCreate( 1 );
        if( ! m_cpuDispatcher )
        {
            throw std::exception( "PxDefaultCpuDispatcherCreate failed!" );
        }
        sceneDesc.cpuDispatcher = m_cpuDispatcher;
    }
    if ( ! sceneDesc.filterShader )
    {
        sceneDesc.filterShader  = m_defaultFilterShader;
    }
/*
    #ifdef PX_WINDOWS
    if ( ! sceneDesc.gpuDispatcher && m_cudaContextManager )
    {
            sceneDesc.gpuDispatcher = m_cudaContextManager->getGpuDispatcher();
    }
    #endif
*/    
    m_scenePhysics = m_physics->createScene( sceneDesc );
    if ( ! m_scenePhysics )
    {
        throw std::exception( "createScene failed!" );
    }

	// Create managers...
	m_textureManager.reset( new TextureManager() );
	m_geometryManager.reset( new GeometryManager( m_textureManager ) );

	m_textureManager->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
	m_textureManager->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	m_textureManager->Add( "water_cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	// Scene 1...
    shapes::CubeParameters cubeParameters;
	cubeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
    cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	cubeParameters.SetCenter( unify::V3< float >( -4.5, 3, 0 ) );
    m_scene1.Add( "cube", scene::MakeObject( new Mesh( cubeParameters ) ) );

	shapes::PointFieldParameters pointFieldParameters; 
	pointFieldParameters.SetMinorRadius( 0.5f );
	pointFieldParameters.SetMajorRadius( 1.0f );
	pointFieldParameters.SetCount( 1000 );
	pointFieldParameters.SetCenter( unify::V3< float >( -1.5, 3, 0 ) );
	pointFieldParameters.SetDiffuse( unify::Color::ColorWhite() );
    m_scene1.Add( "PointField", scene::MakeObject( new Mesh( pointFieldParameters ) ) );
			
	shapes::PointRingParameters pointRingParameters; 
	pointRingParameters.SetMinorRadius( 0.25f );
	pointRingParameters.SetMajorRadius( 0.75f );
	pointRingParameters.SetCount( 1000 );
	pointRingParameters.SetCenter( unify::V3< float >( 1.5, 3, 0 ) );
	pointRingParameters.SetDiffuse( unify::Color::ColorRed() );
	m_scene1.Add( "PointRing", scene::MakeObject( new Mesh( pointRingParameters ) ) );

	shapes::DashRingParameters dashRingParameters; 
	dashRingParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	dashRingParameters.SetMinorRadius( 0.625f );
	dashRingParameters.SetMajorRadius( 1.0f );
	dashRingParameters.SetSize( 0.5f );
	dashRingParameters.SetCount( 5 );
	dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
	dashRingParameters.SetCenter( unify::V3< float >( 4.5, 3, 0 ) );
	m_scene1.Add( "DashRing", scene::MakeObject( new Mesh( dashRingParameters ) ) );

	shapes::PyramidParameters pyramidParameters; 
	pyramidParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
	pyramidParameters.SetCenter( unify::V3< float >( -4.5, 0, 0 ) );
	m_scene1.Add( "Pyramid", scene::MakeObject( new Mesh( pyramidParameters ) ) );

	shapes::CircleParameters circleParameters; 
	circleParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	circleParameters.SetSegments( 24 );
	circleParameters.SetRadius( 1.0f );
	circleParameters.SetDiffuse( unify::Color::ColorBlue() );
	circleParameters.SetCenter( unify::V3< float >( -1.5, 0, 0 ) );
	m_scene1.Add( "Circle", scene::MakeObject( new Mesh( circleParameters ) ) );

	shapes::SphereParameters sphereParameters; 
	sphereParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	sphereParameters.SetSegments( 24 );
	sphereParameters.SetRadius( 1.0f );
	sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
	sphereParameters.SetCenter( unify::V3< float >( 1.5, 0, 0 ) );
	m_scene1.Add( "Sphere", scene::MakeObject( new Mesh( sphereParameters ) ) );

	shapes::CylinderParameters cylinderParameters; 
	cylinderParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	cylinderParameters.SetSegments( 24 );
	cylinderParameters.SetRadius( 1.0f );
	cylinderParameters.SetHeight( 2.0f );
	cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
	cylinderParameters.SetCenter( unify::V3< float >( 4.5, 0, 0 ) );
	m_scene1.Add( "Cylinder", scene::MakeObject( new Mesh( cylinderParameters ) ) );

	shapes::TubeParameters tubeParameters; 
	tubeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	tubeParameters.SetSegments( 24 );
	tubeParameters.SetMajorRadius( 1.0f );
	tubeParameters.SetMinorRadius( 0.5f );
	tubeParameters.SetHeight( 2.0f );
	tubeParameters.SetDiffuse( unify::Color::ColorRed() );
	tubeParameters.SetCenter( unify::V3< float >( -4.5, -3, 0 ) );
	m_scene1.Add( "Tube", scene::MakeObject( new Mesh( tubeParameters ) ) );

	shapes::PlaneParameters planeParameters; 
	planeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	planeParameters.SetSegments( 2 );
	planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
	planeParameters.SetDiffuse( unify::Color::ColorCyan() );
	planeParameters.SetCenter( unify::V3< float >( -1.5, -3, 0 ) );
	m_scene1.Add( "Plane", scene::MakeObject( new Mesh( planeParameters ) ) );

	shapes::ConeParameters coneParameters; 
	coneParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
	coneParameters.SetSegments( 24 );
	coneParameters.SetRadius( 1.0f );
	coneParameters.SetHeight( 2.0f );
	coneParameters.SetDiffuse( unify::Color::ColorGreen() );
	coneParameters.SetCenter( unify::V3< float >( 1.5, -3, 0 ) );
	m_scene1.Add( "Cone", scene::MakeObject( new Mesh( coneParameters ) ) );

	m_camera1.reset( new scene::Object );
	m_scene1.Add( "camera", m_camera1 );
	m_scene1.SetCamera( "camera" );
	m_scene1.SetSize( GetResolution() );

	GeometryGroup * geometryGroup = new GeometryGroup();
	Terra * terra;

    Terra::Parameters terraParameters;
    terraParameters.SetSize( unify::Size< float >( 100, 70 ) );
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
    terraParameters.SetSize( unify::Size< float >( 100, 70 ) );
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

    scene::Object::shared_ptr terraObject = scene::MakeObject( geometryGroup );
    terraObject->GetFrame().MoveBy( unify::V3< float >( 0, -10, -0 ) );

	m_scene1.Add( "terrain", terraObject );

	m_rotation = 0;
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	unify::V3< float > eye( 10.0f * sin( m_rotation ), 10.0f * sin( m_rotation ), 10.0f * cos( m_rotation ) );
	m_camera1->GetFrame().SetPosition( eye );
	m_camera1->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

	m_rotation += 0.01f;
	if ( m_rotation > 3.1415936535f * 2 )
	{
		m_rotation -= 3.1415936545f * 2;
	}

	return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
	m_scene1.Render();
}

void MyGame::Shutdown()
{
    if ( m_cooking )
    {
        m_cooking->release();
        m_cooking = 0;
    }
    if ( m_physics )
    {
        m_physics->release();
        m_physics = 0;
    }

	m_scene1.Clear();
	m_geometryManager.reset();
	m_textureManager.reset();
	Win32Game::Shutdown();
}

