// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/GeometryComponent.h>
#include <me/object/Object.h>
#include <me/GeometryCache.h>

using namespace me;
using namespace object;

GeometryComponent::GeometryComponent( GeometryComponent & component )
	: ObjectComponent( component )
	, m_geometry( component.m_geometry )
	, m_geometryInstanceData( component.m_geometry->CreateInstanceData() )
{
}

GeometryComponent::GeometryComponent()
	: ObjectComponent( "Geometry", false, true )
{
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry )
	: ObjectComponent( "Geometry", false, true )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
{
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry, unify::Matrix matrix )
	: ObjectComponent( "Geometry", false, true )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
{
	m_geometryInstanceData->SetMatrix( matrix );
}

GeometryComponent::~GeometryComponent()
{
}

void GeometryComponent::SetGeometry( Geometry::ptr geometry )
{
	m_geometry = geometry;
	m_geometryInstanceData.reset( geometry->CreateInstanceData() );
}

Geometry::ptr GeometryComponent::GetGeometry()
{
	return m_geometry;
}

const Geometry::ptr GeometryComponent::GetGeometry() const
{
	return m_geometry;
}

void GeometryComponent::SetMatrix( unify::Matrix matrix )
{
	m_geometryInstanceData->SetMatrix( matrix );
}

unify::Matrix GeometryComponent::GetMatrix() const
{
	return *m_geometryInstanceData->GetMatrix();
}

void GeometryComponent::OnStart()
{
}

void GeometryComponent::OnUpdate( UpdateParams params )
{
	m_geometry->Update( params, m_geometryInstanceData.get() );
}

void GeometryComponent::CollectGeometry( GeometryCache & solids, GeometryCache & trans, const unify::FrameLite * frame )
{
	if( m_geometry->IsTrans() )
	{
		trans.Add( m_geometry.get(), { frame, m_geometryInstanceData->GetMatrix() } );
	}
	else
	{
		solids.Add( m_geometry.get(), { frame, m_geometryInstanceData->GetMatrix() } );
	}
}

IObjectComponent::ptr GeometryComponent::Duplicate()
{
	auto duplicate = new GeometryComponent( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}

namespace me
{
	GeometryComponent * AddGeometryComponent( Object * object, Geometry::ptr geometry, unify::Matrix matrix )
	{
		GeometryComponent * component = new GeometryComponent( geometry, matrix );
		object->AddComponent( IObjectComponent::ptr( component ) );
		return component;
	}
}
