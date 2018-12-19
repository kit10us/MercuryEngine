// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/interop/IValue.h>
#include <memory>

namespace me
{
	namespace interop
	{
		class IFunctorSet
		{
		public:
			typedef std::shared_ptr< IFunctorSet > ptr;
			virtual void operator=( std::string ) = 0;
		};

		class IFunctorGet
		{
		public:
			typedef std::shared_ptr< IFunctorGet > ptr;
			virtual std::string operator()() const = 0;
		};
		
		class Functor : public IValue
		{
			IFunctorSet::ptr m_functorSet;
			IFunctorGet::ptr m_functorGet;
		public:
			Functor( IFunctorSet::ptr functorSet = {}, IFunctorGet::ptr functorGet = {} )
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
					throw std::exception( "Interop Functor is not writeable!" );
				}
				(*m_functorSet) = value;
			}

			std::string Get() const override
			{
				if ( !IsReadable() )
				{
					throw std::exception( "Interop Functor is not readable!" );
				}
				return (*m_functorGet)();
			}

			std::string ToString() const override
			{
				return Get();
			}
		};




		template< typename Thing >
		class SetMyThing
		{
			Thing m_thing;
			std::function< void( Thing thing, std::string ) > m_function;
		public:
			SetMyThing( Thing thing, std::function< void( Thing thing, std::string ) > function )
				: m_thing{ thing }
				, m_function{ function }
			{
			}

			virtual ~SetMyThing() {}

			void operator=( std::string value ) const
			{
				return m_function( thing );
			}
		};

		template< typename Thing >
		class GetMyThing
		{
			const Thing m_thing;
			std::function< std::string( const Thing thing ) > m_function;
		public:
			GetMyThing( const Thing thing, std::function< std::string( const Thing thing ) > function )
			: m_thing{ thing }
			, m_function{ function }
			{
			}

			virtual ~GetMyThing() {}

			std::string operator()() const
			{
				return m_function( m_thing );
			}
		};

		template< typename Thing >
		class MyThing : public IValue
		{
			Thing m_myThing;
			std::function< void( Thing thing, std::string ) > m_setMyThing;
			std::function< std::string( Thing thing ) > m_getMyThing;

		public:
			MyThing( Thing thing, 
				std::function< std::string( Thing thing ) > getMyThing,
				std::function< void( Thing thing, std::string value ) > setMyThing = nullptr )
				: m_myThing{ thing }
				, m_setMyThing{ setMyThing }
				, m_getMyThing{ getMyThing }
			{
			}

			bool IsWriteable() const override
			{
				return m_setMyThing != nullptr;
			}

			void Set( std::string value )
			{
				m_setMyThing( m_myThing, value );
			}

			std::string Get() const
			{
				return m_getMyThing( m_myThing );
			}

			std::string ToString() const
			{
				return Get();
			}
		};

	}
}