// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/font/CharacterRender.h>
#include <dxi/font/FontString.h>
#include <dxi/font/IText.h>
#include <dxi/font/Font.h>
#include <me/Geometry.h>

namespace dxi
{
	namespace font
	{
		typedef std::vector< CharacterRender > CharacterRenderList;

		class TextBox3D : public me::Geometry
		{
		public:
			TextBox3D();
			TextBox3D( IText * text );
			virtual ~TextBox3D();
			virtual void Render( const me::IRenderer * renderer, const me::RenderInfo & renderInfo, me::GeometryInstanceData * instanceData );
			virtual void Update( const me::IRenderer * renderer, const me::RenderInfo & renderInfo, me::GeometryInstanceData * instanceData );

		protected:
			std::shared_ptr< IText > m_text;
		};
	} // namespace font
} // namespace dxi