// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/GeometryComponent.h>

using namespace dxi;
using namespace scene;

GeometryComponent::GeometryComponent( core::IOS * os )
	: ObjectComponent( os )
	, m_modelMatrix( unify::Matrix::MatrixIdentity() )
{
}

GeometryComponent::GeometryComponent( core::IOS * os, Geometry::ptr geometry )
	: ObjectComponent( os )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
	, m_modelMatrix( unify::Matrix::MatrixIdentity() )
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
	m_modelMatrix = matrix;
}

unify::Matrix & GeometryComponent::GetModelMatrix()
{
	return m_modelMatrix;
}

const unify::Matrix & GeometryComponent::GetModelMatrix() const
{
	return m_modelMatrix;
}

void GeometryComponent::OnInit( Object * object )
{
}

void GeometryComponent::OnStart( Object * object )
{
}

void GeometryComponent::OnUpdate( const RenderInfo & renderInfo )
{
	m_geometry->Update( renderInfo, m_geometryInstanceData.get() );
}

void GeometryComponent::OnRender( const RenderInfo & renderInfo )
{
	RenderInfo myRenderInfo( renderInfo );
	myRenderInfo.SetWorldMatrix( m_modelMatrix * myRenderInfo.GetWorldMatrix() );
	m_geometry->Render( myRenderInfo, m_geometryInstanceData.get() );
}

void GeometryComponent::OnSuspend()
{
}

void GeometryComponent::OnResume()
{
}
