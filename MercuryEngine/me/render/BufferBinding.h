// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	namespace render
	{
		struct ConstantBufferBinding
		{
			enum TYPE
			{
				/// <summary>
				/// 
				/// </summary>
				VertexBuffer,

				/// <summary>
				/// 
				/// </summary>
				IndexBuffer,

				/// <summary>
				/// 
				/// </summary>
				ConstantBuffer,

				/// <summary>
				/// 
				/// </summary>
				ShaderResource,

				/// <summary>
				/// 
				/// </summary>
				StreamOutput,

				/// <summary>
				/// 
				/// </summary>
				RenderTarget,

				/// <summary>
				/// 
				/// </summary>
				DepthStencil,

				/// <summary>
				/// 
				/// </summary>
				UnorderedAccess,

				/// <summary>
				/// 
				/// </summary>
				Decoder,

				/// <summary>
				/// 
				/// </summary>
				VideoEncoder
			};
			static TYPE FromString( std::string usage );
			static std::string ToString( BufferBinding::TYPE usage );
		};
	}
}