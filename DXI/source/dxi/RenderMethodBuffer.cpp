// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/Exception.h>
#include <assert.h>
#include <dxi/RenderMethodBuffer.h>
#include <dxi/core/Game.h>

using namespace dxi;

RenderMethodBuffer::RenderMethodBuffer()
{
}

void RenderMethodBuffer::Release()
{
	m_methodList.clear();
}

void RenderMethodBuffer::AddMethod( RenderMethod & method )
{
	m_methodList.push_back( method );
}

size_t RenderMethodBuffer::Append( const RenderMethodBuffer & from, size_t vertexOffset, size_t indexOffset )
{
	for( size_t m = 0; m < from.m_methodList.size(); ++m )
	{
		const RenderMethod & methodSource = from.m_methodList[ m ];
		RenderMethod method( methodSource );
		method.startVertex += vertexOffset;
		method.baseVertexIndex = 0;
		method.minIndex += indexOffset;
		//method.vertexCount;
		method.startIndex += indexOffset;
		//method.primitiveCount;
		m_methodList.push_back( method );
	}
	return from.GetSize();
}

RenderMethod & RenderMethodBuffer::GetMethod( unsigned int method )
{
	return m_methodList[ method ];
}

const RenderMethod & RenderMethodBuffer::GetMethod( unsigned int method ) const
{
	return m_methodList[ method ];
}

size_t RenderMethodBuffer::GetSize() const
{
	return m_methodList.size();
}

void RenderMethodBuffer::Render( RenderInfo renderInfo ) const
{
	// Iterate through methods to render.
	for( unsigned int i = 0; i != m_methodList.size(); ++i )
	{
		const RenderMethod & renderMethod = m_methodList[ i ];
		
		/*
		// Cull obvious solids...
		if( renderInfo.IsOptionTrue( RenderOption::TransOnly ) )
		{	
			if( ! currentEffect->UsesTransparency() )
			{
				//pMethod = pMethod->pNext;
				continue;
			}
		}

		// Allow culling obvious transparents...
		if( renderInfo.IsOptionTrue( RenderOption::SolidOnly ) )
		{
			if( currentEffect->UsesTransparency() )
			{
				//pMethod = pMethod->pNext;
				continue;
			}
		}
		*/
		
		/*
		// If... we don't want to set effects.
		//   ... we don't have the same effect already set.
		//   ... we have an effect set.
        if( ! renderInfo.IsOptionTrue( RenderOption::NoEffectSet ) && currentEffect && currentEffecIndex != renderMethod.effecIndex && renderMethod.effecIndex != std::numeric_limits< unsigned int >::max() )
		{
			currentEffect->Use( renderInfo );
		}
		*/

		// Draw Primitive...
		renderMethod.Render( renderInfo );
	}
}