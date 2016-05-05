// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/QXML.h>
#include <dae/Exceptions.h>
#include <dae/InstanceSet.h>
#include <dae/DocumentNode.h>
#include <dae/InstanceVisualScene.h>
#include <boost/optional.hpp>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->scene (0 or 1)
		/// </summary>
		class Scene : public DocumentNode
		{
		public:
			Scene( IDocument & document, const qxml::Element * node );

			/// <summary>
			/// instance_physics_scene (0 or more)
			/// </summary>
			const std::vector< InstanceSet > & GetInstancePhysicsScenes() const;

			/// <summary>
			/// instance_visual_scene (0 or 1)
			/// </summary>
			const InstanceVisualScene * GetInstanceVisualScene() const;

			/// <summary>
			/// instance_kinematics_scene (0 or 1)
			/// </summary>
			const boost::optional< InstanceSet > & GetInstanceKinematicsScene() const;

		private:
			std::vector< InstanceSet > m_instance_physics_scenes;
			std::shared_ptr< InstanceVisualScene > m_instance_visual_scene;
			boost::optional< InstanceSet > m_instance_kinematics_scene;
		};
	}
}
