// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/PrimitiveList.h>
#include <dxi/dae/DAE_Node.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_visual_scene->visual_scene (1 or more)
		/// </summary>
		class VisualScene : public DocumentNode
		{
		public:
			VisualScene( IDocument & document, const qxml::Element * node );

			const std::string & GetID() const;
			const std::string & GetName() const;
			const Node * GetFirstNode() const;

			void Build( PrimitiveList & pl ) const;

		private:
			void Build( PrimitiveList & pl, const unify::Matrix & matrix, const dae::Node * node ) const;

			std::string m_id;
			std::string m_name;		
			boost::shared_ptr< Node > m_firstNode;
		};
	}
}

