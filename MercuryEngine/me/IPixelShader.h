// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>
#include <me/Mercury.h>
#include <unify/Path.h>
#include <memory>

namespace me 
{
	struct PixelShaderParameters
	{
		PixelShaderParameters()
		{
		}

		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
	};

	class IPixelShader
	{
	public:
		typedef std::shared_ptr< IPixelShader > ptr;

		virtual ~IPixelShader() {}

		virtual void SetTrans( bool bTrans ) = 0;

		virtual bool IsTrans() = 0;

		virtual void Use( const me::RenderInfo & renderInfo ) const = 0;
	};
}