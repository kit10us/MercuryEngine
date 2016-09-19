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

void ResourceHub::AddManager( IResourceManagerEarly * manager )
{
	m_resourceManagers[ manager->GetName() ] = std::shared_ptr< IResourceManagerEarly >( manager );
}

void ResourceHub::Clear()
{
	m_resourceManagers.clear();
}

void ResourceHub::Load( unify::Path path )
{
	if ( ! path.IsExtension( "XML" ) )
	{
		throw exception::FailedToCreate( "Could not load resource hub file \"" + path.ToString() + "\", only XML files supported!" );
	}

	qxml::Document doc( path );
}		  

void ResourceHub::Load( std::string type, std::string name, unify::Path path )
{
	m_resourceManagers[type]->AddResource( name, path );
}

void ResourceHub::Load( const qxml::Element & element )
{
	for( auto & resource : element.Children() )
	{
		if( resource.GetType() != qxml::Element::NodeType::Element ) continue;

		auto rm = m_resourceManagers[ resource.GetTagName() ];
		rm->AddResource( resource );
	}
}
