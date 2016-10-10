// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Object.h>
#include <dxi/core/Game.h>

using namespace dxi;
using namespace scene;

Object::Object()
: m_enabled( true )
, m_selectable( false )
, m_checkFrame( true )
, m_lastFrameID( 0 )
{
}

Object::Object( Geometry::ptr geometry, std::shared_ptr< physics::IInstance > physics )
: m_enabled( true )
{
	AddComponent( scene::IComponent::ptr( new GeometryComponent( geometry ) ) );
}

Object::Object( Geometry::ptr geometry, const unify::V3< float > position )
: m_enabled( true )
{
	AddComponent( scene::IComponent::ptr( new GeometryComponent( geometry ) ) );
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

std::map< std::string, std::string > & Object::GetTags()
{
	return m_tags;
}

const std::map< std::string, std::string > & Object::GetTags() const
{
	return m_tags;
}

int Object::ComponentCount() const
{
	return (int)m_components.size();
}

void Object::AddComponent( IComponent::ptr component )
{
	m_components.push_back( component );
}

void Object::RemoveComponent( IComponent::ptr component )
{
	m_components.remove( component );
}

IComponent::ptr Object::GetComponent( int index )
{
	if( index > (int)m_components.size() ) return IComponent::ptr();

	int i = 0;
	for( auto component : m_components )
	{
		if( index == i ) return component;
		++i;
	}

	assert( 0 );
	return IComponent::ptr(); // Should never hit here.
}

IComponent::ptr Object::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if( index == -1 ) return IComponent::ptr();
	return GetComponent( index );
}
	  
int Object::FindComponent( std::string name, int startIndex ) const
{
	int i = 0;
	for( auto component : m_components )
	{
		if( i >= startIndex && unify::StringIs( component->GetName(), name ) ) return i;
		++i;
	}		
	return -1;
}				
	   
void Object::SetEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool Object::IsEnabled() const
{
    return m_enabled;
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

void Object::SetGeometry( Geometry::ptr geometry )
{
	AddComponent( scene::IComponent::ptr( new GeometryComponent( geometry ) ) );
}

unify::Matrix & Object::GetGeometryMatrix()
{
	scene::GeometryComponent * geometry = dynamic_cast< scene::GeometryComponent *>( GetComponent( "Geometry" ).get() );
	return geometry->GetModelMatrix();
}

controllers::IController::shared_ptr Object::GetController()
{
	return m_controller;
}

void Object::SetController( controllers::IController::shared_ptr controller )
{
	m_controller = controller;
}

void Object::OnStart()
{
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnStart();
		}
	}

	if ( GetFirstChild() )
	{
		GetFirstChild()->OnStart();		
	}

	Object::ptr sibling = GetNext();
	while( sibling )
	{
		sibling->OnStart();
		sibling = sibling->GetNext();
	}	
}

void Object::Update( const RenderInfo & renderInfo, core::IInput & input )
{
    // Do not update if we are not enabled.
    if ( ! m_enabled )
    {
        return;
    }

	// Update components...
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->Update( renderInfo, input );
		}
	}

	if( GetFirstChild() )
	{
		GetFirstChild()->Update( renderInfo, input );
	}

	if ( GetNext() )
	{
		GetNext()->Update( renderInfo, input );
	}
}

void Object::RenderSimple( const RenderInfo & renderInfo )
{
	// Update components...
	for( auto && component : m_components )
	{
		if ( component->IsEnabled() )
		{
			component->Render( renderInfo );
		}
	}
}

void Object::RenderHierarchical( const RenderInfo & renderInfo )
{	
	// Render self and children...
	RenderInfo myRenderInfo( renderInfo );

	myRenderInfo.SetWorldMatrix( m_frame.GetMatrix() * myRenderInfo.GetWorldMatrix() );

	RenderSimple( renderInfo );

	if( GetFirstChild() )
	{
		GetFirstChild()->RenderHierarchical( myRenderInfo );
	}

	if( GetNext() )
	{
		GetNext()->RenderHierarchical( renderInfo );
	}
}

void Object::OnSuspend()
{
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnSuspend();
		}
	}

	if( GetFirstChild() )
	{
		GetFirstChild()->OnSuspend();
	}

	Object::ptr sibling = GetNext();
	while( sibling )
	{
		sibling->OnSuspend();
		sibling = sibling->GetNext();
	}
}

void Object::OnResume()
{
	for( auto && component : m_components )
	{
		if( component->IsEnabled() )
		{
			component->OnResune();
		}
	}

	if( GetFirstChild() )
	{
		GetFirstChild()->OnResume();
	}

	Object::ptr sibling = GetNext();
	while( sibling )
	{
		sibling->OnResume();
		sibling = sibling->GetNext();
	}
}

Object::ptr Object::GetPrevious()
{
	return m_previous;
}

const Object::ptr Object::GetPrevious() const
{
	return m_previous;
}

Object::ptr Object::GetNext()
{
	return m_next;
}

const Object::ptr Object::GetNext() const
{
	return m_next;
}

Object::ptr Object::GetParent()
{
	return m_parent;
}

const Object::ptr Object::GetParent() const
{
	return m_parent;
}

Object::ptr Object::GetFirstChild()
{
	return m_firstChild;
}

const Object::ptr Object::GetFirstChild() const
{
	return m_firstChild;
}

Object::ptr Object::AddChild( std::string name )
{
	Object::ptr lastChild = GetFirstChild();
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

Object::ptr Object::FindObject( std::string name )
{	
	Object::ptr child = GetFirstChild();
	while( child )
	{
		if ( unify::StringIs( child->GetName(), name ) )
		{
			return child;
		}
		Object::ptr found = child->FindObject( name );
		if ( found )
		{
			return found;
		}
		child = child->GetNext();
	}

	// Not found...
	return Object::ptr();
}
