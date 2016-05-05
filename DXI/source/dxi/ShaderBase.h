// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/DirectX.h>
#include <dxi/RenderInfo.h>
#include <unify/Path.h>
#include <d3dx9shader.h>

namespace dxi
{
	class ShaderBase
	{
	public:
		ShaderBase();
		ShaderBase( const std::string & entryPointName, const std::string & profile );
		virtual ~ShaderBase();

		// ::Resource...
		void Create();
		void Destroy();
		void Validate();
		void Invalidate();

		void CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );
		void CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile );
		virtual void Use( const RenderInfo * renderInfo ) = 0;
		std::string GetError();

		ID3DXConstantTable * GetConstantTable();

		static bool Version( unsigned int uV1, unsigned int uV2 );
		static void DisuseShader();

	protected:
		// Derived class creates the shader as it knows the correct creation methods.
		virtual void CreateThisShader() = 0;

		unify::Path m_filePath;
		std::string m_code;
		bool m_assembly;
		std::string m_entryPointName;
		std::string m_profile;
		std::string m_errorMessage;
		bool m_created;

		ID3DXBuffer * m_codeBuffer;
		ID3DXConstantTable * m_constantTable;
	};
}