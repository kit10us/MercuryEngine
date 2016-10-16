// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>
#include <dxi/core/IRenderer.h>
#include <unify/Path.h>

namespace dxi
{
	class PixelShader
	{
	public:
		typedef std::shared_ptr< PixelShader > ptr;

		PixelShader( core::IRenderer * renderer );
		PixelShader( core::IRenderer * renderer, const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );

		~PixelShader();

		void Destroy();

		void CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );

		void CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile );

		void Create();

		void Use( const RenderInfo & renderInfo );

		void SetTrans( bool bTrans );

		bool IsTrans();

		std::string GetError();

	protected:

		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;

		bool m_isTrans;	// Does this pixel shader turn the render into transparent (in part or entire)

		class Pimpl;
		std::shared_ptr< Pimpl > m_pimpl;
	};
}