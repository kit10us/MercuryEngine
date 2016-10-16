// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Scene.h>
#include <dxi/Frustum.h>
#include <dxi/core/Game.h>
#include <map>

using namespace dxi;
using namespace scene;

Scene::Scene( dxi::core::IGame * game )
: m_game( game )
, m_root( new Object( game->GetOS() ) )
, m_inited( false )
, m_started( false )
, m_lastCullCount( 0 )
, m_renderSolids( true )
, m_renderTrans( true )
, m_cullingEnabled( true )
, m_defaultLighting( false )
, m_defaultZWriteEnable( true )
, m_color( 0, 0, 180, 255 )
, m_renderPhysics( false )
, m_hasFocus( false )
, m_order( 0.0f )
, m_enabled( true )
, m_mouseDownTimeLimit( 0.025f )
, m_mouseDownTime( 0 )
, m_mouseDrag( false )
{
	m_root->SetName( "root" );
}

Scene::~Scene()
{
}

Object::ptr Scene::GetRoot()
{
	return m_root;
}

const Object::ptr Scene::GetRoot() const
{
	return m_root;
}

void Scene::SetPhysicsScene( std::shared_ptr< physics::IScene > scene )
{
	m_physicsScene = scene;
}

physics::IScene * Scene::GetPhysicsScene()
{
	return m_physicsScene.get();
}

const physics::IScene * Scene::GetPhysicsScene() const
{
	return m_physicsScene.get();
}

Object::ptr Scene::FindObject( const std::string & name )
{
	if ( m_root )
	{
		if ( unify::StringIs( m_root->GetName(), name ) )
		{
			return m_root;
		}
		else
		{
			return m_root->FindObject( name );
		}
	}
	else
	{
		return Object::ptr();
	}
}

void Scene::OnInit()
{
	GetRoot()->OnInit();
}

void Scene::OnStart()
{
	GetRoot()->OnStart();
}

