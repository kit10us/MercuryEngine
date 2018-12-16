// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/IThing.h>
#include <unify/Lookup.h>
#include <string>
#include <memory>
#include <list>
#include <functional>

namespace me
{
	namespace interop
	{
		class IValue
		{
		public:
			typedef std::shared_ptr< IValue > ptr;
			typedef std::shared_ptr< const IValue > const_ptr;
			virtual ~IValue() {}

			virtual bool IsWriteable() const = 0;
			virtual bool IsReadable() const = 0;
			virtual void Set( std::string value ) = 0;
			virtual std::string Get() const = 0;
			virtual std::string ToString() const = 0;
		};

		class String : public IValue
		{
			bool m_isWriteable;
			bool m_isReadable;
			std::string m_value;
		public:
			String( std::string value, bool isWriteable = true, bool isReadable = true )
				: m_value{ value }
				, m_isWriteable{ isWriteable }
				, m_isReadable{ isReadable }
			{
			}
		
			~String() 
			{
			}

			bool IsWriteable() const
			{
				return m_isWriteable;
			}

			bool IsReadable() const
			{
				return m_isReadable;
			}

			void Set( std::string value )
			{
				m_value = value;
			}
		
			std::string Get() const override
			{
				return m_value;
			}

			std::string ToString() const override
			{
				return Get();
			}
		};

		class StringReference : public IValue
		{
			bool m_isWriteable;
			bool m_isReadable;
			std::string & m_reference;
		public:
			StringReference( std::string & reference, std::string value = {}, bool isWriteable = true, bool isReadable = true )
				: m_reference{ reference }
				, m_isWriteable{ isWriteable }
				, m_isReadable{ isReadable }
			{
				m_reference = value;
			}

			~StringReference()
			{
			}

			bool IsWriteable() const
			{
				return m_isWriteable;
			}

			bool IsReadable() const
			{
				return m_isReadable;
			}

			void Set( std::string value )
			{
				m_reference = value;
			}

			std::string Get() const override
			{
				return m_reference;
			}

			std::string ToString() const override
			{
				return Get();
			}
		};

		typedef std::function< void( std::string ) > FunctorSet;
		typedef std::function< std::string() > FunctorGet;


		class StringFunctor : public IValue
		{
			FunctorSet * m_functorSet;
			FunctorGet * m_functorGet;
		public:
			StringFunctor( FunctorSet * functorSet = nullptr, FunctorGet * functorGet = nullptr )
				: m_functorSet{ functorSet }
				, m_functorGet{ functorGet }
			{
			}

			bool IsWriteable() const
			{
				return m_functorSet != nullptr;
			}

			bool IsReadable() const
			{
				return m_functorGet != nullptr;
			}

			void Set( std::string value )
			{
				if ( !IsWriteable() )
				{
					throw std::exception( "StringFunctor is not writeable!" );
				}
				(*m_functorSet)( value );
			}

			std::string Get() const override
			{
				if ( !IsWriteable() )
				{
					throw std::exception( "StringFunctor is not readable!" );
				}
				return (*m_functorGet)();
			}

			std::string ToString() const override
			{
				return Get();
			}
		};
	}

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
	};
}