// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Cast.h>
#include <unify/String.h>
#include <me/exception/OutOfBounds.h>
#include <map>
#include <vector>

namespace me
{
	class Lookup
	{
	public:
		Lookup();

		bool AddValue( std::string name, std::string value );

		bool SetValue( std::string name, std::string value );

		bool SetValue( size_t index, std::string value );

		bool Exists( std::string name ) const;
		
		/// <summary>
		/// Returns the index for value with a specific name. Throws if not found, so ensure you test with Exists beforehand.
		/// </summary>
		size_t Find( std::string name ) const;

		std::string GetValue( std::string name ) const;

		std::string GetValue( size_t index ) const;

		size_t Count() const;
		std::string GetName( size_t index ) const;

	private:
		std::map< std::string /*name*/, size_t /*index into vector*/, unify::CaseInsensitiveLessThanTest > m_valueMap;
		std::vector< std::string > m_values;
		std::vector< std::string > m_names;
	};
}