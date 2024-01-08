#pragma once

#include <memory>
#include <string>

namespace me
{
	namespace os
	{
		class IEnvironment
		{
		public:
			typedef std::shared_ptr< IEnvironment> ptr;
			typedef std::weak_ptr<IEnvironment> weak_ptr;

			// <summary>
			// The lexical name of the enviornment, such as Windows.
			// </summary>
			virtual std::string GetName() const = 0;

			// <summary>
			// The platform, such as x64.
			// </summary>
			virtual std::string GetPlatform() const = 0;
		};
	}
}
