// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/DefaultLoaders.h>
#include <qxml/Document.h>

using namespace dxi;
using namespace scene;

SceneLoader::SceneLoader()
{
    // Add default loaders...
    AddLoaderFunctor( "import", LoaderFunctor::shared_ptr( new ImportLF( *this ) ) );
	AddLoaderFunctor( "resources", LoaderFunctor::shared_ptr( new ResourcesLF ) );
	AddLoaderFunctor( "addpaths", LoaderFunctor::shared_ptr( new AddPathsLF ) );
	AddLoaderFunctor( "addobject", LoaderFunctor::shared_ptr( new AddObjectLF ) );
	AddLoaderFunctor( "setcamera", LoaderFunctor::shared_ptr( new SetCameraLF ) );
	AddLoaderFunctor( "controller", LoaderFunctor::shared_ptr( new ControllerLF ) );
}

SceneLoader::~SceneLoader()
{
}

void SceneLoader::AddLoaderFunctor( const std::string & elementName, LoaderFunctor::shared_ptr loaderFunctor )
{
	std::string key = elementName;
	m_loaderFunctors[ unify::ToLower( key ) ] = loaderFunctor;
}

std::string SceneLoader::ListLoaderFunctors() const
{
	std::string list;
	for( auto itr = m_loaderFunctors.begin(), end = m_loaderFunctors.end(); itr != end; ++itr )
	{ 
		if( ! list.empty() )
		{
			list += ",";
		}
		list += itr->first;
	}
	return list;
}

void SceneLoader::LoadSceneFromXML( Scene::shared_ptr scene, const unify::Path path )
{
	qxml::Document doc( path );
    qxml::Element * node = doc.GetRoot()->GetFirstChild();

    LoadSceneFromNode( scene, *node );
}

void SceneLoader::LoadSceneFromNode( Scene::shared_ptr scene, const qxml::Element & sceneNode )
{
	for( const auto & node : sceneNode.Children() )
	{
		if( node.GetType() != qxml::Element::NodeType::Element ) continue;

		std::string key = node.GetTagName();
		auto itr = m_loaderFunctors.find( unify::ToLower( key ) );
		if( itr != m_loaderFunctors.end() )
		{
			(*itr->second)( node, scene );
		}
		else
		{
			// No loader functor, ignore.
		}
	}
}
