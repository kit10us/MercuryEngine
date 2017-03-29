// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mesh.h>
#include <dae/Node.h>
#include <dae/DocumentNode.h>
#include <string>

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

		const Node * FindNode( std::string id ) const;

		void Build( me::Mesh & mesh ) const;

	private:
		void Build( me::Mesh & mesh, const unify::Matrix & matrix, const dae::Node * node ) const;

		std::string m_id;
		std::string m_name;		
		Node::ptr m_firstNode;
	};
}