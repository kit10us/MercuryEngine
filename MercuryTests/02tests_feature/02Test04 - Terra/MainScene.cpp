// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MainScene.h>
#include <me/Mesh.h>
#include <me/Terra.h>
#include <me/factory/EffectFactories.h>
#include <me/object/GeometryComponent.h>				

using namespace me;
using namespace scene;
using namespace object;

MainScene::MainScene( me::Game * game )
	:Scene( game, "main" )
{	
}

void MainScene::OnStart()
{
	Object * camera = GetObjectAllocator()->NewObject( "camera" );
	camera->AddComponent( IObjectComponent::ptr( new CameraComponent() ) );
	CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "Camera" ).get() );
	cameraComponent->SetProjection(	unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, GetOS()->GetRenderer(0)->GetViewport().GetSize().AspectRatioWH(), 1, 1000 ) );
	camera->GetFrame().SetPosition( unify::V3< float >( 0, 12, -12 ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

	// Add textures we will need for our effects, and terrain generation/modification.
	{
		TextureParameters parameters;
		parameters.renderable = true;
		parameters.lockable = true;
		parameters.min = Filtering::Point;
		parameters.mag = Filtering::Point;
		parameters.mip = Filtering::Point;
		GetManager< ITexture >()->Add( "4", "4.bmp", "", &parameters );
	}

	// Load an effect, then modify it to fit our needs.
	Effect::ptr landEffect = GetManager< Effect >()->Add( "4", "EffectTextured.effect" );
	landEffect->SetTexture( 0, GetManager< ITexture >()->Find( "4" ) );

	Terra::Parameters parameters;
	Terra * terra;
	parameters.Reset();
	parameters.SetSize( unify::Size< float >( 10, 7 ) );
	parameters.SetEffect( GetManager< Effect >()->Find( "4" ) ); // Note the use of FindEffect to simplify interaction with the Effect Manager.
	parameters.SetPoints( unify::RowColumn< unsigned int >( 200, 140 ) );
	parameters.SetConstant( 0.0f );
	parameters.SetTexArea( unify::TexAreaFull() );

	//parameters.SetHeightMap( Terra::TextureOpMap( GetManager< ITexture >()->Find( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );	
	parameters.SetHeightMap( Terra::TextureOpMap( GetManager< ITexture >()->Find( "4" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 3.0f, 2.0f, 1.0f ) ) );

	//parameters.SetDiffuse( unify::ColorUnit::ColorUnitRed() );
	//parameters.SetDiffuses( unify::ColorUnit::ColorUnitBlack(), unify::ColorUnit::ColorUnitRed(), unify::ColorUnit::ColorUnitBlue(), unify::ColorUnit::ColorUnitWhite() );

	terra = new Terra( GetOS()->GetRenderer( 0 ), parameters );
	auto land = GetObjectAllocator()->NewObject( "land" );
	AddGeometryComponent( land, Geometry::ptr( terra ) );
}

void MainScene::OnUpdate( UpdateParams params )
{
	Object * camera = FindObject( "camera" );
	
	//camera->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::AngleInRadians( renderInfo.GetDelta() ) );
	camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}
