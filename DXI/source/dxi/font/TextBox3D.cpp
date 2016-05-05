// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/TextBox3D.h>

using namespace dxi;
using namespace font;

TextBox3D::TextBox3D()
{
}

TextBox3D::TextBox3D( IText * text )
: m_text( text )
{
}

TextBox3D::~TextBox3D()
{
}

void TextBox3D::Render( RenderInfo renderInfo, GeometryInstanceData * instanceData )
{
	if( m_text.get() )
	{
		renderInfo.SetOption( RenderOption::NoFrame, true );
		renderInfo.SetOption( RenderOption::NoEffectSet, true );
		m_text->Render( renderInfo, true );
	}
}

void TextBox3D::Update( unify::Seconds elapsed, GeometryInstanceData * instanceData )
{
	if( m_text.get() )
	{
		m_text->Update( elapsed );
	}
}
