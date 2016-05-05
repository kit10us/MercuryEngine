// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/SceneLoader.h>
#include <dxi/loader/ShapeLoader.h>
#include <dxi/loader/TerraLoader.h>
#include <dxi/XMLConvert.h>

using namespace dxi;

////////////////////////////////////////
// Default loader functors...
////////////////////////////////////////

class ImportLF : public loader::SceneLoader::LoaderFunctor
{
    loader::SceneLoader & m_sceneLoader;
public:
    ImportLF( loader::SceneLoader & sceneLoader )
        : m_sceneLoader( sceneLoader )
    {
    }

    void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		unify::Path source(element->GetDocument()->GetPath());
		source.ChangeFilename( element->GetStringAttribute("source") );
		m_sceneLoader.LoadSceneFromXML( scene, source );
    }
};

class AddPathsLF : public loader::SceneLoader::LoaderFunctor
{
public:
    void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		unify::Path source(element->GetDocument()->GetPath());
		source.ChangeFilename( element->GetStringAttribute("source") );

		scene->GetPathSystem().LoadFromFile( source );
	}
};

class TextureLF : public loader::SceneLoader::LoaderFunctor
{
public:
    void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
        std::string name = element->GetStringAttribute( "name" );
		unify::Path source(element->GetDocument()->GetPath());
		source.ChangeFilename( element->GetStringAttribute( "source" ) );

        unsigned int flags = 0;
        if ( element->HasAttributes( "lockable" ) )
        {
            flags |= TEXTURE_LOCKABLE;
        }

		scene->GetGeometryManager()->GetTextureManager()->LoadFromFile( name, source, flags );
	}
};

class GeometryLF : public loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		std::string name = element->GetStringAttribute( "name" );
		std::string type = element->GetStringAttribute( "type" );
		unify::Path source(element->GetDocument()->GetPath());
		source.ChangeFilename( element->GetStringAttribute("source") );

		geo::Geometry * geometry;
		if( type == "shape" )
		{
			geometry = new geo::Mesh( source, 0, scene->GetGeometryManager()->GetTextureManager() );
		}
		else if( type == "mesh" )
		{
			geometry = new geo::Mesh( source, 0, scene->GetGeometryManager()->GetTextureManager() );
		}
		else if( type == "terra" )
		{
			qxml::Document document( source );
            qxml::Element * rootNode = document.GetRoot();
            geometry = loader::TerraLoader( rootNode, scene->GetGeometryManager()->GetTextureManager() );
		}
		else
		{
			throw unify::Exception( "Not a valid geometry type!" );
		}
		scene->GetGeometryManager()->Add( name, geometry );
	}
};

class TerraLF : public loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		unify::Path source(element->GetDocument()->GetPath());
		source.ChangeFilename( element->GetStringAttribute("source") );
	}
};
class AddObjectLF : public loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		std::string name = element->GetStringAttribute( "name" );
		std::string geometry = element->GetStringAttributeElse( "geometry", "" );
		std::string physics = element->GetStringAttributeElse( "physics", "" );
		scene::Object::shared_ptr object = scene->CreateObject( name, geometry, physics );

		const qxml::Element * subNode = element->GetFirstChild();
		while( subNode )
		{
			if( subNode->GetTagName() == "position" )
			{
				unify::V3< float > position;
				if( ! XMLConvert( subNode, position ) )
				{
					scene->FindPosition( subNode->GetText(), position );
				}
				object->GetFrame().SetPosition( position );
			}
			else if( subNode->GetTagName() == "rotation" )
			{
				unify::Quaternion q;
				XMLConvert( subNode, q );
				object->GetFrame().SetRotation( q );
			}
			else if( subNode->GetTagName() == "geometryposition" )
			{
				unify::V3< float > position;
				XMLConvert( subNode, position );
				unify::Matrix matrix = object->GetGeometryMatrix();
				matrix.SetPosition( position );
				object->SetGeometryMatrix( matrix );
			}
			else if( subNode->GetTagName() == "geometryscale" )
			{
				unify::Matrix matrix = object->GetGeometryMatrix();
				unify::V3< float > scale( 1.0f, 1.0f, 1.0f );
				if( subNode->HasAttributes( "value" ) )
				{
					scale.x = subNode->GetFloatAttribute( "value" );
					scale.y = scale.x;
					scale.z = scale.x;
				}
				else
				{
					scale.x = subNode->GetFloatAttributeElse( "x", 1.0f );
					scale.y = subNode->GetFloatAttributeElse( "y", 1.0f );
					scale.z = subNode->GetFloatAttributeElse( "z", 1.0f );
				}
				matrix.Scale( scale );
				object->SetGeometryMatrix( matrix );
			}
			else if( subNode->GetTagName() == "geometryrotation" )
			{
				unify::Quaternion q;
				XMLConvert( subNode, q );
				unify::Matrix matrix = object->GetGeometryMatrix();
				matrix.SetRotation( q );
				object->SetGeometryMatrix( matrix );
			}
			subNode = subNode->GetNext();
		}
		if( object->GetPhysics() )
		{
			object->GetPhysics()->SyncPhysicsFromObject();
		}
	}
};

