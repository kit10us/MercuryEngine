// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	namespace render
	{
		struct ConstantBufferType
		{
			enum TYPE
			{
				/// <summary>
				/// Used with pixel shaders.
				/// </summary>
				PixelShader,

				/// <summary>
				/// Used with vertex shaders.
				/// </summary>
				VertexShader,

				/// <summary>
				/// Used with compute shaders.
				/// </summary>
				ComputeShader,

				/// <summary>
				/// Used with domain shaders.
				/// </summary>
				DomainShader,

				/// <summary>
				/// Used with geometry shaders.
				/// </summary>
				GeometryShader
			};

			static TYPE FromString( std::string usage );
			static std::string ToString( ConstantBufferType::TYPE usage );
		};
	}
}