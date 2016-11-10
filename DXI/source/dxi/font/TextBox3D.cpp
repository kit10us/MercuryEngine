// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/TextBox3D.h>

using namespace dxi;
using namespace font;
using namespace me;

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

void TextBox3D::Render( const me::RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	me::RenderInfo myRenderInfo( renderInfo );
	if( m_text.get() )
	{
		myRenderInfo.SetOption( RenderOption::NoFrame, true );
		myRenderInfo.SetOption( RenderOption::NoEffectSet, true );
		m_text->Render( myRenderInfo, true );
	}
}

void TextBox3D::Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	if( m_text.get() )
	{
		m_text->Update( renderInfo );
	}
}
