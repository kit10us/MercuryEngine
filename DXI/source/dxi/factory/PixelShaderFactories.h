// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/PixelShader.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class PixelShaderFactory : public rm::ISourceFactory< PixelShader >
	{
	public:
		PixelShader * Produce( unify::Path source ) override;
	};
}