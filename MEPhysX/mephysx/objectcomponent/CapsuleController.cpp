// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#include <mephysx/objectcomponent/CapsuleController.h>
#include <me/object/Object.h>

using namespace me;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

CapsuleController::CapsuleController( CapsuleController & collider )
	: ControllerBase( collider )
{
}

CapsuleController::CapsuleController( mephysx::SceneComponent * sceneComponent, float radius, float height )
: ControllerBase( "CapsuleController", sceneComponent )
{
	PxCapsuleControllerDesc desc;
	desc.setToDefault();
	desc.radius = radius;
	desc.height = height;
	desc.material = sceneComponent->GetPxMaterial();
	// This is done OnAttach: desc.userData = player;

	bool isvalid = desc.isValid();

	PxController * controller = sceneComponent->GetPxControllerManager()->createController( desc );
	
	m_controller.reset( controller, Releaser< PxController > ); 
}

CapsuleController::~CapsuleController()
{
}

std::string CapsuleController::GetWhat() const
{
	return std::string();
}

object::IObjectComponent::ptr CapsuleController::Duplicate()
{
	auto duplicate = new CapsuleController( *this );
	return object::IObjectComponent::ptr( duplicate );
}

void CapsuleController::OnAttach( me::object::Object * object )
{
	m_controller->setUserData( object );
}					  
