// Copyright (c) 2002 - 2018 Quentin S. Smith
// All Rights Reserved

//	QList is a virtual list inherited by other lits. It provides a set of common methods.

#pragma once

#include <unify/Unify.h>
#include <string>

namespace unify
{
	typedef unsigned int		QTID;
	#define QTID_INVALID		unsigned int_MAX

	// A link within the chain of the Link List...
	template< typename T >
	class TLink
	{
	public:
		T * m_pItem;
		unsigned int m_uFlags;		// Extra flags for item	(optional)
		std::string m_sName;		// Name in static list	(optional)
		float m_fSortValue;	// sort value for list	(optional)

		TLink< T > * m_pPrevious;
		TLink< T > * m_pNext;

		~TLink();
		TLink();
		TLink( T * pItemIn, const std::string & sNameIn );
	};


	// For traversal
	template< typename T >
	class Query
	{
	public:
		Query();

		TLink< T > * m_pCurrent;
		const std::string & Name();
		float SortValue();
	};


	template< typename T >
	class QList
	{
	public:
		virtual void Clear() = 0;
		virtual QTID Count() = 0;

		// Traversal...
		virtual T * GotoFirst( Query< T > * pQuery ) = 0;
		virtual T * GotoLast( Query< T > * pQuery ) = 0;
		virtual T * GotoPrevious( Query< T > * pQuery ) = 0;
		virtual T * GotoNext( Query< T > * pQuery ) = 0;
	};

	#include <unify/List.inl>
}

