// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/GeometryComponent.h>
#include <me/scene/Object.h>

using namespace me;
using namespace scene;

GeometryComponent::GeometryComponent( GeometryComponent & component )
	: ObjectComponent( component )
	, m_geometry( component.m_geometry )
	, m_geometryInstanceData( component.m_geometry->CreateInstanceData() )
	, m_matrix( component.m_matrix )
{
}

GeometryComponent::GeometryComponent( IOS * os )
	: ObjectComponent( os )
	, m_matrix( unify::MatrixIdentity() )
{
}

GeometryComponent::GeometryComponent( IOS * os, Geometry::ptr geometry )
	: ObjectComponent( os )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
	, m_matrix( unify::MatrixIdentity() )
{
}

GeometryComponent::~GeometryComponent()
{
}

std::string GeometryComponent::GetName() const
{
	return "Geometry";
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

void GeometryComponent::SetModelMatrix( const unify::Matrix & matrix )
{
	m_matrix = matrix;
}

unify::Matrix & GeometryComponent::GetMatrix()
{
	return m_matrix;
}

const unify::Matrix & GeometryComponent::GetMatrix() const
{
	return m_matrix;
}

void GeometryComponent::OnInit( Object * object )
{
}

void GeometryComponent::OnStart( Object * object )
{
}

void GeometryComponent::OnUpdate( Object * object, const RenderInfo & renderInfo )
{
	m_geometry->Update( renderInfo, m_geometryInstanceData.get() );
}

void GeometryComponent::OnRender( Object * object, const RenderInfo & renderInfo )
{
	RenderInfo myRenderInfo( renderInfo );
	myRenderInfo.SetWorldMatrix( m_matrix * myRenderInfo.GetWorldMatrix() );
	m_geometry->Render( myRenderInfo, m_geometryInstanceData.get() );
}

void GeometryComponent::OnSuspend( Object * object )
{
}

void GeometryComponent::OnResume( Object * object )
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
