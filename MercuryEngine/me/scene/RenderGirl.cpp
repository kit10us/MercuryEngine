// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/RenderGirl.h>
#include <me/scene/Object.h>
#include <me/GeometryCache.h>

using namespace me;
using namespace scene;

RenderGirl::RenderGirl()
{
}

RenderGirl::~RenderGirl()
{
}
											 
void RenderGirl::Begin( const RenderParams * params )
{
	m_params = params;
}

void RenderGirl::Render( scene::IObjectAllocator * allocator )
{
	CameraCache cameras;
	GeometryCacheSummation summation;
	allocator->CollectRendering( *m_params, cameras, summation );
						 
	// Render all geometry for each camera...
	for( auto camera : cameras )
	{
		if( camera.camera->GetRenderer() != m_params->renderer->GetIndex() ) continue;

		RenderInfo myRenderInfo( m_params->renderInfo );
		myRenderInfo.SetViewMatrix( camera.object->GetFrame().GetMatrix().Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		summation.Render( RenderParams{ m_params->renderer, myRenderInfo } );
	}
}

void RenderGirl::End()
{
}
