// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Scene.h>
#include <dxi/Frustum.h>
#include <dxi/core/Game.h>
#include <map>

using namespace dxi;
using namespace scene;

Scene::Scene( dxi::core::IGame & game )
: GameDependant( game )
, m_lastCullCount( 0 )
, m_renderSolids( true )
, m_renderTrans( true )
, m_cullingEnabled( true )
, m_defaultLighting( false )
, m_defaultZWriteEnable( true )
, m_viewport( Game().GetOS().GetDefaultViewport() )
, m_color( 0, 0, 180, 255 )
, m_renderPhysics( false )
, m_hasFocus( false )
, m_order( 0.0f )
, m_enabled( true )
, m_visible( true )
, m_mouseDownTimeLimit( 0.025f )
, m_mouseDownTime( 0 )
, m_mouseDrag( false )
{
}

Scene::~Scene()
{
}

void Scene::Clear()
{
	m_objectList.clear();
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

std::shared_ptr< Object > Scene::Add( Object * object )
{
	std::shared_ptr< Object > object_ptr( object );
	m_objectList.push_back( object_ptr );
    object->SetScene( this );
	if( m_physicsScene && object->GetPhysics() )
	{
		m_physicsScene->Add( object->GetPhysics() );
	}
	return object_ptr;
}

std::shared_ptr< Object > Scene::Add( const std::string & name )
{
	std::shared_ptr< Object > object_ptr( new Object );

	object_ptr->GetTags()[ "name" ] = name;
    
	object_ptr->SetScene( this );
	m_objectList.push_back( object_ptr );
	m_objectMap[ name ] = object_ptr;
	if( m_physicsScene && object_ptr->GetPhysics() )
	{
		m_physicsScene->Add( object_ptr->GetPhysics() );
	}
	return object_ptr;
}


Object::shared_ptr Scene::FindObject( const std::string & name )
{
	ObjectMap::iterator itr = m_objectMap.find( name );
	return itr->second;
}

bool Scene::FindPosition( const std::string & name, unify::V3< float > & position ) const
{
	ObjectMap::const_iterator itr = m_objectMap.find( name );
	if( itr == m_objectMap.end() )
	{
		return false;
	}
	position = itr->second->GetFrame().GetPosition();
	return true;
}

void Scene::SetCamera( const std::string & name )
{
	m_camera.SetObject( FindObject( name ) );
    m_cameraName = name;
}

scene::Camera & Scene::GetCamera()
{
    return m_camera;
}

unsigned int Scene::ObjectCount() const
{
	return static_cast< unsigned int >( m_objectList.size() );
}

void Scene::Update( unify::Seconds elapsed, core::IInput & input )
{
    if ( ! m_enabled )
    {
        return;
    }

    unify::Any onUpdateEventData( Scene::EventData::OnUpdate( this, m_objectList, elapsed, input ) );
    GetListenerMap().Fire( "OnUpdate", onUpdateEventData );

	if ( m_physicsScene )
    {
		m_physicsScene->Update( elapsed, input );
	}

	// Object updating (animation, independant physics)...
	for( ObjectList::iterator itrObject = m_objectList.begin(), end = m_objectList.end(); itrObject != end; ++itrObject )
	{
		(*itrObject)->Update( elapsed, input );
	}
     
	unify::Rect< int > sceneRect( 
		static_cast< int >( m_viewport.GetTopLeftX() ), static_cast< int >( m_viewport.GetTopLeftY() ), 
		static_cast< int >( m_viewport.GetWidth() ), static_cast< int >( m_viewport.GetHeight() ) );
    if ( HasFocus() && input.IsMouseAvailable() && sceneRect.IsIn( input.MouseX(), input.MouseY() ) )
    {
        scene::Camera & camera = GetCamera();

        Frustum frustum( camera.GetMatrix() );

        unify::Size< float > resolution = Game().GetOS().GetResolution();
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
                    m_objectOver->GetListenerMap().Fire( "onDragStop", onDragStopData );

                    unify::Any onUpData( Object::EventData::OnUp( m_objectOver.get(), input.MouseV3< float >() ) );
                    m_objectOver->GetListenerMap().Fire( "onUp", onUpData );
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
                m_objectOver->GetListenerMap().Fire( "onDrag", onDrag );
            }
        }
        else
        {
            // Create a list of objects that were hit...
            Object::shared_ptr closestObject;
            float closestObjectDistance = 0.0f;
            for ( std::list< Object::shared_ptr >::iterator itr = m_objectList.begin(), end = m_objectList.end(); itr != end; ++itr )
            {
                Object::shared_ptr object = *itr;
                if ( ! object->GetGeometry() )
                {
                    continue;
                }
                unify::BBox< > bbox = object->GetBBox();
                
                object->GetFrame().GetFinalMatrix().TransformCoord( bbox.sup );
                object->GetFrame().GetFinalMatrix().TransformCoord( bbox.inf );
                
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
                        m_objectOver->GetListenerMap().Fire( "onOverExit", onOverExitData );
                    }
                    m_objectOver = closestObject;
                    unify::Any onOverEnterData( Object::EventData::OnOverEnter( m_objectOver.get(), closestObjectDistance, input ) );
                    m_objectOver->GetListenerMap().Fire( "onOverEnter", onOverEnterData );
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
                                m_mouseDownTime += elapsed;
                                if ( m_mouseDownTime >= m_mouseDownTimeLimit )
                                {
                                    m_mouseDrag = true;
                                    m_mouseDownTime = 0.0f;
                                    
                                    // We are past our down time limit, so we are no longer "over" the object, we are "drag".
                                    unify::Any onOverExitData( Object::EventData::OnOverExit( m_objectOver.get(), m_objectOver.get() ) );
                                    m_objectOver->GetListenerMap().Fire( "onOverExit", onOverExitData );
                                    
                                    // Initial calls
                                    unify::Any onDrag( Object::EventData::OnDrag( m_objectOver.get(), input.MouseV3< float >(), ray, true ) );
                                    m_objectOver->GetListenerMap().Fire( "onDrag", onDrag );
                                }
                            }
                            else
                            {
                                unify::Any onDownData( Object::EventData::OnDown( m_objectOver.get(), input.MouseV3< float >() ) );
                                m_objectOver->GetListenerMap().Fire( "onDown", onDownData );
                                m_mouseDownTime += elapsed;
                            }
                        }
                        else if ( ! input.MouseDown( 0 ) )
                        {
                            unify::Any onUpData( Object::EventData::OnUp( m_objectOver.get(), input.MouseV3< float >() ) );
                            m_objectOver->GetListenerMap().Fire( "onUp", onUpData );
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
                    m_objectOver->GetListenerMap().Fire( "onOverExit", eventData );
                    m_objectOver.reset();
                }
            }
        }
    }
}

