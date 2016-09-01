// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/QXML.h>
#include <unify/String.h>
#include <vector>
#include <string>

namespace dae
{
	/// <summary>
	/// DAE node: COLLADA->library_geometries->geometry->mesh->source->float_array (0 or 1)
	/// </summary>
	class FloatArray
	{
	public:
		FloatArray( const qxml::Element * node );

		int GetCount() const;
		const std::string & GetID() const;
		const std::string & GetName() const;
		int GetDigits() const;
		int GetMagnitude() const;
		const std::string & GetStringContents() const;
		const std::vector< float > & GetArrayContents() const;

	private:
		int m_count;
		std::string m_id;
		std::string m_name;
		int m_digits;
		int m_magnitude;
		std::string m_stringContents;
		std::vector< float > m_arrayContents;
	};
}