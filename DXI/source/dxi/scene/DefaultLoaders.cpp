// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/DefaultLoaders.h>
#include <dxi/XMLConvert.h>
#include <dxi/Mesh.h>
#include <dxi/exception/NotImplemented.h>

using namespace dxi;
using namespace scene;

ImportLF::ImportLF( SceneLoader & sceneLoader )
    : m_sceneLoader( sceneLoader )
{
}

void ImportLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	unify::Path source( unify::Path( element->GetDocument()->GetPath() ).DirectoryOnly() + element->GetStringAttribute( "source" ) );
	m_sceneLoader.LoadSceneFromXML( scene, source.ToString() );
}

void AddPathsLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	//unify::Path source( unify::Path( element->GetDocument()->GetPath() ).DirectoryOnly() + element->GetStringAttribute( "source" ) );
	//scene->GetPathSystem().LoadFromFile( source );
	throw new exception::NotImplemented;
}

void TextureLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	core::Game::GetGameInstance()->GetManager< Texture >()->Add( element );
}

void ShapeLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	std::string name = element->GetStringAttribute( "name" );
	unify::Path source( unify::Path( element->GetDocument()->GetPath() ).DirectoryOnly() + element->GetStringAttribute( "source" ) );

	auto & geometryManager = *core::Game::GetGameInstance()->GetManager< Geometry >();

	/* TODO:
	Geometry * geometry = new Mesh( source, 0 );
	geometryManager.Add( name, geometry );
	*/
}

void MeshLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	std::string name = element->GetStringAttribute( "name" );
	unify::Path source( unify::Path( element->GetDocument()->GetPath() ).DirectoryOnly() + element->GetStringAttribute( "source" ) );

	auto & geometryManager = *core::Game::GetGameInstance()->GetManager< Geometry >();

	/* TODO:
	Geometry * geometry = new Mesh( source, 0 );
	geometryManager.Add( name, geometry );
	*/
}

void TerraLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	std::string name = element->GetStringAttribute( "name" );
	unify::Path source( unify::Path( element->GetDocument()->GetPath() ).DirectoryOnly() + element->GetStringAttribute( "source" ) );

	auto & geometryManager = *core::Game::GetGameInstance()->GetManager< Geometry >();

	qxml::Document document( source );
    qxml::Element * rootNode = document.GetRoot();
	/* TODO:
    TerraLoader( rootNode );
	*/
}

void AddObjectLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
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

void SetCameraLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	scene->SetCamera( element->GetText() );
}

void ControllerLF::operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
{
	/*
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
	*/
	throw new exception::NotImplemented;
}
