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

void ImportLF::operator()( const qxml::Element & element, scene::Scene::shared_ptr scene )
{
	unify::Path source( unify::Path( element.GetDocument()->GetPath() ).DirectoryOnly() + element.GetAttribute( "source" )->GetString() );
	m_sceneLoader.LoadSceneFromXML( scene, source.ToString() );
}

void ResourcesLF::operator()( const qxml::Element & element, scene::Scene::shared_ptr scene )
{
	core::Game::GetInstance()->GetResourceHub().Load( element );
}																	

void AddPathsLF::operator()( const qxml::Element & element, scene::Scene::shared_ptr scene )
{
	//unify::Path source( unify::Path( element.GetDocument()->GetPath() ).DirectoryOnly() + element.GetAttribute< std::string >( "source" ) );
	//scene->GetPathSystem().LoadFromFile( source );
	throw new exception::NotImplemented;
}

void AddObjectLF::operator()( const qxml::Element & element, scene::Scene::shared_ptr scene )
{
	core::Game & game = *core::Game::GetInstance();
	auto & geometryManager = *core::Game::GetInstance()->GetManager< Geometry >();

	std::string name = element.GetAttribute( "name" )->GetString();
	std::string geometry = element.GetAttributeElse< std::string >( "geometry", "" );
	// TODO: std::string physics = element.GetAttributeElse< std::string >( "physics", "" );
	scene::Object::shared_ptr object { scene->Add( name ) };
	object->SetGeometry( geometryManager.Find( geometry ) );

	const qxml::Element * subNode = element.GetFirstChild();
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
		else if( subNode->GetTagName() == "lookat" )
		{
			unify::V3< float > position;
			if( !XMLConvert( subNode, position ) )
			{
				scene->FindPosition( subNode->GetText(), position );
			}
			object->GetFrame().LookAt( position );
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
			object->GetGeometryMatrix() = matrix;
		}
		else if( subNode->GetTagName() == "geometryscale" )
		{
			unify::Matrix matrix = object->GetGeometryMatrix();
			unify::V3< float > scale( 1.0f, 1.0f, 1.0f );
			if( subNode->HasAttributes( "value" ) )
			{
				scale.x = subNode->GetAttribute( "value" )->Get< float >();
				scale.y = scale.x;
				scale.z = scale.x;
			}
			else
			{
				scale.x = subNode->GetAttributeElse( "x", 1.0f );
				scale.y = subNode->GetAttributeElse( "y", 1.0f );
				scale.z = subNode->GetAttributeElse( "z", 1.0f );
			}
			matrix.Scale( scale );
			object->GetGeometryMatrix() = matrix;
		}
		else if( subNode->GetTagName() == "geometryrotation" )
		{
			unify::Quaternion q;
			XMLConvert( subNode, q );
			unify::Matrix matrix = object->GetGeometryMatrix();
			matrix.SetRotation( q );
			object->GetGeometryMatrix() = matrix;
		}
		subNode = subNode->GetNext();
	}
	if( object->GetPhysics() )
	{
		object->GetPhysics()->SyncPhysicsFromObject();
	}
}

void SetCameraLF::operator()( const qxml::Element & element, scene::Scene::shared_ptr scene )
{
	core::Game & game = *core::Game::GetInstance();

	scene->SetCamera( element.GetText() );

	// TODO: This is a hack. It is a larger problem to solve this: variables/functions in scene.xml, math in scene.xml, migration to a scripting language, custom functions.
	scene->GetCamera().SetProjection( unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, game.GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );

}

void ControllerLF::operator()( const qxml::Element & element, scene::Scene::shared_ptr scene )
{
	/*
	std::string objectName = element.GetAttribute< std::string >( "object" );
	std::string typeName = element.GetAttribute< std::string >( "type" );
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
