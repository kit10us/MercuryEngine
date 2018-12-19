// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/IThing.h>
#include <me/interop/IValue.h>
#include <unify/Lookup.h>

namespace me
{
	namespace interop
	{
		class Interop 
		{
			unify::Lookup< std::string, interop::IValue::ptr > m_lookup;
		public:
			size_t Add( std::string key, std::string init = std::string{} );

			size_t Add( std::string key, interop::IValue::ptr value );

			size_t Count() const;

			bool Exists( std::string key ) const;

			size_t Find( std::string key ) const;

			std::string GetName( size_t index ) const;

			std::string GetValue( size_t index ) const;

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

				bool operator==( const iterator & itr ) const;

				bool operator!=( const iterator & itr ) const;

				unify::KeyValuePair< std::string, interop::IValue::ptr > & operator*();

				const unify::KeyValuePair< std::string, interop::IValue::ptr > & operator*() const;

				unify::KeyValuePair< std::string, interop::IValue::ptr > * operator->();

				const unify::KeyValuePair< std::string, interop::IValue::ptr > * operator->() const;
			};

			iterator begin();

			iterator end();
		};
	}
}