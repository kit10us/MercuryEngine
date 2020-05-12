// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/IThing.h>
#include <me/interop/IValue.h>
#include <unify/Lookup.h>

namespace me::interop
{
	/// <summary>
	/// Provides access of variables specific to the Interp's owner,
	/// especially for runtime access, such as scripts or runtime tools.
	/// Variables can be accessed via a key or an index.
	/// </summary>
	class Interop
	{
		unify::Lookup< std::string, IValue::ptr > m_lookup;

	public:
		/// <summary>
		/// Add a variable.
		/// </summary>
		size_t Add( std::string key, IValue::ptr value );

		/// <summary>
		/// Add a string variable.
		/// </summary>
		size_t Add( std::string key, std::string init = std::string{} );

		/// <summary>
		/// Returns the number of variables.
		/// </summary>
		size_t Count() const;

		/// <summary>
		/// Check if we have a variable.
		/// </summary>
		bool Exists( std::string key ) const;

		/// <summary>
		/// Find a variable defined by a given key.
		/// </summary>
		size_t Find( std::string key ) const;

		/// <summary>
		/// Get the name of a variable by index.
		/// </summary>
		std::string GetName( size_t index ) const;

		/// <summary>
		/// Get the value of a variable, as a string, by index.
		/// </summary>
		std::string GetValue( size_t index ) const;

		/// <summary>
		/// Get the value of a variable, as a string, by key.
		/// </summary>
		std::string GetValue( std::string key ) const;

		void SetValue( size_t index, std::string value );

		void SetValue( std::string key, std::string value );

		class iterator
		{
			unify::Lookup< std::string, interop::IValue::ptr >::iterator m_itr;
		public:
			iterator( unify::Lookup< std::string, interop::IValue::ptr >::iterator itr );

			iterator operator++();

			iterator operator++( int );

			iterator operator--();

			iterator operator--( int );

			bool operator==( const iterator& itr ) const;

			bool operator!=( const iterator& itr ) const;

			unify::KeyValuePair< std::string, interop::IValue::ptr >& operator*();

			const unify::KeyValuePair< std::string, interop::IValue::ptr >& operator*() const;

			unify::KeyValuePair< std::string, interop::IValue::ptr >* operator->();

			const unify::KeyValuePair< std::string, interop::IValue::ptr >* operator->() const;
		};

		iterator begin();

		iterator end();
	};
}