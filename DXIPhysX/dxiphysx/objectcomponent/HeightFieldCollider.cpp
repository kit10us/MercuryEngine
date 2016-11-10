// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/HeightFieldCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

//PxHeightFieldGeometry

HeightFieldCollider::HeightFieldCollider( me::IOS * os, GameComponent * gameComponent, dxi::Terra * terra, unify::RowColumn< unsigned int > rcCount  )
	: ColliderBase( os, gameComponent )
{
	PxPhysics * physics = gameComponent->GetPhysics();

	physx::PxReal heightScale = 1.0f;

	physx::PxHeightFieldSample* samples = (physx::PxHeightFieldSample*) new physx::PxHeightFieldSample[rcCount.row * rcCount.column];
	unify::RowColumn< unsigned int > rc;
	for ( rc.column = 0; rc.column < rcCount.column; rc.column++ )
	{
		for ( rc.row = 0; rc.row < rcCount.row; rc.row++ )
		{
			float x = (float)rc.row / ( rcCount.row - 1 );
			float y = (float)rc.column / ( rcCount.column - 1 );
			samples[rc.row + (rc.column * rcCount.row)].height = (physx::PxI16)terra->GetDepth( y, x );
			samples[rc.row + (rc.column * rcCount.row)].materialIndex0 = 0;
			samples[rc.row + (rc.column * rcCount.row)].materialIndex1 = 0;
		}
	}

	physx::PxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.format = physx::PxHeightFieldFormat::eS16_TM;
	heightFieldDesc.nbColumns = rc.column;
	heightFieldDesc.nbRows = rc.row;
	heightFieldDesc.samples.data = samples;
	heightFieldDesc.samples.stride = sizeof( physx::PxHeightFieldSample );

	physx::PxHeightField * heightField = 0;
	heightField = physics->createHeightField( heightFieldDesc );

	float width = terra->GetSize().width / rcCount.row;
	float height = terra->GetSize().height / rcCount.column;

	PxHeightFieldGeometry heightFieldGeometry( heightField, physx::PxMeshGeometryFlags(), heightScale, width, height );
	m_shape.reset( gameComponent->GetPhysics()->createShape( heightFieldGeometry, *m_material ), Releaser< PxShape > );

	PxTransform tm( m_shape->getLocalPose() );
	tm.p.x -= terra->GetSize().width / 2;
	tm.p.z -= terra->GetSize().height / 2;
	m_shape->setLocalPose( tm );

}

HeightFieldCollider::~HeightFieldCollider()
{
}

std::string HeightFieldCollider::GetName() const
{
	return "HeightField Collider";
}

me::IObjectComponent * HeightFieldCollider::Duplicate()
{
	auto duplicate = new HeightFieldCollider( *this );
	return duplicate;
}

