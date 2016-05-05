// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <bi/DebugDraw.h>
#include <dxi/core/Win32Game.h>
#include <unify/Color.h>

using namespace bi;

DebugDraw::DebugDraw() 
	: m_debugMode( DBG_NoDebug )
{
}

void DebugDraw::drawLine( const btVector3 & from, const btVector3 & to, const btVector3 & color )
{
	IDirect3DDevice9 * device = dxi::core::Win32Game::GetGameInstance()->GetDxDevice();
	unsigned int fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	unify::Color color2( 
		unify::Color::ColorARGB(
			255, static_cast< unsigned char >( color.x() * 255 ), static_cast< unsigned char >( color.y() * 255 ), static_cast< unsigned char >( color.z() * 255 ) 
			) 
		);
	struct Vertex {
		unify::V3< float > position;
		unify::Color diffuse;
	} vertices[ 2 ] =
	{
		{ unify::V3< float >( from.x(), from.y(), from.z() ), color2 },
		{ unify::V3< float >( to.x(), to.y(), to.z() ), color2 }
	};

	device->DrawPrimitiveUP( D3DPT_LINELIST, 1, ( void * * )&vertices, sizeof Vertex );
}

void DebugDraw::drawContactPoint( const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color )
{
}

void DebugDraw::reportErrorWarning( const char * message )
{
	OutputDebugStringA( message );
}

void DebugDraw::draw3dText(const btVector3& location,const char* textString)
{
}

void DebugDraw::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int	DebugDraw::getDebugMode() const
{
	return m_debugMode;
}