void Scene::Update( const RenderInfo & renderInfo )
{
	if ( ! m_inited )
	{
		OnInit();
		m_inited = true;
	}

    if ( ! m_enabled )
    {
        return;
    }

	// On first update, call start ONCE.
	if ( ! m_started )
	{
		OnStart();
		m_started = true;
	}

	if ( m_physicsScene )
    {
		m_physicsScene->Update( renderInfo );
	}

	// Object updating (animation, independant physics)...
	GetRoot()->Update( renderInfo );

	/*
	// TODO: Commented out due to changes of object lists into scene nodes.
	unify::Rect< int > sceneRect( 
		static_cast< int >( m_viewport.GetTopLeftX() ), static_cast< int >( m_viewport.GetTopLeftY() ), 
		static_cast< int >( m_viewport.GetWidth() ), static_cast< int >( m_viewport.GetHeight() ) );
    if ( HasFocus() && input.IsMouseAvailable() && sceneRect.IsIn( input.MouseX(), input.MouseY() ) )
    {
        scene::Camera & camera = GetCamera();

        Frustum frustum( camera.GetMatrix() );

        unify::Size< float > resolution = Game()->GetOS()->GetResolution();
        unify::V2< float > mouseUnit = input.MouseV2< float >();
        mouseUnit /= unify::V2< float >( resolution.width, resolution.height );

        unify::Ray< float > ray;
        frustum.CastPoint( mouseUnit, ray );

        if ( m_mouseDrag )
        {
            // Detect mouse not down (thus, stopping drag).
            if ( ! input.MouseDown( 0 ) )
            {
                if ( m_objectOver )
                {
                    unify::Any onDragStopData( Object::EventData::OnDragStop( m_objectOver.get(), input.MouseV3< float >(), ray ) );
                    //m_objectOver->GetListenerMap().Fire( "onDragStop", onDragStopData );

                    unify::Any onUpData( Object::EventData::OnUp( m_objectOver.get(), input.MouseV3< float >() ) );
                    //m_objectOver->GetListenerMap().Fire( "onUp", onUpData );
                }

                m_mouseDownTime = 0;
                m_mouseDrag = false;
            }            
            
            if ( ! m_objectOver )
            {
                m_mouseDrag = false;
            }
            else
            {
                unify::Any onDrag( Object::EventData::OnDrag( m_objectOver.get(), input.MouseV3< float >(), ray, false ) );
                //m_objectOver->GetListenerMap().Fire( "onDrag", onDrag );
            }
        }
        else
        {
            // Create a list of objects that were hit...
            Object::ptr closestObject;
            float closestObjectDistance = 0.0f;
            for ( auto object : m_objectList )
            {
                if ( ! object->GetGeometry() )
                {
                    continue;
                }
                unify::BBox< > bbox = object->GetBBox();
                
                object->GetFrame().GetMatrix().TransformCoord( bbox.sup );
                object->GetFrame().GetMatrix().TransformCoord( bbox.inf );
                
				unify::V3< float > hitPoint;
                if ( bbox.RayTest( ray, hitPoint ) )
                {
                    float distance = ray.origin.DistanceTo( object->GetFrame().GetPosition() );
                    if ( object->GetSelectable() && ( ! closestObject || distance < closestObjectDistance ) )
                    {
                        closestObjectDistance = distance;
                        closestObject = object;
                    }
                }
            }

            // If we found a new closest object...
            if ( closestObject )
            {
                // If we have a new object we are over...
                if ( closestObject != m_objectOver )
                {
                    // Unselect previous object, if we had one...
                    if ( m_objectOver )
                    {
                        unify::Any onOverExitData( Object::EventData::OnOverExit( m_objectOver.get(), closestObject.get() ) );
                        //m_objectOver->GetListenerMap().Fire( "onOverExit", onOverExitData );
                    }
                    m_objectOver = closestObject;
                    unify::Any onOverEnterData( Object::EventData::OnOverEnter( m_objectOver.get(), closestObjectDistance, input ) );
                    //m_objectOver->GetListenerMap().Fire( "onOverEnter", onOverEnterData );
                }
                else
                {
                    if ( m_objectOver )
                    {
                        // We are over same object, allow more events...
                        if ( input.MouseDown( 0 ) )
                        {
                            if ( m_mouseDownTime )
                            {
                                m_mouseDownTime += renderInfo.GetDelta();
                                if ( m_mouseDownTime >= m_mouseDownTimeLimit )
                                {
                                    m_mouseDrag = true;
                                    m_mouseDownTime = 0.0f;
                                    
                                    // We are past our down time limit, so we are no longer "over" the object, we are "drag".
                                    unify::Any onOverExitData( Object::EventData::OnOverExit( m_objectOver.get(), m_objectOver.get() ) );
                                    //m_objectOver->GetListenerMap().Fire( "onOverExit", onOverExitData );
                                    
                                    // Initial calls
                                    unify::Any onDrag( Object::EventData::OnDrag( m_objectOver.get(), input.MouseV3< float >(), ray, true ) );
                                    //m_objectOver->GetListenerMap().Fire( "onDrag", onDrag );
                                }
                            }
                            else
                            {
                                unify::Any onDownData( Object::EventData::OnDown( m_objectOver.get(), input.MouseV3< float >() ) );
                                //m_objectOver->GetListenerMap().Fire( "onDown", onDownData );
                                m_mouseDownTime += renderInfo.GetDelta();
                            }
                        }
                        else if ( ! input.MouseDown( 0 ) )
                        {
                            unify::Any onUpData( Object::EventData::OnUp( m_objectOver.get(), input.MouseV3< float >() ) );
                            //m_objectOver->GetListenerMap().Fire( "onUp", onUpData );
                            m_mouseDownTime = 0;
                            m_mouseDrag = false;
                        }
                    }
                }
            }

            // No new closest object, so no object selected either...
            else
            {
                // If we have no object to be over, but we had one...
                if ( m_objectOver )
                {
                    unify::Any eventData( Object::EventData::OnOverExit( m_objectOver.get(), closestObject.get() ) );
                    //m_objectOver->GetListenerMap().Fire( "onOverExit", eventData );
                    m_objectOver.reset();
                }
            }
        }
    }
	*/
}

void Scene::Suspend()
{
	GetRoot()->OnSuspend();
}

void Scene::Resume()
{
	GetRoot()->OnResume();
}						

RenderInfo & Scene::GetRenderInfo()
{
	return m_renderInfo;
}

struct FinalObject
{
	Object::ptr object;	 // Replace with geometry.
	unify::Matrix transform;
};

struct FinalCamera
{
	Object::ptr object;
	unify::Matrix transform;
	Camera * camera;
};							  

