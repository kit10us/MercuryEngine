// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <medx9/Renderer.h>
#include <me/IVertexConstruct.h>
#include <me/VertexDeclaration.h>
#include <unify/unify.h>
#include <unify/String.h>
#include <unify/DataLock.h>
#include <atlbase.h>
#include <map>

namespace medx9
{
	/// <summary> 
	/// Defines the structure of the data in a vertex. 
	/// </summary>
	class VertexConstruct : public me::IVertexConstruct
	{
	public:
		VertexConstruct( const me::IRenderer * renderer, const me::VertexDeclaration & vd );
		~VertexConstruct();

		void Use() const override;

	private:
		const Renderer * m_renderer;
		CComPtr< IDirect3DVertexDeclaration9 > m_layout;
	};
}