RenderInfo & Scene::GetRenderInfo()
{
	return m_renderInfo;
}

void Scene::Render()
{
	if( ! m_camera.HasObject() )
	{
		return;
	}

	Viewport viewportBackup;
	Game().GetOS().GetRenderer()->GetViewport( viewportBackup );

	Viewport viewport;
	Game().GetOS().GetRenderer()->SetViewport( m_viewport );

	// TODO: DX11 (this is moved to the begin scene, so we need to figure this out): win::DX::GetDxDevice()->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_color, 1.0f, 0 );
	// TODO: DX11: win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, false );

	m_renderInfo.SetViewMatrix( m_camera.GetMatrix() );

#if 1 // Straight render (no culling or depth sorting for transparencies)...
	for( auto && object : m_objectList )
	{
		object->Render( m_renderInfo );
	}

#else // Standard rendering... 
	std::map< Geometry::shared_ptr, std::vector< Object::shared_ptr > > listObjects; // Non-culled objects

	// Build Render list...
	float fDistance;

	// Create the view frustum...
	Frustum frustum;
	unify::Matrix view, projection;
	
	core::Game & game = *core::Game::GetInstance();

	game.GetTransformManager()->CopyTransformFromTo( Transform::Index::View, view );
	game.GetTransformManager()->CopyTransformFromTo( Transform::Index::Projection, projection );

	win::DX::GetDxDevice()->SetVertexShaderConstantF( 4, (float*)&view, 4 );

	frustum.Calculate( view, projection );

	m_lastCullCount = 0;

	// Cull against the frustum...
	for( ObjectList::iterator itrObject = m_objectList.begin(); itrObject != m_objectList.end(); ++itrObject )
	{
		Object::shared_ptr object = *itrObject;

		// Check if object want's to be rendered...
		if( ! object->GetGeometry() || ! object->GetRender() ) continue;

		// Compute distance from object to camera
		fDistance = object->GetFrame()->GetPosition()->DistanceTo( *m_currentCamera->GetFrame()->GetPosition() );

		// Cull against frustum...
		if( m_cullingEnabled )
		{
			// This should be stored else where... (probably keep the ID of the screen when computed in both geo and physics (and use physics).
			unify::BBox< float > bboxTrans = object->GetGeometry()->GetBBox();
			object->GetFrame()->GetFinalMatrix().TransformCoord( bboxTrans.inf );
			object->GetFrame()->GetFinalMatrix().TransformCoord( bboxTrans.sup );

			if( CS_OUTSIDE == frustum.CullBBox( &bboxTrans ) )
			{
				m_lastCullCount++;
				continue;
			}
		}

		listObjects[ object->GetGeometry() ].push_back( object );
	}

	// Backup current states...
	unsigned int dwInitState[2];
	dwInitState[0] = RenderState::Get( RenderState::Lighting );
	dwInitState[1] = RenderState::Get( RenderState::CullMode );

	// Set initial states...
	unsigned int stateCatchList[ STATE_COUNT ];
	stateCatchList[ STATE_LIGHTING ] = m_defaultLighting;
	stateCatchList[ STATE_ZWRITEENABLE ] = m_defaultZWriteEnable;

	RenderState::Set( RenderState::Lighting, stateCatchList[STATE_LIGHTING] );
	RenderState::Set( RenderState::CullMode, stateCatchList[STATE_CULLMODE] );
	RenderState::Set( RenderState::ZWriteEnable, stateCatchList[STATE_ZWRITEENABLE] );

	// SOLIDS...
	RenderState::Set( RenderState::ZWriteEnable, true );
	if( m_renderSolids )
	{
		for( std::map< Geometry::shared_ptr, std::vector< Object::shared_ptr > >::iterator itrList = listObjects.begin(); itrList != listObjects.end(); ++itrList )
		{
			Geometry::shared_ptr geometry = itrList->first;

			// Allow reduced effect setting...
			renderInfo.SetOption( RenderOption::SolidOnly, true );
			renderInfo.SetOption( RenderOption::FrameCheck, false ); // Since we are rendering twice (solids, and trans) ignore frame check.

			// TODO: Should allow setting the geometry once here

			for( std::vector< Object::shared_ptr >::iterator itrObject = itrList->second.begin(); itrObject < itrList->second.end(); ++itrObject )
			{
				(*itrObject)->Render( m_renderInfo );
			}
		}
	}

	// TRANS...
	RenderState::Set( RenderState::ZWriteEnable, false );
	if( m_renderTrans )
	{
		renderInfo.SetOption( RenderOption::TransOnly, true );
		renderInfo.SetOption( RenderOption::FrameCheck, false ); // Because we are rendering twice (solid, trans).
		for( std::map< Geometry::shared_ptr, std::vector< Object::shared_ptr > >::iterator itrList = listObjects.begin(); itrList != listObjects.end(); ++itrList )
		{
			for( unsigned int u = 0; u < itrList->second.size(); u++ )
			{
				itrList->second[ u ]->Render( m_renderInfo );
			}
		}
	}

	// TODO: Commented out for now as probablly unnecessary and more costly than not doing so.
	// Clean the effects left by the RENDER_RETAINEFFECT flag...
	//PixelShader::DisuseShader();
	//VertexShader::DisuseShader();

	// Restore states...
	RenderState::Set( RenderState::Lighting, dwInitState[0] );
	RenderState::Set( RenderState::CullMode, dwInitState[1] );
	//RenderState::Set( RenderState::ZWriteEnable, dwInitState[2] );

#endif
	if( m_renderPhysics && m_physicsScene )
	{
		m_physicsScene->Render();
	}

	Game().GetOS().GetRenderer()->SetViewport( viewportBackup );

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
	m_viewport.SetWidth( size.width );
	m_viewport.SetHeight( size.height );
}

unify::Size< float > Scene::GetSize() const
{
	return unify::Size< float >( m_viewport.GetWidth(), m_viewport.GetHeight() );
}

void Scene::SetPosition( const unify::V2< float > & position )
{
	m_viewport.SetTopLeftX( position.x );
	m_viewport.SetTopLeftY( position.y );
}

unify::V2< float > Scene::GetPosition() const
{
	return unify::V2< float >( m_viewport.GetTopLeftX(), m_viewport.GetTopLeftY() );
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

void Scene::SetVisible( bool visible )
{
    m_visible = visible;
}

bool Scene::GetVisible() const
{
    return m_visible;
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

Object::shared_ptr Scene::GetObjectOver() const
{
    return m_objectOver;
}

void Scene::SetObjectOver( Object::shared_ptr objectOver )
{
    m_objectOver = objectOver;
}
