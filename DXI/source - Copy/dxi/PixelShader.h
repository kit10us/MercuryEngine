// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/ShaderBase.h>

namespace dxi
{
	class PixelShader : public dxi::ShaderBase
	{
	public:
		typedef std::shared_ptr< PixelShader > shared_ptr;

		PixelShader();
		PixelShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );

		// ::ShaderBase::QResource...
		virtual void Destroy();

		// ::ShaderBase...
		virtual void Use( const RenderInfo * renderInfo );

		void SetTrans( bool bTrans );
		bool IsTrans();

		static void DisuseShader();

	protected:
		// ::ShaderBase...
		void CreateThisShader();

		bool m_isTrans;	// Does this pixel shader turn the render into transparent (in part or entire)

		IDirect3DPixelShader9 * m_shader;
	};
}