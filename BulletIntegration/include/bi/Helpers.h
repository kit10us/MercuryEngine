// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <dxi/scene/Object.h>
#include <dxi/scene/Scene.h>
#include <bi/Scene.h>

namespace bi
{
    btRigidBody * GetRigidBody( dxi::scene::Object * object );
    const btRigidBody * GetRigidBody( const dxi::scene::Object * object );
    btDiscreteDynamicsWorld * GetWorld( dxi::scene::Scene * scene );
    const btDiscreteDynamicsWorld *  GetWorld( const dxi::scene::Scene * scene );
}