// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/ObjectGroup.h>

using namespace dxi;
using namespace scene;

ObjectGroup::ObjectGroup()
{
}

ObjectGroup::~ObjectGroup()
{
}

void ObjectGroup::Add( Object::shared_ptr object )
{
    m_objects.push_back( object );
}

size_t ObjectGroup::Count() const
{
    return m_objects.size();
}

void ObjectGroup::Clear()
{
    m_objects.clear();
}

void ObjectGroup::ForEach( ObjectGroup::IObjectFunctor & functor )
{
    for ( size_t index = 0, count = m_objects.size(); index != count; ++index )
    {
        functor( *(m_objects[ index ].get()), index, count );
    }
}

void ObjectGroup::Update( unify::Seconds elapsed, core::IInput & input )
{
    for ( size_t index = 0, count = m_objects.size(); index != count; ++index )
    {
        m_objects[ index ]->Update( elapsed, input );
    }
}

void ObjectGroup::Render( RenderInfo renderInfo )
{
    for ( size_t index = 0, count = m_objects.size(); index != count; ++index )
    {
        m_objects[ index ]->Render( renderInfo );
    }
}
