// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Object.h>
#include <dxi/core/Game.h>
//#include <dxi/shapes/ShapeType.h>

using namespace dxi;
using namespace scene;

Object::Object()
: m_enabled( true )
, m_visible( true )
, m_selectable( false )
, m_checkFrame( true )
, m_lastFrameID( 0 )
, m_renderBBox( false )
, m_geometryMatrix( unify::Matrix::MatrixIdentity() )
, m_scene( 0 )
{
}

Object::Object( Geometry::shared_ptr geometry, std::shared_ptr< physics::IInstance > physics )
: m_enabled( true )
, m_visible( true )
, m_geometry( geometry )
, m_geometryInstanceData( geometry->CreateInstanceData() )
, m_renderBBox( false )
, m_physics( physics )
, m_geometryMatrix( unify::Matrix::MatrixIdentity() )
, m_scene( 0 )
{
}

Object::Object( Geometry::shared_ptr geometry, const unify::V3< float > position )
: m_enabled( true )
, m_visible( true )
, m_geometry( geometry )
, m_geometryInstanceData( geometry->CreateInstanceData() )
, m_renderBBox( false )
, m_geometryMatrix( unify::Matrix::MatrixIdentity() )
, m_scene( 0 )
{
    GetFrame().SetPosition( position );
}

Object::~Object()
{
}

void Object::SetScene( Scene * scene )
{
    m_scene = scene;
}

Scene * Object::GetScene() const
{
    return m_scene;
}

void Object::SetEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool Object::GetEnabled() const
{
    return m_enabled;
}

void Object::SetVisible( bool visible )
{
    m_visible = visible;
}

bool Object::GetVisible() const
{
    return m_visible;
}

void Object::SetSelectable( bool selectable )
{
    m_selectable = selectable;
}

bool Object::GetSelectable() const
{
    return m_selectable;
}

void Object::CheckFrame( bool checkFrame )
{
	m_checkFrame = checkFrame;
}

bool Object::CheckFrame() const
{
	return m_checkFrame;
}

unify::Frame & Object::GetFrame()
{
	return m_frame;
}

const unify::Frame & Object::GetFrame() const
{
	return m_frame;
}

void Object::SetEffect( const Effect & effect )
{
	m_effect = effect;
}

Effect & Object::GetEffect()
{
	return m_effect;
}

Geometry::shared_ptr Object::GetGeometry()
{
	return m_geometry;
}

void Object::SetGeometry( Geometry::shared_ptr geometry )
{
	m_geometry = geometry;
    m_geometryInstanceData.reset( geometry ? geometry->CreateInstanceData() : 0 );
}

std::shared_ptr< physics::IInstance > Object::GetPhysics()
{
	return m_physics;
}

std::shared_ptr< const physics::IInstance > Object::GetPhysics() const
{
	return m_physics;
}

void Object::SetPhysics( std::shared_ptr< physics::IInstance > physics )
{
	m_physics = physics;
}

controllers::IController::shared_ptr Object::GetController()
{
	return m_controller;
}

void Object::SetController( controllers::IController::shared_ptr controller )
{
	m_controller = controller;
}

void Object::Update( unify::Seconds elapsed, core::IInput & input )
{
    // Do not update if we are not enabled.
    if ( ! m_enabled )
    {
        return;
    }

    unify::Any onUpdateEventData( EventData::OnUpdate( this, elapsed, input ) );
    GetListenerMap().Fire( "onUpdate", onUpdateEventData );

	if( m_controller )
	{
		m_controller->Update( elapsed, input );
	}

	if( m_geometry )
	{
		m_geometry->Update( elapsed, m_geometryInstanceData.get() );
	}

    unify::Matrix worldMatrix;
    // TODO: dxi::Transform::Get( dxi::Transform::Index::World, worldMatrix );
    unify::V3< float > worldForwardVector = worldMatrix.GetForward();
    int x(0);x;
}

void Object::Render( RenderInfo & renderInfo )
{
	// Do nothing if we have no geometry to render, or are not visible...
	if( ! m_geometry || ! m_visible )
	{
        return;
	}

    // Prevent rendering if we have already render this frame, and are flagged to do such a check. 
	if( renderInfo.IsOptionTrue( RenderOption::FrameCheck ) && m_checkFrame )
	{
		if( m_lastFrameID == renderInfo.FrameID() ) 
        {
            return;
        }
	}
	m_lastFrameID = renderInfo.FrameID(); // Regardless, note that we have rendered this render frame.


	// Use matrix...
	if( ! renderInfo.IsOptionTrue( RenderOption::NoFrame ) )
	{
		m_frame.Update();
		renderInfo.SetWorldMatrix( m_geometryMatrix * m_frame.GetFinalMatrix() );
	}

	std::string name = m_tags["name"];

	m_geometry->Render( renderInfo, m_geometryInstanceData.get() );

	/* TODO: DX11:
	if( renderInfo.IsOptionTrue( RenderOption::RenderAllBBox ) || renderInfo.IsOptionTrue( RenderOption::RenderBBox ) )
	{
        shapes::CubeParameters cubeParameters;
        cubeParameters.SetInf( m_geometry->GetBBox().inf );
		cubeParameters.SetSup( m_geometry->GetBBox().sup );
		cubeParameters.SetDiffuse( unify::Color::ColorARGB( 100, 40, 40, 255 ) );
		// TODO: cubeParameters.SetEffect( ... );
		
		Mesh bbox;
		bbox.CreateFromShape( cubeParameters );
		//bbox.GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::True, Blend::Effect::SrcAlpha, Blend::Effect::InvSrcAlpha ) );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ZWRITEENABLE, false );

		bbox.Render( renderInfo, 0 );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ZWRITEENABLE, true );
	}
	*/
}

void Object::RenderBBox( bool bTF )
{
	m_renderBBox = bTF;
}

bool Object::RenderBBox() const
{
	return m_renderBBox;
}

unify::BBox< float > Object::GetBBoxXFormed()
{
	unify::BBox< float > bboxTrans;
	if( ! m_geometry.get() ) return bboxTrans;

	// Update BBox by render object...
	bboxTrans = m_geometry->GetBBox();
	GetFrame().GetFinalMatrix().TransformCoord( bboxTrans.inf );
	GetFrame().GetFinalMatrix().TransformCoord( bboxTrans.sup );
	//return m_Physics.GetBBox();
	return bboxTrans;
}

const unify::BBox< float > & Object::GetBBox()
{
	return m_geometry->GetBBox();
}

void Object::SyncFrame( const unify::Frame & frame )
{
	m_frame = frame;
}

void Object::SyncBBox( const unify::BBox< float > & bbox )
{
	m_geometry->GetBBox() = bbox;
}

unify::Matrix & Object::GetGeometryMatrix()
{
	return m_geometryMatrix;
}

const unify::Matrix & Object::GetGeometryMatrix() const
{
	return m_geometryMatrix;
}

GeometryInstanceData::shared_ptr Object::GetGeometryInstanceData()
{
	return m_geometryInstanceData;
}

std::map< std::string, std::string > & Object::GetTags()
{
	return m_tags;
}

const std::map< std::string, std::string > & Object::GetTags() const
{
	return m_tags;
}