void Accumulate( std::list< FinalObject > & renderList, std::list< FinalCamera > & cameraList, Object::ptr current, unify::Matrix parentTransform )
{
	assert( current );
	assert( current->IsEnabled() );

	// Solve our transform.
	unify::Matrix transform = current->GetFrame().GetMatrix(); 
	transform *= parentTransform;

	// Check for a camera...
	scene::Camera * camera{};
	for( int i = 0; i < current->ComponentCount(); ++i )
	{
		IComponent::ptr component = current->GetComponent( i );
		if( !component->IsEnabled() ) continue;

		camera = dynamic_cast< scene::Camera * >( component.get() );
		if( camera != nullptr )
		{
			cameraList.push_back( { current, transform, camera } );
		}
	}

	renderList.push_back(
	{
		current,
		transform
	} );
						   
	// Handle children
	Object::ptr child = current->GetFirstChild();
	while( child )
	{
		if( child->IsEnabled() ) 
		{
			Accumulate( renderList, cameraList, child, transform );
		}

		child = child->GetNext();
	}
}

void Scene::Render( size_t index, const Viewport & viewport )
{	
	std::list< FinalObject > renderList;
	std::list< FinalCamera > cameraList;
	
	unify::Matrix transform = unify::Matrix::MatrixIdentity();

	if( GetRoot() && GetRoot()->IsEnabled() )
	{
		Accumulate( renderList, cameraList, GetRoot(), transform );
	}

	for( auto camera : cameraList )
	{	
		if( camera.camera->GetRenderer() != index ) continue;

		RenderInfo renderInfo( m_renderInfo );
		renderInfo.SetRenderer( m_game->GetOS()->GetRenderer( index ) ); // TODO: Likely renderer should know it's index, AND this Render function should take a IRenderer instead of an index.
		renderInfo.SetViewMatrix( camera.transform.Inverse() );
		renderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		for( auto object : renderList )
		{
 			renderInfo.SetWorldMatrix( object.transform );
			object.object->RenderSimple( renderInfo );
		}
	}

	if( m_renderPhysics && m_physicsScene )
	{
		m_physicsScene->Render();
	}

	m_renderInfo.IncrementFrameID();
}

void Scene::SetDefaultLighting( bool lighting )
{
	m_defaultLighting = lighting ? true : FALSE;
}

void Scene::SetDefaultZWriteEnable( bool zWriteEnable )
{
	m_defaultZWriteEnable = zWriteEnable ? true : FALSE;
}

unsigned int Scene::LastCullCount() const
{
	return m_lastCullCount;
}

void Scene::SetRenders( bool bSolids, bool bTrans ) 
{ 
	m_renderSolids = bSolids;
	m_renderTrans = bTrans; 
}

void Scene::SetCulling( bool enabled ) 
{ 
	m_cullingEnabled = enabled;
}

bool Scene::RayTest( const unify::V3< float > * pFirst, const unify::V3< float > * pSecond )
{
	return true;
}

void Scene::SetSize( const unify::Size< float > & size )
{
	m_viewport.SetSize( size );
}

unify::Size< float > Scene::GetSize() const
{
	return m_viewport.GetSize();
}

void Scene::SetPosition( const unify::V2< float > & position )
{
	m_viewport.SetUL( position );
}

unify::V2< float > Scene::GetPosition() const
{
	return m_viewport.GetUL();
}

void Scene::SetZ( const unify::MinMax< float > & z )
{
	m_viewport.SetMinDepth( z.Min() );
	m_viewport.SetMaxDepth( z.Max() );
}

void Scene::SetColor( const unify::Color & color )
{
	m_color = color;
}

void Scene::SetOrder( float order )
{
    m_order = order;
}

float Scene::GetOrder() const
{
    return m_order;
}

void Scene::SetEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool Scene::GetEnabled() const
{
    return m_enabled;
}

void Scene::SetFocus( bool focus )
{
    m_hasFocus = focus;
}

bool Scene::HasFocus() const
{
    return m_hasFocus;
}

bool Scene::CantLoseFocus() const
{
    return m_mouseDrag;
}

void Scene::SetRenderPhysics( bool renderPhysics )
{
	m_renderPhysics = renderPhysics;
}

bool Scene::GetRenderPhysics() const
{
	return m_renderPhysics;
}

Object::ptr Scene::GetObjectOver() const
{
    return m_objectOver;
}

void Scene::SetObjectOver( Object::ptr objectOver )
{
    m_objectOver = objectOver;
}
