// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/GrowableObjectstack.h>
#include <list>

using namespace me;
using namespace scene;
using namespace object;
using namespace render;

GrowableObjectStack::GrowableObjectStack( IScene * scene, size_t max )
	: ObjectAllocator( "GrowableObjectStack" )
	, m_scene{ scene }
	, m_max{ max }
	, m_stacks{ std::shared_ptr< ObjectStack >( new ObjectStack( scene, max ) ) }
{	
	AddInterface( "GrowableObjectStack", this );
}

GrowableObjectStack::~GrowableObjectStack()
{
}

size_t GrowableObjectStack::Count() const
{
	size_t count = 0;
	for( auto && stack : m_stacks )
	{
		count += stack->Count();
	}
	return count;
}

bool GrowableObjectStack::Available() const
{
	return true;
}

Object * GrowableObjectStack::NewObject( std::string name )
{
	for( auto && stack : m_stacks )
	{
		if ( stack->Available() ) return stack->NewObject( name );
	} 

	// Need to grow...
	std::shared_ptr< ObjectStack > newStack( new ObjectStack( m_scene, m_max ) );
	m_stacks.push_back( newStack );

	return newStack->NewObject( name );
}

bool GrowableObjectStack::DestroyObject( Object * object )
{
	for( auto && stack : m_stacks )
	{
		if ( stack->DestroyObject( object ) ) return true;
	}
	return false;
}

Object * GrowableObjectStack::CopyObject( Object * from, std::string name ) 
{
	Object * newObject = NewObject( name );
	newObject->CopyFrom( name, *from );
	return newObject;
}

void GrowableObjectStack::CollectObjects( std::vector< Object * > & objects )
{
	for( auto && stack : m_stacks )
	{
		stack->CollectObjects( objects );
	}
}

Object * GrowableObjectStack::FindObject( std::string name )
{
	Object * object = nullptr;
	for( auto && stack : m_stacks )
	{
		object = stack->FindObject( name );
		if ( object ) break;
	}
	return object;
}

void GrowableObjectStack::DirtyObject( object::Object * object )
{
	assert( "We don't own objects!" == 0 );
}

void GrowableObjectStack::Update( const UpdateParams & params )
{
	for( auto && stack : m_stacks )
	{
		stack->Update( params );
	}
}

void GrowableObjectStack::CollectCameras( RenderGirl & renderGirl ) 
{
	for( auto && stack : m_stacks )
	{
		stack->CollectCameras( renderGirl );
	}
}

void GrowableObjectStack::CollectRendering( render::Params params, const FinalCamera & camera, GeometryCacheSummation & solids, GeometryCacheSummation & trans )
{
	for( auto && stack : m_stacks )
	{
		stack->CollectRendering( params, camera, solids, trans );
	}
}

