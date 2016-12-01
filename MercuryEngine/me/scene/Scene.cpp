// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Frustum.h>
#include <map>

using namespace me;
using namespace scene;

Scene::Scene( IGame * game )
: m_game( game )
, m_root( new Object( this, "root" ) )
, m_inited( false )
, m_started( false )
, m_lastCullCount( 0 )
, m_order( 0.0f )
, m_enabled( true )
, m_renderObjects( true )
, m_renderSolids( true )
, m_renderTrans( true )
, m_cullingEnabled( true )
, m_defaultLighting( false )
, m_defaultZWriteEnable( true )
{
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

void Scene::Update( IRenderer * renderer, const RenderInfo & renderInfo )
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
			component->OnUpdate( this, renderer, renderInfo );
		}
	}		   

	// Object updating (animation, independant physics)...
	GetRoot()->Update( renderer, renderInfo );
}

struct FinalCamera
{
	Object::ptr object;
	unify::Matrix transform;
	CameraComponent * camera;
};

void Accumulate( std::list< RenderSet > & renderList, std::list< FinalCamera > & cameraList, Object::ptr current, IRenderer * renderer, const RenderInfo & renderInfo, unify::Matrix parentTransform )
{
	assert( current );
	assert( current->IsEnabled() );

	// Check for a camera...
	CameraComponent * camera{};
	for( int i = 0; i < current->ComponentCount(); ++i )
	{
		IObjectComponent::ptr component = current->GetComponent( i );
		if( !component->IsEnabled() ) continue;

		camera = dynamic_cast< CameraComponent * >(component.get());
		if( camera != nullptr )
		{
			cameraList.push_back( FinalCamera{ current, current->GetFrame().GetMatrix() * parentTransform, camera } );
		}
	}																		 

	current->CollectRenderables( renderList, renderer, renderInfo, parentTransform );

	// Handle children
	Object::ptr child = current->GetFirstChild();
	while( child )
	{
		if( child->IsEnabled() )
		{
			Accumulate( renderList, cameraList, child, renderer, renderInfo, current->GetFrame().GetMatrix() * parentTransform );
		}

		child = child->GetNext();
	}
}

void Scene::Render( IRenderer * renderer, const RenderInfo & renderInfo )
{
	// Render scene components
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnRender( this, renderer, renderInfo );
		}
	}

	if ( m_renderObjects == false )
	{
		return;
	}

	// Accumulate objects for rendering, and cameras.
	std::list< RenderSet > renderList;
	std::list< FinalCamera > cameraList;			 
	if( GetRoot() && GetRoot()->IsEnabled() )
	{
		Accumulate( renderList, cameraList, GetRoot(), renderer, renderInfo, unify::MatrixIdentity() );
	}

	if ( cameraList.empty() ) return;

	// Sort list by component...
	// TODO: Check if making all of these Geometry::ptr into Geometry* during rendering is faster (since there is no changing of reference counting or temp vars).
	std::map< Geometry::ptr, std::vector< unify::Matrix > > sorted;
	
	for( auto set : renderList )
	{
		sorted[ set.geo ].push_back( set.world );
	}

	for( auto camera : cameraList )
	{
		if( camera.camera->GetRenderer() != renderer->GetIndex() ) continue;

		RenderInfo myRenderInfo( renderInfo );
		myRenderInfo.SetViewMatrix( camera.transform.Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		for( auto pair : sorted )
		{
			pair.first->Render( renderer, myRenderInfo, 0, pair.second );
		}
	}
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

void Scene::SetRenderObjects( bool enabled )
{
	m_renderObjects = enabled;
}

bool Scene::GetRenderObjects() const
{
	return m_renderObjects;
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

