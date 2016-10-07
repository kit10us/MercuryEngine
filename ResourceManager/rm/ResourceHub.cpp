#include <rm/ResourceHub.h>
#include <rm/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace rm;

ResourceHub::ResourceHub()
{
}

ResourceHub::~ResourceHub()
{
}

void ResourceHub::AddManager( std::shared_ptr< IResourceManagerEarly > manager )
{
	m_resourceManagers[ manager->GetName() ] = manager;
}

void ResourceHub::Clear()
{
	m_resourceManagers.clear();
}

void ResourceHub::Load( std::string type, std::string name, unify::Path path )
{
	m_resourceManagers[type]->AddResource( name, path );
}
