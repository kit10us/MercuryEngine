// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dae/Node.h>
#include <dae/BindMaterial.h>
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
			std::shared_ptr< BindMaterial > m_bind_material;
		};
	}
}
