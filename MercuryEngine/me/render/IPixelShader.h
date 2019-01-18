// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/IShader.h>
#include <me/render/Blend.h>
#include <unify/Path.h>
#include <memory>

namespace me 
{
	namespace render
	{
		struct PixelShaderParameters
		{
			PixelShaderParameters()
				: blendDesc{}
				, trans{ false }
			{
			}

			unify::Path path;
			std::string code;
			std::string entryPointName;
			std::string profile;
			render::ConstantTable constantTable;
			BlendDesc blendDesc;
			bool trans;
		};

		class IPixelShader : public render::IShader
		{
		public:
			typedef std::shared_ptr< IPixelShader > ptr;

			virtual ~IPixelShader() {}

			virtual BlendDesc GetBlendDesc() const = 0;
		};
	}
}