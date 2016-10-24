// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Scene.h>
#include <dxi/Frustum.h>
#include <map>

using namespace dxi;
using namespace scene;

Scene::Scene( dxi::core::IGame * game )
: m_game( game )
, m_root( new Object( game->GetOS(), this ) )
, m_inited( false )
, m_started( false )
, m_lastCullCount( 0 )
, m_renderSolids( true )
, m_renderTrans( true )
, m_cullingEnabled( true )
, m_defaultLighting( false )
, m_defaultZWriteEnable( true )
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
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnInit( this );
		}
	}
}

void Scene::OnStart()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnStart( this );
		}
	}
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

	for( auto && component : m_components )
	{
		if ( component->IsEnabled( ) )
		{
			component->OnUpdate( this, renderInfo );
		}
	}		   

	// Object updating (animation, independant physics)...
	GetRoot()->Update( renderInfo );
}

void Scene::Suspend()
{
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnSuspend();
		}
	}	

	GetRoot()->OnSuspend();
}

void Scene::Resume()
{
	GetRoot()->OnResume();

	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnResume();
		}
	}
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
	CameraComponent * camera;
};							  

void Accumulate( std::list< FinalObject > & renderList, std::list< FinalCamera > & cameraList, Object::ptr current, unify::Matrix parentTransform )
{
	assert( current );
	assert( current->IsEnabled() );

	// Solve our transform.
	unify::Matrix transform = current->GetFrame().GetMatrix(); 
	transform *= parentTransform;

	// Check for a camera...
	scene::CameraComponent * camera{};
	for( int i = 0; i < current->ComponentCount(); ++i )
	{
		IObjectComponent::ptr component = current->GetComponent( i );
		if( !component->IsEnabled() ) continue;

		camera = dynamic_cast< scene::CameraComponent * >( component.get() );
		if( camera != nullptr )
		{
			cameraList.push_back( FinalCamera{ current, transform, camera } );
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
	for ( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->OnRender( this, m_renderInfo );
		}
	}

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

Object::ptr Scene::GetObjectOver() const
{
    return m_objectOver;
}

void Scene::SetObjectOver( Object::ptr objectOver )
{
    m_objectOver = objectOver;
}																

int Scene::ComponentCount() const
{
	return (int)m_components.size();
}

void Scene::AddComponent( ISceneComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( component );
}

void Scene::RemoveComponent( ISceneComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

ISceneComponent::ptr Scene::GetComponent( int index )
{
	if ( index > (int)m_components.size() ) return ISceneComponent::ptr();

	int i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component;
		++i;
	}

	assert( 0 );
	return ISceneComponent::ptr(); // Should never hit here.
}

ISceneComponent::ptr Scene::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if ( index == -1 ) return ISceneComponent::ptr();
	return GetComponent( index );
}

int Scene::FindComponent( std::string name, int startIndex ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( i >= startIndex && unify::StringIs( component->GetName(), name ) ) return i;
		++i;
	}
	return -1;
}

