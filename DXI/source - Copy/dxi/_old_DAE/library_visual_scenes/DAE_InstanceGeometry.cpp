// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/library_visual_scenes/DAE_InstanceGeometry.h>

using namespace dxi;
using namespace dae;

InstanceGeometry::InstanceGeometry( const qxml::Element * node )
: InstanceSet( node, InstanceSet::InstanceTypeGeometry )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "bind_material" ) )
		{
			m_bind_material.reset( new BindMaterial( childNode ) );
		}
	}
}

const BindMaterial * InstanceGeometry::GetBindMaterial() const
{
	return m_bind_material.get();
}