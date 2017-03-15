// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mesh.h>
#include <me/factory/EffectFactories.h>

using namespace me;

// Describes a piece of ground.
class GroundDesc
{
public:
	typedef std::shared_ptr< GroundDesc > ptr;

	GroundDesc();
	~GroundDesc();

	void AddDefault( Geometry::ptr geometry );

	Geometry::ptr GetDefault( size_t i );

private:
	// Defaults are able to tile with other defaults.
	std::vector< Geometry::ptr > m_defaults;

};
