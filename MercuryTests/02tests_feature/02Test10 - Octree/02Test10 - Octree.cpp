// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/Mesh.h>
#include <me/scene/SceneManager.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <MEWinMain.h>

#include <me/scene/BBoxRendererComponent.h>
#include <me/scene/CameraComponent.h>

#include <me/scene/SceneManager.h>
#include <me/scene/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

using namespace me;

class MyGame : public Game
{
protected:
public:
	MyGame() : Game( "setup_models.xml" ) {}
	void Startup() override;
	void Update( UpdateParams params ) override;
} game;

RegisterGame( game );

void MyGame::Startup()
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "EffectColor.effect" );

	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced3D", "EffectColorInstanced3D_SC.effect" );
	
	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced3D", "EffectColorInstanced3D.effect" );

	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced_ambient", "ColorInstanced_ambient.effect" );

	Scene::ptr scene = sceneManager->AddScene( "scene" );

	// Add a camera...
	Object * camera = scene->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, GetOS()->GetRenderer(0)->GetDisplay().GetSize().AspectRatioWH(), 1.0f, 1000.0f ) );

	// Geo1
	sg::CubeParameters cubeParameters;
	cubeParameters.SetEffect( color3DEffect );
    cubeParameters.SetSize( unify::Size3< float >( 1, 1, 1 ) );
	cubeParameters.SetDiffuse( unify::Color::ColorBlue() );
	Geometry::ptr geo1( sg::CreateShape( GetOS()->GetRenderer(0), cubeParameters ) );

	// Geo2
	sg::SphereParameters sphereParameters;
	sphereParameters.SetEffect( color3DEffect );
	sphereParameters.SetRadius( 1 );
	sphereParameters.SetDiffuse( unify::Color::ColorRed() );
	Geometry::ptr geo2( sg::CreateShape( GetOS()->GetRenderer( 0 ), sphereParameters ) );

	// Geo3
	sg::ConeParameters coneParameters;
	coneParameters.SetEffect( color3DEffect );
	coneParameters.SetHeight( 1 );
	coneParameters.SetRadius( 1 );
	coneParameters.SetDiffuse( unify::Color::ColorGreen() );
	Geometry::ptr geo3( sg::CreateShape( GetOS()->GetRenderer( 0 ), coneParameters ) );

	size_t depth = 40;
	size_t columns = 40;
	size_t rows = 40;
	float spacing = 2.0f;
	for( size_t d = 0; d < depth; ++d )
	{
		for( size_t c = 0; c < columns; c++ )
		{
			for( size_t r = 0; r < rows; ++r )
			{			
				size_t shape = (columns + d + r) % 3;

				auto object = scene->NewObject( "geo" );
				AddGeometryComponent( object, (shape == 0 ) ? geo1 : (shape == 1) ? geo2 : geo3 );

				float x = (rows * spacing * -0.5f) + r * spacing;
				float y = (columns * spacing * -0.5f) + c * spacing;
				float z = (depth * spacing * -0.5f) + d * spacing;

				object->GetFrame().SetPosition( unify::V3< float >( x, y, z ) );
			}
		}
	}

	// Camera is based on number of objects
	float max = (float)std::max( std::max( depth, columns), rows );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, max * spacing * 0.5f, 0 - max * spacing * 2.0f ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );


	canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( this ) );
	scene->AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
	canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( this, font2 ) ) );
}

void MyGame::Update( UpdateParams params )
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	// Use of camera controls to simplify camera movement...
	Object * camera = sceneManager->FindScene( "scene" )->FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
