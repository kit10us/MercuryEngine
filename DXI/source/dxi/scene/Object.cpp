// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Object.h>
#include <stack>

using namespace dxi;
using namespace scene;

Object::Object( core::IOS * os )
	: Object( os, 0 )
{
}

Object::Object( core::IOS * os, Scene * scene )
	: m_os( os )
	, m_enabled( true )
	, m_selectable( false )
	, m_checkFrame( true )
	, m_lastFrameID( 0 )
	, m_scene( scene )
{
}			  

Object::Object( core::IOS * os, Geometry::ptr geometry, const unify::V3< float > position )
: m_os( os )
, m_enabled( true )
{
	AddComponent( scene::IObjectComponent::ptr( new GeometryComponent( os, geometry ) ) );
	GetFrame().SetPosition( position );
}

Object::Object( const Object & object, std::string name )
	: m_os ( object.m_os )
	, m_enabled( object.m_enabled )
	, m_selectable( object.m_selectable )
	, m_checkFrame( object.m_checkFrame )
	, m_lastFrameID( 0 )
	, m_scene( object.m_scene )
	, m_name( name )
	, m_frame( object.m_frame )
	, m_tags( object.m_tags )
{
	for( auto component : object.m_components )
	{
		AddComponent( scene::IObjectComponent::ptr( component.c->Duplicate() ) );
	}
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

void Object::AddTag( std::string tag )
{
	m_tags.push_back( tag );
	m_tags.sort();
}

bool Object::HasTag( std::string tag ) const
{	
	for( auto && item : m_tags )
	{		  
		if ( unify::StringIs( item, tag ) ) return true;
	}
	return false;
}

int Object::ComponentCount() const
{
	return (int)m_components.size();
}

void Object::AddComponent( IObjectComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( ComponentHolder( component ) );
}

void Object::RemoveComponent( IObjectComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

IObjectComponent::ptr Object::GetComponent( int index )
{
	if( index > (int)m_components.size() ) return IObjectComponent::ptr();

	int i = 0;
	for( auto component : m_components )
	{
		if( index == i ) return component.c;
		++i;
	}

	assert( 0 );
	return IObjectComponent::ptr(); // Should never hit here.
}

IObjectComponent::ptr Object::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if( index == -1 ) return IObjectComponent::ptr();
	return GetComponent( index );
}
	  
int Object::FindComponent( std::string name, int startIndex ) const
{
	int i = 0;
	for( auto component : m_components )
	{
		if( i >= startIndex && unify::StringIs( component.c->GetName(), name ) ) return i;
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
	AddComponent( scene::IObjectComponent::ptr( new GeometryComponent( m_os, geometry ) ) );
}

unify::Matrix & Object::GetGeometryMatrix()
{
	scene::GeometryComponent * geometry = dynamic_cast< scene::GeometryComponent *>( GetComponent( "Geometry", 0 ).get() );
	return geometry->GetModelMatrix();
}

void Object::Update( const RenderInfo & renderInfo )
{
    // Do not update if we are not enabled.
    if ( ! m_enabled )
    {
        return;
    }

	// Update components...
	for( auto && component : m_components )
	{
		// Regardless of enabled, ensure OnInit is always called.
		if ( component.initDone == false )
		{
			component.c->OnInit( this );
			component.initDone = true;
		}

		// Only start and update if enabled.
		if ( !component.c->IsEnabled() ) continue;

		// Start is basically a way to get us into a beginning state.
		if ( component.startDone == false )
		{
			component.c->OnStart( this );
			component.startDone = true;
		}

		component.c->OnUpdate( this, renderInfo );
	}

	if( GetFirstChild() )
	{
		GetFirstChild()->Update( renderInfo );
	}

	if ( GetNext() )
	{
		GetNext()->Update( renderInfo );
	}
}

void Object::RenderSimple( const RenderInfo & renderInfo )
{
	// Update components...
	for( auto && component : m_components )
	{
		// Don't try rendering if we haven't been properly setup...
		if ( !component.initDone || !component.startDone || !component.c->IsEnabled() ) continue;

		component.c->OnRender( this, renderInfo );
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
		if( component.c->IsEnabled() )
		{
			component.c->OnSuspend( this );
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
		if( component.c->IsEnabled() )
		{
			component.c->OnResume( this );
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
		m_firstChild.reset( new Object( m_os, m_scene ) );
		lastChild = m_firstChild;
	}
	else
	{
		// Find our last child...
		while( lastChild->GetNext() )
		{
			lastChild = lastChild->GetNext();
		}
		lastChild->m_next.reset( new Object( m_os, m_scene ) );
		lastChild->m_next->m_previous = lastChild;
		lastChild = lastChild->GetNext();	 
	}
	lastChild->m_parent.reset( this );
	
	// Last child is new child...
	lastChild->SetName( name );
	return lastChild;
}

Object::ptr Object::Duplicate( std::string name )
{
	Object::ptr duplicate( new Object(*this, name) );

	Object::ptr lastSibling = m_parent->GetFirstChild();

	// Find our last sibling...
	while ( lastSibling->GetNext() )
	{
		lastSibling = lastSibling->GetNext();
	}

	lastSibling->m_next = duplicate;
	duplicate->m_previous = lastSibling;
	duplicate->m_parent = m_parent;

	return duplicate;

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


std::list< Object * > Object::AllChildren( bool recursive )
{
	std::list< Object * > objects;

	std::stack< Object * > stack;
	stack.push( this );

	while( ! stack.empty() )
	{	
		Object * object = stack.top(); 
		stack.pop();

		objects.push_back( object );
		
		if ( object->GetNext() ) stack.push( object->GetNext().get() );
		if ( object->GetFirstChild() ) stack.push( object->GetFirstChild().get() );
	}

	return objects;
}

Scene * Object::GetScene()
{
	return m_scene;
}
