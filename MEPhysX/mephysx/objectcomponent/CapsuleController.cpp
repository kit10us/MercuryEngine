// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/CapsuleController.h>
#include <me/scene/Object.h>

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
	desc.material = sceneComponent->GetMaterial();
	// This is done OnAttach: desc.userData = player;

	bool isvalid = desc.isValid();

	PxController * controller = sceneComponent->GetControllerManager()->createController( desc );
	
	m_controller.reset( controller, Releaser< PxController > ); 
}

CapsuleController::~CapsuleController()
{
}

std::string CapsuleController::GetWhat() const
{
	return std::string();
}

scene::IObjectComponent * CapsuleController::Duplicate()
{
	auto duplicate = new CapsuleController( *this );
	return duplicate;
}

void CapsuleController::OnAttach( me::scene::Object * object )
{
	m_controller->setUserData( object );
}					  
