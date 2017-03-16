// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/RenderGirl.h>
#include <me/object/Object.h>
#include <me/GeometryCache.h>

using namespace me;
using namespace scene;
using namespace object;

RenderGirl::RenderGirl()
	: m_renderCount{ 0 }
{
}

RenderGirl::~RenderGirl()
{
}

const RenderParams * RenderGirl::GetParams() const
{
	return m_params;
}

void RenderGirl::Begin( const RenderParams * params )
{
	m_params = params;
	m_renderCount = 0;
}

void RenderGirl::AddCamera( object::FinalCamera camera )
{	
	m_cameraCache.push_back( camera );
}

void RenderGirl::Render( scene::IObjectAllocator * allocator )
{
	GeometryCacheSummation summation;
					 
	// Render all geometry for each camera...
	for( auto camera : m_cameraCache )
	{	
		if( camera.camera->GetRenderer() != m_params->renderer->GetIndex() ) continue;

		allocator->CollectRendering( *m_params, camera, summation );
		m_renderCount += summation.Count();

		RenderInfo myRenderInfo( m_params->renderInfo );
		myRenderInfo.SetViewMatrix( camera.object->GetFrame().GetMatrix().Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		summation.Render( RenderParams{ m_params->renderer, myRenderInfo } );
	}
}

size_t RenderGirl::End()
{
	return m_renderCount;
}
