// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/IRenderer.h>
#include <me/scene/RenderGirl.h>
#include <me/object/component/CameraComponent.h>
#include <me/render/GeometryCache.h>

using namespace me;
using namespace scene;
using namespace object;
using namespace render;

RenderGirl::RenderGirl()
	: m_renderCount{ 0 }
{
}

RenderGirl::~RenderGirl()
{
}

const render::Params * RenderGirl::GetParams() const
{
	return m_params;
}

void RenderGirl::Begin( const render::Params * params )
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
	GeometryCacheSummation summationSolids;
	GeometryCacheSummation summationTrans;

	IRenderer* renderer = m_params->renderer;


	// Render all geometry for each camera...
	for( auto camera : m_cameraCache )
	{	
		if( camera.camera->GetRenderer() != m_params->renderer->GetIndex() ) continue;

		allocator->CollectRendering( *m_params, camera, summationSolids, summationTrans );

		RenderInfo myRenderInfo( m_params->renderInfo );
		myRenderInfo.SetViewMatrix( camera.object->GetFrame().GetMatrix().Inverse() );
		myRenderInfo.SetProjectionMatrix( camera.camera->GetProjection() );

		renderer->BeforeRenderSolids();

		summationSolids.Render( render::Params{ m_params->renderer, myRenderInfo } );
		m_renderCount += summationSolids.Count();

		renderer->BeforeRenderTrans();

		summationTrans.Render( render::Params{ m_params->renderer, myRenderInfo } );
		m_renderCount += summationTrans.Count();
	}
}

size_t RenderGirl::End()
{
	return m_renderCount;
}
