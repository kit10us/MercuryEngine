// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/QXML.h>
#include <unify/Optional.h>
#include <string>

namespace dae
{
	/// <summary>
	/// DAE node: general
	/// </summary>
	class Input_Unshared
	{
	public:
		typedef std::shared_ptr< Input_Unshared > ptr;

		Input_Unshared( const qxml::Element * node );

		const std::string & GetSemantic() const;
		const std::string & GetSource() const;
			
	private:
		std::string m_semantic;
		std::string m_source;
	};
}