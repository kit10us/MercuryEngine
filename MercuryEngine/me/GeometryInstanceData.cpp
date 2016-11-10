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

std::map< std::string, unify::Any > & GeometryInstanceData::GetAttributes()
{
	return m_attributes;
}

const std::map< std::string, unify::Any > & GeometryInstanceData::GetAttributes() const
{
	return m_attributes;
}
