// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/SkeletalGeometry.h>

using namespace me;
using namespace render;

/// <summary>
/// Owns both the geometry and the skeleton instance data.
/// </summary>
struct SkeletalGeometryInstanceData
{

};


GeometryInstanceData::ptr SkeletalGeometry::CreateInstanceData() const
{
	return {};
}

void 
SkeletalGeometry::Update( const UpdateParams & params, GeometryInstanceData * instanceData )
{
	/*
	if (m_skeleton)
	{
		m_skeleton->Update(params, instanceData);
	}
	*/
}

void SkeletalGeometry::Render( const Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed )
{
	if (m_geometry)
	{
		m_geometry->Render(params, instanceData, matrixFeed);
	}

	/* TODO: Right now it is not the same parameters, and the actual parameters are transient/hand-rolled (which is not good!)
	if (m_skeleton)
	{
		m_skeleton->Render(params, instanceData, matrixFeed);
	}
	*/
}

bool SkeletalGeometry::IsTrans() const 
{
	if (!m_geometry)
	{
		return false;
	}

	return m_geometry->IsTrans();
}