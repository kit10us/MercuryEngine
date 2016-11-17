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
#include <me/scene2d/Canvas.h>
#include <me/scene2d/FPS.h>

using namespace me;

class MyGame : public Game
{
protected:
public:
	MyGame() : Game( "setup_models.xml" ) {}
	void Startup();
	void Update( RenderInfo & renderInfo );
} game;

RegisterGame( game );

void MyGame::Startup()
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", "media/EffectColor.effect" );
	//Effect::ptr color3DEffect = GetManager< Effect >()->Add( "ColorInstanced3D", "media/EffectColorInstanced3D.effect" );

	Scene::ptr scene = sceneManager->Add( "scene" );

	// Add a camera...
	Object::ptr camera = scene->GetRoot()->AddChild( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent( GetOS() ) ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 25, -100 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

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

	size_t depth = 12;
	size_t columns = 12;
	size_t rows = 12;
	float spacing = 2.0f;
	for( size_t d = 0; d < depth; ++d )
	{
		for( size_t c = 0; c < columns; c++ )
		{
			for( size_t r = 0; r < rows; ++r )
			{			
				size_t shape = (columns + d + r) % 3;

				auto object = scene->GetRoot()->AddChild( "geo" );
				AddGeometryComponent( object.get(), GetOS(), (shape == 0 ) ? geo1 : (shape == 1) ? geo2 : geo3 );

				float x = (rows * spacing * -0.5f) + r * spacing;
				float y = (columns * spacing * -0.5f) + c * spacing;
				float z = (depth * spacing * -0.5f) + d * spacing;

				object->GetFrame().SetPosition( unify::V3< float >( x, y, z ) );
			}
		}
	}

	scene2d::Canvas::ptr canvas( new scene2d::Canvas( this ) );
	scene->AddComponent( canvas );

	Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "media/font2.effect" );	
	canvas->AddElement( scene2d::IElement::ptr( new scene2d::FPS( this, font2 ) ) );
}

void MyGame::Update( RenderInfo & renderInfo )
{
	using namespace scene;

	SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

	// Use of camera controls to simplify camera movement...
	Object::ptr camera = sceneManager->Find( "scene" )->GetRoot()->FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
