// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/DAE_Node.h>
#include <dxi/dae/DAE_BindMaterial.h>
#include <qxml/QXML.h>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_visual_scenes->visual_scene->node->instance_geometry ()
		/// </summary>
		//  (TODO: might support InstanceController as well).
		class InstanceGeometry : public InstanceSet
		{
		public:
			InstanceGeometry( const qxml::Element * node );
			const BindMaterial * GetBindMaterial() const;

		private:
			boost::shared_ptr< BindMaterial > m_bind_material;
		};
	}
}
