// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/ShapeFactory.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:

	void Startup()
	{
		// Call base startup.
		Game::Startup();

		// Add a texture.
		GetManager< Texture >()->Add( "borgcube", "media/borgcube.bmp" );

		// Create a vertex declaration from JSON.
		qjson::Object vertexJson = { {"Position", "Float3" }, { "Diffuse", "Color" }, { "Tex0", "Float2" } };
		VertexDeclaration vd( vertexJson );

		// Load shaders.
		PixelShader::shared_ptr ps = GetManager< PixelShader >()->Add( MakePixelShaderJson( "textured_2d",  "media/shaders/textured2d.shader", "ps_main", "ps_1_1" ) );
		VertexShader::shared_ptr vs = GetManager< VertexShader >()->Add( MakeVertexShaderJson( "textured_2d", "media/shaders/textured2d.shader", "vs_main", "vs_1_1", vertexJson ) );

		// Create an effect.
		Effect::shared_ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
		borgCubeEffect->SetVertexShader( vs );
		borgCubeEffect->SetPixelShader( ps );
		borgCubeEffect->SetTexture( 0, GetManager< Texture >()->Find( "borgcube" ) );
		
        // Create geometry.
		{
			shapes::ShapeBaseParameters commonParameters;
			commonParameters.SetEffect( GetManager< Effect >()->Find( "borgcube" ) );

			shapes::CubeParameters cubeParameters;
			cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
			GetManager< Geometry >()->Add( "cube", shapes::CreateShape( commonParameters + cubeParameters ) );

			shapes::PointFieldParameters pointFieldParameters;
			pointFieldParameters.SetMinorRadius( .5f );
			pointFieldParameters.SetMajorRadius( 1.0f );
			pointFieldParameters.SetCount( 1000 );
			GetManager< Geometry >()->Add( "pointfield", shapes::CreateShape( commonParameters + pointFieldParameters ) );

			shapes::PointRingParameters pointRingParameters;
			pointRingParameters.SetMinorRadius( .25f );
			pointRingParameters.SetMajorRadius( .75f );
			pointRingParameters.SetCount( 1000 );
			GetManager< Geometry >()->Add( "pointring", shapes::CreateShape( commonParameters + pointRingParameters ) );

			shapes::DashRingParameters dashRingParameters;
			dashRingParameters.SetMinorRadius( 0.625f );
			dashRingParameters.SetMajorRadius( 1 );
			dashRingParameters.SetCount( 5 );
			dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
			GetManager< Geometry >()->Add( "dashring", shapes::CreateShape( commonParameters + dashRingParameters ) );

			shapes::PyramidParameters pyramidParameters;
			pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
			GetManager< Geometry >()->Add( "pyramid", shapes::CreateShape( commonParameters + pyramidParameters ) );

			shapes::CircleParameters circleParameters;
			circleParameters.SetSegments( 24 );
			circleParameters.SetRadius( 1 );
			circleParameters.SetDiffuse( unify::Color::ColorRed() );
			GetManager< Geometry >()->Add( "circle", shapes::CreateShape( commonParameters + circleParameters ) );

			shapes::SphereParameters sphereParameters;
			sphereParameters.SetSegments( 24 );
			sphereParameters.SetRadius( 1 );
			sphereParameters.SetDiffuse( unify::Color::ColorRed() );
			GetManager< Geometry >()->Add( "sphere", shapes::CreateShape( commonParameters + sphereParameters ) );

			shapes::CylinderParameters cylinderParameters;
			cylinderParameters.SetSegments( 24 );
			cylinderParameters.SetRadius( 1 );
			cylinderParameters.SetHeight( 2 );
			cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
			GetManager< Geometry >()->Add( "cylinder", shapes::CreateShape( commonParameters + cylinderParameters ) );

			shapes::TubeParameters tubeParameters;
			tubeParameters.SetSegments( 24 );
			tubeParameters.SetMajorRadius( 1 );
			tubeParameters.SetMinorRadius( 0.5f );
			tubeParameters.SetHeight( 2 );
			tubeParameters.SetDiffuse( unify::Color::ColorBlue() );
			GetManager< Geometry >()->Add( "tube", shapes::CreateShape( commonParameters + tubeParameters ) );

			shapes::PlaneParameters planeParameters;
			planeParameters.SetSegments( 2 );
			planeParameters.SetSize( unify::Size< float >( 2, 2 ) );
			planeParameters.SetDiffuse( unify::Color::ColorCyan() );
			GetManager< Geometry >()->Add( "plane", shapes::CreateShape( commonParameters + planeParameters ) );

			shapes::ConeParameters coneParameters;
			coneParameters.SetSegments( 24 );
			coneParameters.SetRadius( 1 );
			coneParameters.SetHeight( 2 );
			coneParameters.SetDiffuse( unify::Color::ColorRed() );
			GetManager< Geometry >()->Add( "cone", shapes::CreateShape( commonParameters + coneParameters ) );
		}

		// Add a scene.
		{
			scene::Scene::shared_ptr scene = GetSceneManager()->Add( "scene", new scene::Scene );

			scene->Add( "cube", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "cube" ), unify::V3< float >( -4.5f, 3, 0 ) ) ) );
			scene->Add( "pointfield", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "pointfield" ), unify::V3< float >( -1.5f, 3, 0 ) ) ) );
			scene->Add( "pointring", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "pointring" ), unify::V3< float >( 1.5f, 3, 0 ) ) ) );
			scene->Add( "dashring", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "dashring" ), unify::V3< float >( 4.5f, 3, 0 ) ) ) );
			scene->Add( "pyramid", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "pyramid" ), unify::V3< float >( -4.5f, 0, 0 ) ) ) );
			scene->Add( "circle", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "circle" ), unify::V3< float >( -1.5f, 0, 0 ) ) ) );
			scene->Add( "sphere", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "sphere" ), unify::V3< float >( 1.5f, 0, 0 ) ) ) );
			scene->Add( "cylinder", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "cylinder" ), unify::V3< float >( 4.5f, 0, 0 ) ) ) );
			scene->Add( "tube", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "tube" ), unify::V3< float >( -4.5f, -3, 0 ) ) ) );
			scene->Add( "plane", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "plane" ), unify::V3< float >( -1.5f, -3, 0 ) ) ) );
			scene->Add( "cone", scene::Object::shared_ptr( new scene::Object( GetManager< Geometry >()->Find( "cone" ), unify::V3< float >( 1.5f, -3, 0 ) ) ) );

			scene::Object::shared_ptr cameraObject( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
			scene->Add( "camera", cameraObject );
			scene::Camera::shared_ptr camera( new scene::Camera( cameraObject ) );
			scene->SetCamera( "camera" );
			scene->GetCamera().SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS()->GetResolution().AspectRatioHW(), 1, 1000 ) );
			camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
			camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
		}
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		// Use of camera controls to simplify camera movement...
		scene::Object::shared_ptr cameraObject = GetSceneManager()->Find( "scene" )->GetCamera().GetObject();
		cameraObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) );
		cameraObject->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		Game::Render();
	}

	void Shutdown()
	{		
		Game::Shutdown();
	}
} game;

