// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MainScene.h>
#include <me/render/Mesh.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/component/CameraComponent.h>
#include <sg/ShapeCreators.h>

#include <me/render/Billboard.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

#include <SolarBody.h>


MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
		auto TM = GetManager< ITexture >();
		auto EM = GetManager< Effect >();

	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectColor.effect" ) );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "texture3d", unify::Path( "EffectTexture.effect" ) );

	GetManager< ITexture >()->Add( "01Mercury", unify::Path( "solar\\01Mercury.bmp" ) );
	//textured3DEffect->SetTexture( 0, GetManager< ITexture >()->Find( "01Mercury" ) );


	// Add an object to act as a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add a camera component to the camera object
	auto * cameraComponent = new component::CameraComponent();
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->AddComponent( component::IObjectComponent::ptr( cameraComponent ) );

	auto createObject = [&]( sg::ShapeBaseParameters parameters ) //->me::object::Object*
	{
		const unify::V3< float > startPos{ unify::V3< float >::V3Zero() };
		
		static int objectIndex = 0;
		std::string objectName = "object " + unify::Cast< std::string >( objectIndex++ );
		auto object = GetObjectAllocator()->NewObject( objectName );
		
		object->GetFrame().SetPosition(startPos);

		Geometry::ptr meshProg(sg::CreateShape(GetOS()->GetRenderer(0), parameters));
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		AddGeometryComponent(object, meshProg);
		
		return object;
	};

	// Create solar bodies...

	{
		auto effext{ textured3DEffect->Duplicate() };
		effext->SetTexture( 0, TM->Add("00sun", unify::Path("solar\\00sun.jpg")));

		sg::SphereParameters parameters;
		parameters.SetEffect(effext);
		parameters.SetRadius( 1.0f );
		//	parameters.SetDiffuse( unify::Color::ColorRed() );
		auto object = createObject(parameters);
		m_rootSolarBody.reset( new SolarBody( "Sun", object, SpacialData( 0.0f, unify::AngleZero(), unify::AngleZero(), unify::AngleZero(), unify::AngleZero() ) ) );
	}

	{
		auto effext{ textured3DEffect->Duplicate() };
		effext->SetTexture(0, TM->Add("01Mercury", unify::Path("solar\\01Mercury.bmp")));
		
		sg::SphereParameters parameters;
		parameters.SetEffect(effext);
		parameters.SetRadius(0.5f);
		auto object = createObject(parameters);

		m_rootSolarBody->AddChild( 
			SolarBody::ptr( 
				new SolarBody( "Earth", object, 
					SpacialData( 3.0f, unify::AngleInDegrees( 0.f ), unify::AngleInDegrees( 10.0f ), unify::AngleInDegrees( 0.0f ), unify::AngleInDegrees( 10.0f ) 
						) 
					) 
				)
			);
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

	// Update solar system...
	if ( m_rootSolarBody )
	{
		m_rootSolarBody->Update( params, nullptr );
	}
}
