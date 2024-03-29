// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/GeometryInstanceData.h>

using namespace me;
using namespace render;

GeometryInstanceData::GeometryInstanceData()
{
}

GeometryInstanceData::~GeometryInstanceData()
{
}

void GeometryInstanceData::Update( unify::TimeDelta elapsed )
{
	// Do nothing.
	elapsed;
}

void GeometryInstanceData::SetMatrix( unify::Matrix matrix )
{
	m_isMatrixSet = true;
	m_matrix = matrix;
}

const unify::Matrix * GeometryInstanceData::GetMatrix() const
{
	return &m_matrix;
}

bool GeometryInstanceData::IsMatrixSet() const
{
	return m_isMatrixSet;
}

void GeometryInstanceData::ResetMatrix()
{
	m_matrix = unify::MatrixIdentity();
	m_isMatrixSet = false;
}