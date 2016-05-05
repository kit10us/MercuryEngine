// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/dae/library_visual_scenes/DAE_InstanceMaterial.h>
#include <qxml/QXML.h>
#include <map>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// </summary>
		class BindMaterial_TechniqueCommon
		{
		public:
			BindMaterial_TechniqueCommon( const qxml::Element * node );
			const std::vector< boost::shared_ptr< InstanceMaterial > > & GetInstanceMaterial() const;
			const InstanceMaterial & GetInstanceMaterial( const std::string & name ) const;

		private:
			std::vector< boost::shared_ptr< InstanceMaterial > > m_instance_material;
			std::map< std::string, boost::shared_ptr< InstanceMaterial > > m_instance_material_map;
		};
	}
}
