// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/GeometryInstanceData.h>

using namespace me;

GeometryInstanceData::GeometryInstanceData()
{
}

GeometryInstanceData::~GeometryInstanceData()
{
}

void GeometryInstanceData::Update( unify::Seconds elapsed )
{
	elapsed;
}

void GeometryInstanceData::SetMatrix( unify::Matrix matrix )
{
	m_isMatrixSet = true;
	m_matrix = matrix;
}

unify::Matrix GeometryInstanceData::GetMatrix() const
{
	return m_matrix;
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