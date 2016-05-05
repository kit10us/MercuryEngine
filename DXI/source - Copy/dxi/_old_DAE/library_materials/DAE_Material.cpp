// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/library_materials/DAE_Material.h>

using namespace dxi;
using namespace dae;

Material::Material( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetStringAttribute( "id" ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
{
	for( const qxml::Element * childNode = node->GetFirstChild(); childNode != 0; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "instance_effect" ) )
		{
			m_instance_effect.reset( new InstanceEffect( childNode ) );
		}
	}
}

const std::string & Material::GetID() const
{
	return m_id;
}

const std::string & Material::GetName() const
{
	return m_name;
}

const InstanceEffect & Material::GetInstanceEffect() const
{
	return *m_instance_effect;
}


