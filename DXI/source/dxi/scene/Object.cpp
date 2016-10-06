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
{
}

Object::Object( Geometry::shared_ptr geometry, const unify::V3< float > position )
: m_enabled( true )
, m_visible( true )
, m_geometry( geometry )
, m_geometryInstanceData( geometry->CreateInstanceData() )
, m_renderBBox( false )
, m_geometryMatrix( unify::Matrix::MatrixIdentity() )
{
    GetFrame().SetPosition( position );
}

Object::~Object()
{
}

void Object::SetName( std::string name )
{
	m_name = name;
}

std::string Object::GetName() const
{
	return m_name;
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

unify::FrameLite & Object::GetFrame()
{
	return m_frame;
}

const unify::FrameLite & Object::GetFrame() const
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

void Object::Update( const RenderInfo & renderInfo, core::IInput & input )
{
    // Do not update if we are not enabled.
    if ( ! m_enabled )
    {
        return;
    }

	if( m_controller )
	{
		m_controller->Update( renderInfo, input );
	}

	if( m_geometry )
	{
		m_geometry->Update( renderInfo, m_geometryInstanceData.get() );
	}

    unify::Matrix worldMatrix;
    // TODO: dxi::Transform::Get( dxi::Transform::Index::World, worldMatrix );
    unify::V3< float > worldForwardVector = worldMatrix.GetForward();
    int x(0);x;
}

void Object::Render( const RenderInfo & renderInfo )
{	
	// Render self and children...
	if ( m_visible )
	{
		RenderInfo myRenderInfo( renderInfo );

		myRenderInfo.SetWorldMatrix( m_geometryMatrix * m_frame.GetMatrix() * myRenderInfo.GetWorldMatrix() );

		if ( m_geometry )
		{
			m_geometry->Render( myRenderInfo, m_geometryInstanceData.get() );
		}	

		if( GetFirstChild() )
		{
			GetFirstChild()->Render( myRenderInfo );
		}
	}

	if( GetNext() )
	{
		GetNext()->Render( renderInfo );
	}
								  
	/*
	// Do nothing if we have no geometry to render, or are not visible...
	if( ! m_geometry || ! m_visible )
	{
        return;
	}

	RenderInfo myRenderInfo( renderInfo );

    // Prevent rendering if we have already render this frame, and are flagged to do such a check. 
	if( myRenderInfo.IsOptionTrue( RenderOption::FrameCheck ) && m_checkFrame )
	{
		if( m_lastFrameID == renderInfo.FrameID() ) 
        {
            return;
        }
	}
	m_lastFrameID = renderInfo.FrameID(); // Regardless, note that we have rendered this render frame.

	// Use matrix...
	if( !myRenderInfo.IsOptionTrue( RenderOption::NoFrame ) )
	{
		m_frame.Update();
		myRenderInfo.SetWorldMatrix( m_geometryMatrix * m_frame.GetFinalMatrix() );
	}

	std::string name = m_tags["name"];

	m_geometry->Render( myRenderInfo, m_geometryInstanceData.get() );
	*/

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
	GetFrame().GetMatrix().TransformCoord( bboxTrans.inf );
	GetFrame().GetMatrix().TransformCoord( bboxTrans.sup );
	//return m_Physics.GetBBox();
	return bboxTrans;
}

const unify::BBox< float > & Object::GetBBox()
{
	return m_geometry->GetBBox();
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

Object::shared_ptr Object::GetPrevious()
{
	return m_previous;
}

const Object::shared_ptr Object::GetPrevious() const
{
	return m_previous;
}

Object::shared_ptr Object::GetNext()
{
	return m_next;
}

const Object::shared_ptr Object::GetNext() const
{
	return m_next;
}

Object::shared_ptr Object::GetParent()
{
	return m_parent;
}

const Object::shared_ptr Object::GetParent() const
{
	return m_parent;
}

Object::shared_ptr Object::GetFirstChild()
{
	return m_firstChild;
}

const Object::shared_ptr Object::GetFirstChild() const
{
	return m_firstChild;
}

Object::shared_ptr Object::AddChild( std::string name )
{
	Object::shared_ptr lastChild = GetFirstChild();
	if ( ! lastChild )
	{
		// No children...
		m_firstChild.reset( new Object );
		lastChild = m_firstChild;
	}
	else
	{
		// Find our last child...
		while( lastChild->GetNext() )
		{
			lastChild = lastChild->GetNext();
		}
		lastChild->m_next.reset( new Object );
		lastChild->m_next->m_previous = lastChild;
		lastChild = lastChild->GetNext();	 
	}
	lastChild->m_parent.reset( this );
	
	// Last child is new child...
	lastChild->SetName( name );
	return lastChild;
}

Object::shared_ptr Object::FindObject( std::string name )
{	
	Object::shared_ptr child = GetFirstChild();
	while( child )
	{
		if ( unify::StringIs( child->GetName(), name ) )
		{
			return child;
		}
		Object::shared_ptr found = child->FindObject( name );
		if ( found )
		{
			return found;
		}
		child = child->GetNext();
	}

	// Not found...
	return Object::shared_ptr();
}
