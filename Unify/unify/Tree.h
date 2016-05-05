// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>

namespace unify 
{
	class Tree
	{
	public:
		Tree();
		Tree( const std::string & sName );
		Tree( const Tree & tree );
		virtual ~Tree();
		virtual Tree & operator=( const Tree & tree );
		virtual void Detach();
		virtual void AttachTo( Tree * pTo );
		virtual void SetName( const std::string & name );
		virtual const std::string & GetName() const;
		virtual Tree * GetTopSibling();
		virtual Tree * GetBottomSibling();
		virtual Tree * GetRoot();
		virtual Tree * GetParent();
		virtual Tree * GetChild();
		virtual Tree * GetPrev();
		virtual Tree * GetNext();
		virtual Tree * SetParent( Tree * parent );
		virtual Tree * SetChild( Tree * child );
		virtual Tree * SetPrev( Tree * prev );
		virtual Tree * SetNext( Tree * next);

	protected:
		Tree * m_child;	// A child would have us as Parent
		Tree * m_prevSib; // Sibling we are Next to
		Tree * m_nextSib;	// Next sibling to our parent (child of our parent)
		Tree * m_parent;

		std::string m_name;	// Frames name
	};
} // namespace unify
#include <unify/Tree.inl>
