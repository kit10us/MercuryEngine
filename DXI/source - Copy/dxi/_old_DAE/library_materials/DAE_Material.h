// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/DAE_DocumentNode.h>
#include <dxi/dae/library_visual_scenes/DAE_InstanceEffect.h>
#include <qxml/QXML.h>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_materials->material (1 or more)
		/// </summary>
		class Material : public DocumentNode
		{
		public:
			Material( IDocument & document, const qxml::Element * node );

			const std::string & GetID() const;
			const std::string & GetName() const;
			const InstanceEffect & GetInstanceEffect() const;

		private:
			std::string m_id;
			std::string m_name;
			boost::shared_ptr< InstanceEffect > m_instance_effect; // (1)
		};
	}
}