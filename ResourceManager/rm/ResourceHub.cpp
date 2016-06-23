#include <rm/ResourceHub.h>

using namespace rm;

ResourceHub::ResourceHub()
{
}

ResourceHub::~ResourceHub()
{
}

void ResourceHub::AddManager( IResourceManagerEarly * manager )
{
	m_resourceManagers[ manager->GetName() ] = std::shared_ptr< IResourceManagerEarly >( manager );
}

void ResourceHub::Clear()
{
	m_resourceManagers.clear();
}