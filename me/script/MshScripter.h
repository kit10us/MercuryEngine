// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Cast.h>

#include <map>
#include <optional>
#include <string_view>


namespace me::script
{
	class MshScripter
	{
	public:
		MshScripter();

		void SetGlobal(std::string_view name, std::string_view value);

		template<typename T>
		std::optional<T> GetGlobal(std::string_view name) const;

	private:
		std::map<std::string, std::string> m_globals;
	};

}