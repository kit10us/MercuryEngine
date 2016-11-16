// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

namespace me
{
	template< typename T >
	class ComponentInstance
	{
	public:
		ComponentInstance()
			: m_initialized( false )
			, m_started( false )
		{
		}

		ComponentInstance( T component )
			: ComponentInstance()
		{
			m_component = component;
		}

		bool operator==( const ComponentInstance & ch )
		{
			return m_component == ch.m_component;
		}

		T Component()
		{
			return m_component;
		}

		void SetInitialized( bool initialized )
		{
			m_initialized = initialized;
		}

		void SetStarted( bool started )
		{
			m_started = started;
		}

		bool IsInitialized() const 
		{ 
			return m_initialized; 
		}

		bool IsStarted() const
		{
			return m_started;
		}

	private:
		bool m_initialized;
		bool m_started;
		T m_component;
	};
}