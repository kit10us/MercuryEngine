// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx11/Renderer.h>
#include <me/VertexDeclaration.h>
#include <me/IVertexShader.h>
#include <atlbase.h>

namespace medx11
{

	/// <summary> 
	/// Defines the structure of the data in a vertex. 
	/// </summary>
	class VertexConstruct : public me::IVertexConstruct
	{
	public:
		VertexConstruct( const me::IRenderer * renderer, const me::VertexDeclaration & vd, const me::IVertexShader & vs );
		~VertexConstruct();
		
		void Use() const override;

	private:
		const Renderer * m_renderer;
		CComPtr< ID3D11InputLayout > m_layout;
	};
}