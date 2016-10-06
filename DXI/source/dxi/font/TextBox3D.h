// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/font/CharacterRender.h>
#include <dxi/font/FontString.h>
#include <dxi/font/IText.h>
#include <dxi/font/Font.h>
#include <dxi/Geometry.h>

namespace dxi
{
	namespace font
	{
		typedef std::vector< CharacterRender > CharacterRenderList;

		class TextBox3D : public Geometry
		{
		public:
			TextBox3D();
			TextBox3D( IText * text );
			virtual ~TextBox3D();
			virtual void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );
			virtual void Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );

		protected:
			std::shared_ptr< IText > m_text;
		};
	} // namespace font
} // namespace dxi