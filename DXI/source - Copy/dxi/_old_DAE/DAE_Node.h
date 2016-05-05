// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/DAE_InstanceSet.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <qxml/QXML.h>
#include <unify/Matrix.h>
#include <boost/optional.hpp>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_visual_scene->visual_scene->node (1 or more)
		/// </summary>
		class Node : public DocumentNode
		{
		public:
			/// <summary>
			/// For hierarchical tree structure.
			/// </summary>
			Node( IDocument & document, const Node * parent, const qxml::Element * node );
			
			/// <summary>
			/// Specifically for LibraryNodes, as there isn't a hierarchy.
			/// </summary>
			Node( IDocument & document, const qxml::Element * node );

			void Append( boost::shared_ptr< Node > node );

			const std::string & GetID() const;
			const std::string & GetName() const;
			const std::string & GetSID() const;
			const unify::Matrix & GetMatrix() const;
			unify::Matrix GetFinalMatrix() const;
			const std::vector< boost::shared_ptr< InstanceSet > > & GetInstances() const;
			const Node * GetParent() const;
			const Node * GetFirstChild() const;
			const Node * GetNext() const;

		private:
			const Node * m_parent;
			std::string m_id;
			std::string m_name;
			std::string m_sid;
			// type;
			// layer;

			unify::Matrix m_matrix; // Combination of all transformation_elements.
			std::vector< boost::shared_ptr< InstanceSet > > m_instances;
			boost::shared_ptr< Node > m_firstChild;
			boost::shared_ptr< Node > m_next;
		};
	}
}
