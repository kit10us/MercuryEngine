// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/scene/SceneManager.h>
#include <me/object/Object.h>
#include <me/Mesh.h>
#include <sg/ShapeCreators.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/BBoxRendererComponent.h>
#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/object/CameraComponent.h>

#include <sculpter/Shape.h>
#include <sculpter/Sheet.h>
#include <sculpter/spline/ISpline.h>

#include <algorithm>
#include <math.h>

using namespace me;
using namespace scene;
using namespace object;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
	// Add common effects...
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectColor.effect" ) );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "texture3d", unify::Path( "EffectTextured.effect" ) );

	// Load shaders.
	IPixelShader::ptr ps = GetManager< IPixelShader >()->Add( "texture3d", unify::Path( "shaders/textured3d.xml" ) );
	IVertexShader::ptr vs = GetManager< IVertexShader >()->Add( "texture3d", unify::Path( "shaders/textured3d.xml" ) );
																									  
	// Add a texture.
	GetManager< ITexture >()->Add( "borgcube", unify::Path( "borgcube.bmp" ) );

	// Create an effect.
	Effect::ptr borgCubeEffect = GetManager< Effect >()->Add( "borgcube", new Effect );
	borgCubeEffect->SetVertexShader( vs );
	borgCubeEffect->SetPixelShader( ps );
	borgCubeEffect->SetTexture( 0, GetManager< ITexture >()->Find( "borgcube" ) );

	//scene->AddComponent( scene::SceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), color3DEffect ) ) );

	// Add a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );	 
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "camera" ).get() );
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800/600, 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// From dynamically generated geometry (shape creator)...
	{
		sg::CubeParameters cubeParameters;
		cubeParameters.SetEffect( color3DEffect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		Geometry::ptr meshProg( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) );
		PrimitiveList & plProg = ( (Mesh*)meshProg.get() )->GetPrimitiveList();
		auto object = GetObjectAllocator()->NewObject( "cubeDyna" );
		AddGeometryComponent( object, meshProg );
		object->GetFrame().SetPosition( unify::V3< float >( 0 - 5.0f, 0, 0 ) );
		object->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );
	}


	// Sculpter...
	{
		using namespace sculpter;

		class Line : public spline::ISpline
		{
		public:
			Line( float length )
				: m_length{ length }
			{
			}

			Point GetPoint( float delta ) override
			{
				// Clamp delta to within limits (it's either this or assert, but in case we do over or inder our limits a smudge, like with rounding errors)...
				delta = std::min< float >( delta, 1.0f );
				delta = std::max< float >( delta, 0.0f );

				using namespace unify;
				Matrix o = MatrixIdentity();
				V3< float > translation{ o.GetLeft() * ( m_length * delta ) };
				o.Translate( translation );
				return o;
			}

		private:
			float m_length;
		};

		class Sine : public spline::ISpline
		{
		public:
			Sine( float length, float height, unify::Angle a, unify::Angle b )
				: m_length{ length }
				, m_height{ height }
				, m_angleA{ a }
				, m_angleB{ b }
			{
			}

			Point GetPoint( float delta ) override
			{
				// Clamp delta to within limits (it's either this or assert, but in case we do over or inder our limits a smudge, like with rounding errors)...
				delta = std::min< float >( delta, 1.0f );
				delta = std::max< float >( delta, 0.0f );

				using namespace unify;
				Matrix o = MatrixIdentity();
				float PI2 = AnglePI2().ToRadians();
				//y: V3< float > translation{ delta * m_length, sinf( delta * PI2 ) * m_height, 0.0f };
				//z: V3< float > translation{ delta * m_length, 0.0f, sinf( delta * PI2 ) * m_height };
				
				float rad = m_angleA.ToRadians() + delta * m_angleB.ToRadians();
				V3< float > translation{ delta * m_length, sinf( rad ) * m_height, 0.0f };

				o.Translate( translation );
				return o;
			}

		private:
			float m_length;
			float m_height;
			unify::Angle m_angleA;
			unify::Angle m_angleB;
		};


		Sheet::ptr sheet(
			new Sheet(
				unify::MatrixIdentity(),
				{
					spline::ISpline::ptr{ new Line( 3 ) },
					//spline::ISpline::ptr{ new Sine( 3, 1, unify::AngleInDegrees( 0 ), unify::AngleInDegrees( 360 ) ) }
					spline::ISpline::ptr{ new Sine( 3, 3, unify::AngleInDegrees( 0 ), unify::AngleInDegrees( 180 ) ) }
				}, 
				{ 30, 30 } 
			) 
		);

		Mesh * mesh = new Mesh( GetOS()->GetRenderer( 0 ) );
		PrimitiveList & pl = mesh->GetPrimitiveList();

		sheet->Create( pl, { 2.0f, 2.0f }, color3DEffect );

		auto object = GetObjectAllocator()->NewObject( "sculpterCube" );
		AddGeometryComponent( object, Geometry::ptr{ mesh } );
		object->GetFrame().SetPosition( unify::V3< float >( 0 + 5.0f, 0, 0 ) );
		//object->AddComponent( IObjectComponent::ptr( new object::BBoxRendererComponent( GetOS(), color3DEffect ) ) );


		auto  shape( sculpter::Shape::ptr( new sculpter::Shape() ) );
	}
	
}

void MainScene::OnUpdate( UpdateParams params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( unify::V3< float >( 0, 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) ) );
	
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
