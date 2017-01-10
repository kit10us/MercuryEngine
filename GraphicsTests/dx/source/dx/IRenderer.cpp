#include <dx/IRenderer.h>

using namespace dx;

IRenderer * IRenderer::s_renderer = 0;

void IRenderer::SetInstance( IRenderer * renderer )
{ 
	s_renderer = renderer; 
}

IRenderer * IRenderer::GetInstance() 
{ 
	assert( s_renderer ); 
	return s_renderer; 
}
