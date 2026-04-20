// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

namespace me::script
{
	template<typename T>
	std::optional<T> GetGlobal(std::string_view name) const
	{
		auto itr = m_globals.find(name);
		if (itr == m_globals.end())
		{
			return {};
		}
		else
		{
			return unify::Cast<T>(*itr);
		}
	}
}