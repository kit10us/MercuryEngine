// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/GeometryComponent.h>

using namespace dxi;
using namespace scene;

GeometryComponent::GeometryComponent()
	: Component( "Geometry" )
	, m_modelMatrix( unify::Matrix::MatrixIdentity() )
{
}

GeometryComponent::GeometryComponent( Geometry::ptr geometry )
	: Component( "Geometry" )
	, m_geometry( geometry )
	, m_geometryInstanceData( geometry->CreateInstanceData() )
	, m_modelMatrix( unify::Matrix::MatrixIdentity() )
{
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

void GeometryComponent::OnStart()
{
}

void GeometryComponent::Update( const RenderInfo & renderInfo, core::IInput & input )
{
	m_geometry->Update( renderInfo, m_geometryInstanceData.get() );
}

void GeometryComponent::Render( const RenderInfo & renderInfo )
{
	RenderInfo myRenderInfo( renderInfo );
	myRenderInfo.SetWorldMatrix( m_modelMatrix * myRenderInfo.GetWorldMatrix() );
	m_geometry->Render( myRenderInfo, m_geometryInstanceData.get() );
}

void GeometryComponent::OnSuspend()
{
}

void GeometryComponent::OnResune()
{
}