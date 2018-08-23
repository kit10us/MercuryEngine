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

MainScene::MainScene( me::game::Game * gameInstance )
	:Scene( gameInstance, "main" )
{	
}

void MainScene::OnStart()
{
	Effect::ptr color3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectColor.effect" ) );
	Effect::ptr textured3DEffect = GetManager< Effect >()->Add( "color3d", unify::Path( "EffectTextured.effect" ) );

	// Add an object to act as a camera...
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add a camera component to the camera object
	auto * cameraComponent = new component::CameraComponent();
	cameraComponent->SetProjection( unify::MatrixPerspectiveFovLH( 3.141592653589f / 4.0f, 800 / 600, 1, 1000 ) );
	camera->AddComponent( component::IObjectComponent::ptr( cameraComponent ) );

	auto createObject = [&]( sg::ShapeBaseParameters parameters)->me::object::Object*
	{
		const unify::V3< float > startPos{ -2.5f, 2.5f, 0.0f };
		const int itemsPerRow = 3;
		const int itemsPerColumn = 3;
		const int itemsPerPage = itemsPerRow * itemsPerColumn;
		const unify::V3< float > changePerRow { 0.0f, -2.5f, 0.0f };
		const unify::V3< float > changePerColumn { 2.5f, 0.0f, 0.0f };
		const unify::V3< float > changePerPage { 0.0f, 0.0f, 2.5f };

		static int objectIndex = 0;
		int page = objectIndex / itemsPerPage;
		int row = (objectIndex % itemsPerPage) / itemsPerRow;
		int column = (objectIndex % itemsPerPage) % itemsPerRow;
			
		std::string objectName = "object " + unify::Cast< std::string >( objectIndex++ );
		auto object = GetObjectAllocator()->NewObject( objectName );
		
		unify::V3< float > pos = startPos + unify::V3< float >{ (changePerRow * row) + (changePerColumn * column) + (changePerPage * page ) };
		object->GetFrame().SetPosition( pos );

		Geometry::ptr meshProg(sg::CreateShape(GetOS()->GetRenderer(0), parameters));
		PrimitiveList & plProg = ((Mesh*)meshProg.get())->GetPrimitiveList();
		AddGeometryComponent(object, meshProg);
		
		return object;
	};

	// Create objects...
	{
		sg::CubeParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetSize(unify::Size3< float >(1, 1, 1));
		parameters.SetDiffuseFaces(unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta());
		auto object = createObject( parameters );
	}

	{
		sg::PointFieldParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetMajorRadius( 1 );
		parameters.SetMinorRadius( 0.5f );
		parameters.SetCount( 1000 );
		auto object = createObject(parameters);
	}

	{
		sg::PointRingParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetMajorRadius(1.0f - 0.25f);
		parameters.SetMinorRadius(0.25f);
		parameters.SetCount(1000);
		auto object = createObject(parameters);
	}

	{
		sg::DashRingParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetMajorRadius(1);
		parameters.SetMinorRadius(0.9f);
		parameters.SetSize( 0.5f );
		parameters.SetCount(12);
		auto object = createObject(parameters);
	}

	{
		sg::PyramidParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetSize( { 1, 1, 1 } );
		auto object = createObject(parameters);
	}

	{
		sg::CircleParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetRadius( 1.0f );
		auto object = createObject(parameters);
	}

	{
		sg::SphereParameters parameters;
		parameters.SetEffect( color3DEffect );
		parameters.SetRadius( 0.5f );
		parameters.SetDiffuse( unify::Color::ColorRed() );
		auto object = createObject(parameters);
	}

	{
		sg::CylinderParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetRadius(0.25f);
		parameters.SetHeight(1.0f);
		auto object = createObject(parameters);
	}

	{
		sg::TubeParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetMajorRadius(0.5f);
		parameters.SetMinorRadius(0.25f );
		parameters.SetHeight(1.0f);
		auto object = createObject(parameters);
	}

	{
		sg::PlaneParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetSize( {1, 1} );
		auto object = createObject(parameters);
	}

	{
		sg::ConeParameters parameters;
		parameters.SetEffect(color3DEffect);
		parameters.SetRadius( 0.25f );
		parameters.SetHeight( 1 );
		auto object = createObject(parameters);
	}
}

void MainScene::OnUpdate( const UpdateParams & params )
{
	// Use of camera controls to simplify camera movement...
	Object * camera = FindObject( "camera" );
	
	camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( params.GetDelta().GetSeconds() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