class SetCameraLF : public loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		scene->SetCamera( element->GetText() );
	}
}; 

class ControllerLF : public loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		std::string objectName = element->GetStringAttribute( "object" );
		std::string typeName = element->GetStringAttribute( "type" );
		scene::Object * object = scene.get()->FindObject( objectName ).get();
		if( ! object )
		{
			return;
		}

		const controllers::ControllerFactory::Producer * producer = scene.get()->GetControllerFactory()->GetProducer( typeName );
		if( ! producer )
		{
			return;
		}
		object->SetController( producer->Produce( object, element, scene.get() ) );
	}
};

////////////////////////////////////////
////////////////////////////////////////


loader::SceneLoader::SceneLoader()
{
    // Add default loaders...
    AddLoaderFunctor( "import", LoaderFunctor::shared_ptr( new ImportLF( *this ) ) );
	AddLoaderFunctor( "addpaths", LoaderFunctor::shared_ptr( new AddPathsLF ) );
    AddLoaderFunctor( "texture", LoaderFunctor::shared_ptr( new TextureLF ) );
	AddLoaderFunctor( "geometry", LoaderFunctor::shared_ptr( new GeometryLF ) );
	AddLoaderFunctor( "addobject", LoaderFunctor::shared_ptr( new AddObjectLF ) );
	AddLoaderFunctor( "setcamera", LoaderFunctor::shared_ptr( new SetCameraLF ) );
	AddLoaderFunctor( "controller", LoaderFunctor::shared_ptr( new ControllerLF ) );
}

loader::SceneLoader::~SceneLoader()
{
}

void loader::SceneLoader::AddLoaderFunctor( const std::string & elementName, LoaderFunctor::shared_ptr loaderFunctor )
{
	std::string key = elementName;
	m_loaderFunctors[ unify::ToLower( key ) ] = loaderFunctor;
}

std::string loader::SceneLoader::ListLoaderFunctors() const
{
	std::string list;
	for( std::map< std::string, LoaderFunctor::shared_ptr >::const_iterator itr = m_loaderFunctors.begin(),
		end = m_loaderFunctors.end(); itr != end; ++itr )
	{ 
		if( ! list.empty() )
		{
			list += ",";
		}
		list += itr->first;
	}
	return list;
}

void loader::SceneLoader::LoadSceneFromXML( scene::Scene::shared_ptr scene, const unify::Path path )
{
	qxml::Document doc( path );
    qxml::Element * node = doc.GetRoot()->GetFirstChild();

    LoadSceneFromNode( scene, node );
}

void loader::SceneLoader::LoadSceneFromNode( scene::Scene::shared_ptr scene, qxml::Element * node )
{
	while( node )
	{
		std::string key = node->GetTagName();
		std::map< std::string, std::shared_ptr< LoaderFunctor > >::iterator itr = m_loaderFunctors.find( unify::ToLower( key ) );
		if( itr != m_loaderFunctors.end() )
		{
			(*itr->second)( node, scene );
		}
		else
		{
			// No loader functor, ignore.
		}

		node = node->GetNext();
	}
}
