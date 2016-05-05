// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/Mesh.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	scene::Object::shared_ptr m_camera;
	float m_rotation;

public:

	void Startup()
	{
		// Call base startup.
		Game::Startup();

		// Add a texture.
		GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );

		// Create a vertex declaration from JSON.
		qjson::Object vertexJson;
		vertexJson.Add( { "Position", "Float3" } );
		vertexJson.Add( { "Diffuse", "Color" } );
		vertexJson.Add( { "Tex1", "Float2" } );
		VertexDeclaration vd( vertexJson );

		// Load shaders.
		PixelShader::shared_ptr ps = GetPixelShaderManager()->LoadFromFile( "textured_2d", "media/shaders/textured2d.shader", "ps_main", "ps_1_1" );
		VertexShader::shared_ptr vs = GetVertexShaderManager()->LoadFromFile( "textured_2d", "media/shaders/textured2d.shader", "vs_main", "vs_1_1", vd );

		// Create an effect.
		Effect::shared_ptr borgCubeEffect = GetEffectManager()->Add( "borgcube", new Effect );
		borgCubeEffect->SetVertexShader( vs );
		borgCubeEffect->SetPixelShader( ps );
		borgCubeEffect->SetTexture( 0, Find< Texture >( "borgcube" ) );
		
        // Create geometry.
		{
			shapes::ShapeBaseParameters commonParameters;
			commonParameters.SetEffect( Find< Effect >( "borgcube" ) );

			shapes::CubeParameters cubeParameters;
			cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
			GetGeometryManager()->Add( "cube", new geo::Mesh( commonParameters + cubeParameters ) );

			shapes::PointFieldParameters pointFieldParameters;
			pointFieldParameters.SetMinorRadius( .5f );
			pointFieldParameters.SetMajorRadius( 1.0f );
			pointFieldParameters.SetCount( 1000 );
			GetGeometryManager()->Add( "pointfield", new geo::Mesh( commonParameters + pointFieldParameters ) );

			shapes::PointRingParameters pointRingParameters;
			pointRingParameters.SetMinorRadius( .25f );
			pointRingParameters.SetMajorRadius( .75f );
			pointRingParameters.SetCount( 1000 );
			GetGeometryManager()->Add( "pointring", new geo::Mesh( commonParameters + pointRingParameters ) );

			shapes::DashRingParameters dashRingParameters;
			dashRingParameters.SetMinorRadius( 0.625f );
			dashRingParameters.SetMajorRadius( 1 );
			dashRingParameters.SetCount( 5 );
			dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
			GetGeometryManager()->Add( "dashring", new geo::Mesh( commonParameters + dashRingParameters ) );

			shapes::PyramidParameters pyramidParameters;
			pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
			GetGeometryManager()->Add( "pyramid", new geo::Mesh( commonParameters + pyramidParameters ) );

			shapes::CircleParameters circleParameters;
			circleParameters.SetSegments( 24 );
			circleParameters.SetRadius( 1 );
			circleParameters.SetDiffuse( unify::Color::ColorRed() );
			GetGeometryManager()->Add( "circle", new geo::Mesh( commonParameters + circleParameters ) );

			shapes::SphereParameters sphereParameters;
			sphereParameters.SetSegments( 24 );
			sphereParameters.SetRadius( 1 );
			sphereParameters.SetDiffuse( unify::Color::ColorRed() );
			GetGeometryManager()->Add( "sphere", new geo::Mesh( commonParameters + sphereParameters ) );

			shapes::CylinderParameters cylinderParameters;
			cylinderParameters.SetSegments( 24 );
			cylinderParameters.SetRadius( 1 );
			cylinderParameters.SetHeight( 2 );
			cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
			GetGeometryManager()->Add( "cylinder", new geo::Mesh( commonParameters + cylinderParameters ) );

			shapes::TubeParameters tubeParameters;
			tubeParameters.SetSegments( 24 );
			tubeParameters.SetMajorRadius( 1 );
			tubeParameters.SetMinorRadius( 0.5f );
			tubeParameters.SetHeight( 2 );
			tubeParameters.SetDiffuse( unify::Color::ColorBlue() );
			GetGeometryManager()->Add( "tube", new geo::Mesh( commonParameters + tubeParameters ) );

			shapes::PlaneParameters planeParameters;
			planeParameters.SetSegments( 2 );
			planeParameters.SetSize( unify::Size< float >( 2, 2 ) );
			planeParameters.SetDiffuse( unify::Color::ColorCyan() );
			GetGeometryManager()->Add( "plane", new geo::Mesh( commonParameters + planeParameters ) );

			shapes::ConeParameters coneParameters;
			coneParameters.SetSegments( 24 );
			coneParameters.SetRadius( 1 );
			coneParameters.SetHeight( 2 );
			coneParameters.SetDiffuse( unify::Color::ColorRed() );
			GetGeometryManager()->Add( "cone", new geo::Mesh( commonParameters + coneParameters ) );
		}

		// Add a scene.
		{
			scene::Scene::shared_ptr scene = GetSceneManager()->Add( "scene", new scene::Scene() );

			scene->Add( "cube", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "cube" ), unify::V3< float >( -4.5f, 3, 0 ) ) ) );
			scene->Add( "pointfield", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "pointfield" ), unify::V3< float >( -1.5f, 3, 0 ) ) ) );
			scene->Add( "pointring", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "pointring" ), unify::V3< float >( 1.5f, 3, 0 ) ) ) );
			scene->Add( "dashring", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "dashring" ), unify::V3< float >( 4.5f, 3, 0 ) ) ) );
			scene->Add( "pyramid", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "pyramid" ), unify::V3< float >( -4.5f, 0, 0 ) ) ) );
			scene->Add( "circle", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "circle" ), unify::V3< float >( -1.5f, 0, 0 ) ) ) );
			scene->Add( "sphere", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "sphere" ), unify::V3< float >( 1.5f, 0, 0 ) ) ) );
			scene->Add( "cylinder", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "cylinder" ), unify::V3< float >( 4.5f, 0, 0 ) ) ) );
			scene->Add( "tube", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "tube" ), unify::V3< float >( -4.5f, -3, 0 ) ) ) );
			scene->Add( "plane", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "plane" ), unify::V3< float >( -1.5f, -3, 0 ) ) ) );
			scene->Add( "cone", scene::Object::shared_ptr( new scene::Object( Find< geo::Geometry >( "cone" ), unify::V3< float >( 1.5f, -3, 0 ) ) ) );

			m_camera.reset( new scene::Object );
			scene->Add( "camera", m_camera );
			scene->SetCamera( "camera" );

			m_rotation = 0;
		}
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eye( 10.0f * sin( m_rotation ), 10.0f * sin( m_rotation ), 10.0f * cos( m_rotation ) );
		m_camera->GetFrame().SetPosition( eye );
		m_camera->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

		m_rotation += 0.01f;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

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

