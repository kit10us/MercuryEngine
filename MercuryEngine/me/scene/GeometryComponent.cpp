// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/GeometryComponent.h>
#include <me/scene/Object.h>
#include <me/GeometryCache.h>

using namespace me;
using namespace scene;

GeometryComponent::GeometryComponent( GeometryComponent & component )
	: ObjectComponent( component )
	, m_geometry( component.m_geometry )
	, m_geometryInstanceData( component.m_geometry->CreateInstanceData() )
{
}

GeometryComponent::GeometryComponent( IOS * os )
	: ObjectComponent( os )
{
}

GeometryComponent::GeometryComponent( IOS * os, Geometry::ptr geometry )
	: ObjectComponent( os )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
{
}

GeometryComponent::~GeometryComponent()
{
}

std::string GeometryComponent::GetType() const
{
	return "Geometry";
}

std::string GeometryComponent::GetWhat() const
{
	return std::string();
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

void GeometryComponent::OnInit()
{
}

void GeometryComponent::OnStart()
{
}

void GeometryComponent::OnUpdate( UpdateParams params )
{
	m_geometry->Update( params, m_geometryInstanceData.get() );
}

void GeometryComponent::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
	cache.Add( m_geometry.get(), frame );
}

void GeometryComponent::OnSuspend()
{
}

void GeometryComponent::OnResume()
{
}

IObjectComponent * GeometryComponent::Duplicate()
{
	auto duplicate = new GeometryComponent( *this );
	return duplicate;
}

GeometryComponent * me::AddGeometryComponent( Object * object, IOS * os, Geometry::ptr geometry )
{
	GeometryComponent * component = new GeometryComponent( os, geometry );
	object->AddComponent( IObjectComponent::ptr( component ) );
	return component;
}
