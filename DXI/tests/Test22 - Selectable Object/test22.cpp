// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// Selectable Object
/// This tests 3D objects, and their events.

#include <dxi/core/Game.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <dxi/Frustum.h>

#include "CustomEvents.h"

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:
	void Startup();
	bool Update( unify::Seconds elapsed, RenderInfo & renderInfo, IInput & input );
	void Render();
	void Shutdown();
} game;


void MyGame::Startup()
{
	Game::Startup();

	GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
	GetTextureManager()->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	GetTextureManager()->Add( "water_cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	scene::Object::shared_ptr object;

	// Scene 1...
    
    scene::Scene::shared_ptr scene1 = GetSceneManager()->AddScene( "scene1", new scene::Scene );
    scene1->SetOrder( 1.0f );

    // Create shape geometry...
    shapes::ShapeBaseParameters commonParameters;
	commonParameters
		.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 )
		.SetTexture( GetTextureManager()->Find( "borgcube" ) );

    shapes::CubeParameters cubeParameters;
    cubeParameters
		.SetSize( unify::Size3< float >( 2, 2, 2 ) )
		.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
	GetGeometryManager()->Add( "cube",  new Mesh( commonParameters + cubeParameters ) );

    shapes::PointFieldParameters pointFieldParameters;
	pointFieldParameters
		.SetMinorRadius( 0.5f )
		.SetMajorRadius( 1.0f )
		.SetCount( 1000 );
	GetGeometryManager()->Add( "pointfield", new Mesh( commonParameters + pointFieldParameters ) );

    shapes::PointRingParameters pointRingParameters;
    pointRingParameters
		.SetMinorRadius( .25f )
		.SetMajorRadius( .75f )
		.SetCount( 1000 );
	GetGeometryManager()->Add( "pointring", new Mesh( commonParameters + pointRingParameters ) );

    shapes::DashRingParameters dashRingParameters;
	dashRingParameters
		.SetMinorRadius( 1.25f )
		.SetMajorRadius( 2.0f )
		.SetSize( 0.5f )
		.SetCount( 5 )
		.SetDiffuse( unify::Color::ColorGreen() );
	GetGeometryManager()->Add( "dashring", new Mesh( commonParameters + dashRingParameters ) );

    shapes::PyramidParameters pyramidParameters;
	pyramidParameters
		.SetSize( unify::Size3< float >( 2, 2, 2 ) )
		.SetDiffuse( unify::Color::ColorYellow() );
    GetGeometryManager()->Add( "pyramid", new Mesh( commonParameters + pyramidParameters ) );

    shapes::CircleParameters circleParameters;
	circleParameters
		.SetSegments( 24 )
		.SetRadius( 1.0f )
		.SetDiffuse( unify::Color::ColorBlue() );
    GetGeometryManager()->Add( "circle", new Mesh( commonParameters + circleParameters ) );

    shapes::SphereParameters sphereParameters;
	sphereParameters
		.SetSegments( 24 )
		.SetRadius( 1.0f )
		.SetDiffuse( unify::Color::ColorCyan() );
	GetGeometryManager()->Add( "sphere", new Mesh( commonParameters + sphereParameters ) );

    shapes::CylinderParameters cylinderParameters;
	cylinderParameters
		.SetSegments( 24 )
		.SetRadius( 1.0f )
		.SetHeight( 2.0f )
		.SetDiffuse( unify::Color::ColorMagenta() );
	GetGeometryManager()->Add( "cylinder", new Mesh( commonParameters + cylinderParameters ) );

    shapes::TubeParameters tubeParameters;
	tubeParameters
		.SetSegments( 24 )
		.SetMajorRadius( 1.0f )
		.SetMinorRadius( 0.5f )
		.SetHeight( 2.0f )
		.SetDiffuse( unify::Color::ColorRed() );
	GetGeometryManager()->Add( "tube", new Mesh( commonParameters + tubeParameters ) );

    shapes::PlaneParameters planeParameters;
	planeParameters
		.SetSegments( 2 )
		.SetSize( unify::Size< float >( 2.0f, 2.0f ) )
		.SetDiffuse( unify::Color::ColorCyan() );
	GetGeometryManager()->Add( "plane", new Mesh( commonParameters + planeParameters ) );

    shapes::ConeParameters coneParameters;
	coneParameters
		.SetSegments( 24 )
		.SetRadius( 1 )
		.SetHeight( 2 )
		.SetDiffuse( unify::Color::ColorRed() );
	GetGeometryManager()->Add( "cone", new Mesh( commonParameters + coneParameters ) );

    shapes::DashRingParameters selectableParameters;
	selectableParameters
		.SetMinorRadius( 1.25f )
		.SetMajorRadius( 2.0f )
		.SetSize( 0.5f )
		.SetCount( 5 )
		.SetDiffuse( unify::Color::ColorGreen( 255, 150 ) );
	GetGeometryManager()->Add( "selectable", new Mesh( selectableParameters ) );

    shapes::DashRingParameters dragParameters;
	dragParameters
		.SetMinorRadius( 1.25f )
		.SetMajorRadius( 2.0f )
		.SetSize( 0.5f )
		.SetCount( 5 )
		.SetDiffuse( unify::Color::ColorGreen( 255, 150 ) );
	GetGeometryManager()->Add( "drag", new Mesh( dragParameters ) );
    
    scene1->GetListenerMap().Add( "onFocusLost", events::Listener::shared_ptr( new SceneLostFocus ) );
    
    object.reset( new scene::Object( GetGeometryManager()->Find( "selectable" ) ) );
    scene1->Add( "selectable", object );
    object->SetVisible( false );
    object->SetSelectable( false );
    object->GetListenerMap().Add( "onUpdate", events::Listener::shared_ptr( new SelectableAnimation ) );

    object.reset( new scene::Object( GetGeometryManager()->Find( "drag" ) ) );
    scene1->Add( "drag", object );
    object->SetVisible( false );
    object->SetSelectable( false );
    object->GetListenerMap().Add( "onUpdate", events::Listener::shared_ptr( new OnDragAnimation ) );

    events::Listener::shared_ptr selectHighlightOnOverEnterShared( new SelectHighlightOnOverEnter( scene1->FindObject( "selectable" ) ) );
    events::Listener::shared_ptr selectHighlightOnOverExitShared( new SelectHighlightOnOverExit( scene1->FindObject( "selectable" ) ) );
    events::Listener::shared_ptr onDragShared( new OnDrag( scene1->FindObject( "drag" ) ) );
    events::Listener::shared_ptr onDragStopShared( new OnDragStop( scene1->FindObject( "drag" ) ) );
    events::Listener::shared_ptr onDragAnimationShared( new OnDragAnimation() );

    object.reset( new scene::Object( GetGeometryManager()->Find( "cube" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 3, 0 ) );
	scene1->Add( "cube", object ); 
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );
    
    object.reset( new scene::Object( GetGeometryManager()->Find( "pointfield" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 3, 0 ) );
	scene1->Add( "pointfield", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "pointring" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 3, 0 ) );
	scene1->Add( "pointring", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "dashring" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 3, 0 ) );
	scene1->Add( "dashring", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

    object.reset( new scene::Object( GetGeometryManager()->Find( "pyramid" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 0, 0 ) );
	scene1->Add( "pyramid", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "circle" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 0, 0 ) );
	scene1->Add( "circle", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "sphere" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 0, 0 ) );
	scene1->Add( "sphere", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "cylinder" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
	scene1->Add( "cylinder", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "tube" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, -3, 0 ) );
	scene1->Add( "tube", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

    object.reset( new scene::Object( GetGeometryManager()->Find( "plane" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, -3, 0 ) );
	scene1->Add( "plane", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

	object.reset( new scene::Object( GetGeometryManager()->Find( "cone" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, -3, 0 ) );
	scene1->Add( "cone", object );
    object->GetListenerMap().Add( "OnOverEnter", selectHighlightOnOverEnterShared );
    object->GetListenerMap().Add( "OnOverExit", selectHighlightOnOverExitShared );
    object->GetListenerMap().Add( "OnDrag", onDragShared );
    object->GetListenerMap().Add( "OnDragStop", onDragStopShared );

    scene::Object::shared_ptr camera1;
	camera1.reset( new scene::Object() );
	scene1->Add( "camera1", camera1 );
    camera1->GetFrame().SetPosition( unify::V3< float >( 0, 2, -20 ) );
    camera1->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
	scene1->SetCamera( "camera1" );
	scene1->SetSize( GetOS().GetResolution() );

	// Scene 2...
    scene::Scene * scene2 = new scene::Scene;
    GetSceneManager()->AddScene( "scene2", scene2 );
    scene2->SetOrder( 2.0f );

	GeometryGroup * geometryGroup = new GeometryGroup();
	Terra * terra;

    Terra::Parameters terraParameters;
    terraParameters.SetSize( unify::Size< float >( 10, 7 ) );
    terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
    terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
    terraParameters.SetConstant( 0 );
    terraParameters.SetTexArea( unify::TexArea::Full() );
	terra = new Terra( terraParameters );
	dxi::ColorOp depthOp;
	depthOp.r = 0.5f;
	depthOp.g = 0.25f;
	depthOp.b = -1.0f;
	terra->ApplyHeightMap( GetTextureManager()->Find( "earth" ), depthOp );
	terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, GetTextureManager()->Find( "earth" ) );
	terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::False ) );
	geometryGroup->Add( Geometry::shared_ptr( terra ) );

    terraParameters.Reset();
    terraParameters.SetSize( unify::Size< float >( 10, 7 ) );
    terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
    terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
    terraParameters.SetConstant( 0.1f );
    terraParameters.SetTexArea( unify::TexArea::Full() );
	terra = new Terra( terraParameters );
	dxi::ColorOp noDrawOp;
	noDrawOp.r = 1.0f;
	terra->ApplyAlphaMap( GetTextureManager()->Find( "water_cutout" ), noDrawOp );
	terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, GetTextureManager()->Find( "earth" ) );
	terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
	geometryGroup->Add( Geometry::shared_ptr( terra ) );

	object.reset( new scene::Object( Geometry::shared_ptr( geometryGroup ) ) );

	scene2->Add( "group", object );
    scene::Object::shared_ptr camera2;
	camera2.reset( new scene::Object );
    camera2->GetListenerMap().Add( "onUpdate", events::Listener::shared_ptr( new CameraRotation() ) );
	scene2->Add( "camera2", camera2 );
	scene2->SetCamera( "camera2" );
	scene2->SetSize( GetOS().GetResolution() / 3 );
	scene2->SetPosition( unify::V2< int >( 20, GetOS().GetResolution().height - ( GetOS().GetResolution().height / 3 ) - 20 ) );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	return Game::Update( elapsed, renderInfo, input );
}

void MyGame::Render()
{
}

void MyGame::Shutdown()
{
	Game::Shutdown();
}
