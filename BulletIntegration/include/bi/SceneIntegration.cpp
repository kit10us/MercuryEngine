#pragma once

#include <bi/SceneIntegration.h>
#include <dxi/XMLConvert.h>
#include <bi/XMLConvert.h>

using namespace bi;

class BISceneLF : public dxi::loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, dxi::scene::Scene::shared_ptr scene );
};

class BIModifyLF : public dxi::loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, dxi::scene::Scene::shared_ptr scene );
};

void BISceneLF::operator()( const qxml::Element * element, dxi::scene::Scene::shared_ptr scene )
{
	boost::shared_ptr< bi::Scene > biScene( new bi::Scene );
	scene->SetPhysicsScene( biScene );

	// Attributes...
	bool render = element->GetBooleanAttributeElse( "render", false );
	scene->SetRenderPhysics( render );

	const qxml::Element * subNode = element->GetFirstChild();
	while( subNode )
	{
		if( subNode->GetTagName() == "shape" )
		{
			std::string name = subNode->GetStringAttribute( "name" );

			if( ! biScene )
			{
				throw std::exception( "Attempted to add BI Shape with no BI Scene!" );
			}

			boost::shared_ptr< btCollisionShape > shape;
			if( ! bi::XMLConvertShape( subNode, shape ) )
			{
				throw std::exception( "Unable to process physic's shape from XML element!" );
			}

			biScene->AddShape( name, shape );
		}
		subNode = subNode->GetNext();
	}
}

void BIModifyLF::operator()( const qxml::Element * element, dxi::scene::Scene::shared_ptr scene )
{
	std::string objectName = element->GetStringAttribute( "object" );
	dxi::scene::Object::shared_ptr object = scene->FindObject( objectName );
	if( ! object )
	{
		return;
	}

	if( ! object.get()->GetPhysics() )
	{
		return;
	}

	bi::Instance * instance = dynamic_cast< bi::Instance * >( object->GetPhysics().get() );
	btRigidBody * body = instance->GetRigidBody();

	const qxml::Element * subNode = element->GetFirstChild();
	while( subNode )
	{
		if( boost::iequals( subNode->GetTagName(), "setLinearFactor" ) )
		{
			unify::V3< float > v;
			dxi::XMLConvert( subNode, v );
			body->setLinearFactor( bi::convert( v ) );
		}
		else if( boost::iequals( subNode->GetTagName(), "setAngularFactor" ) )
		{
			unify::V3< float > v;
			dxi::XMLConvert( subNode, v );
			body->setAngularFactor( bi::convert( v ) );
		}
		else if( boost::iequals( subNode->GetTagName(), "setSleepingThresholds" ) )
		{
			btScalar linear = subNode->GetFloatAttribute( "linear" );
			btScalar angular = subNode->GetFloatAttribute( "angular" );
			body->setSleepingThresholds( linear, angular );
		}
		subNode = subNode->GetNext();
	}
}

void bi::IntegrateManager( dxi::scene::SceneManager & manager )
{
	manager.GetSceneLoader().AddLoaderFunctor( "biscene", dxi::loader::SceneLoader::LoaderFunctor::shared_ptr( new BISceneLF ) );
	manager.GetSceneLoader().AddLoaderFunctor( "bimodify", dxi::loader::SceneLoader::LoaderFunctor::shared_ptr( new BIModifyLF ) );
}