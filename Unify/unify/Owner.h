// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <memory>

namespace unify
{
	/// <summary>
	/// Provides ownership symantics, where two or more objects can observer ownership of memory or events.
	/// </summary>
	class Owner
	{
	private:
		Owner(std::string name);
	public:
		typedef std::shared_ptr< Owner > ptr;
		typedef std::weak_ptr< Owner > weak_ptr;

		static ptr Create(std::string name);

		std::string Name() const;
	private:
		std::string m_name;
	};
}