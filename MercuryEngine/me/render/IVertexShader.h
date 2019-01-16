// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/IShader.h>
#include <me/render/VertexDeclaration.h>
#include <unify/Path.h>
#include <memory>

namespace me 
{
	namespace render
	{
		struct VertexShaderParameters
		{
			VertexShaderParameters()
				: trans{ false }
			{
			}

			unify::Path path;
			std::string code;
			std::string entryPointName;
			std::string profile;
			render::ConstantBufferParameters constantBufferParameters;
			VertexDeclaration::ptr vertexDeclaration;
			bool trans;
		};

		class IVertexShader : public render::IShader
		{
		public:
			typedef std::shared_ptr< IVertexShader > ptr;

			virtual ~IVertexShader() {}
			
			/// <summary>
			/// Set our vertex declaration.
			/// </summary>
			virtual void SetVertexDeclaration( me::render::VertexDeclaration::ptr vertexDeclaration ) = 0;

			/// <summary>
			/// Get our pixel description.
			/// </summary>
			virtual VertexDeclaration::ptr GetVertexDeclaration() const = 0;
		};
	}
}