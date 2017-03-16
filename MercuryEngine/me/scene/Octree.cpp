// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/Octree.h>
#include <list>

using namespace me;	 
using namespace scene;
using namespace object;

Octree::Octree( Scene * scene, size_t max )
	: m_scene{ scene }
	, m_max{ max }
	, m_stacks{ std::shared_ptr< ObjectStack >( new ObjectStack( scene, max ) ) }
{	
}

Octree::~Octree()
{
}

size_t Octree::Count() const
{
	size_t count = 0;
	for( auto && stack : m_stacks )
	{
		count += stack->Count();
	}
	return count;
}

bool Octree::Available() const
{
	return true;
}

Object * Octree::NewObject( std::string name )
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

bool Octree::DestroyObject( Object * object )
{
	for( auto && stack : m_stacks )
	{
		if ( stack->DestroyObject( object ) ) return true;
	}
	return false;
}

Object * Octree::CopyObject( Object * from, std::string name ) 
{
	Object * newObject = NewObject( name );
	newObject->CopyFrom( name, *from );
	return newObject;
}

void Octree::CollectObjects( std::vector< Object * > & objects )
{
	for( auto && stack : m_stacks )
	{
		stack->CollectObjects( objects );
	}
}

Object * Octree::FindObject( std::string name )
{
	Object * object = nullptr;
	for( auto && stack : m_stacks )
	{
		object = stack->FindObject( name );
		if ( object ) break;
	}
	return object;
}

Object * Octree::GetObject( size_t index )
{
	// Find stack
	for ( auto && stack : m_stacks )
	{
		if ( index >= stack->Count() )
		{
			index -= stack->Count();
			continue;
		}
		else
		{
			return stack->GetObject( index );
		}
	}

	return nullptr;
}


void Octree::Update( UpdateParams params )
{
	for( auto && stack : m_stacks )
	{
		stack->Update( params );
	}
}

void Octree::CollectCameras( RenderGirl & renderGirl )
{
	for( auto && stack : m_stacks )
	{
		stack->CollectCameras( renderGirl );
	}
}

void Octree::CollectRendering( RenderParams params, const FinalCamera & camera, GeometryCacheSummation & summation )
{
	for( auto && stack : m_stacks )
	{
		stack->CollectRendering( params, camera, summation );
	}
}

