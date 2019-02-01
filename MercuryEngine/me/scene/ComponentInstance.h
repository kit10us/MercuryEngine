// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

namespace me
{
	template< typename T >
	class ComponentInstance
	{
	public:
		ComponentInstance()
			: m_started( false )
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

		const T Component() const
		{
			return m_component;
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
		bool m_started;
		T m_component;
	};
}