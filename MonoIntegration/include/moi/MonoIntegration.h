#pragma once

#include <string>

namespace moi
{
	class IScript
	{
	public:
		virtual void Go() = 0;
	};

	class IGameScript
	{
	public:
		virtual void Setup() = 0;
		virtual void Startup() = 0;
		virtual void Update( float elapsed ) = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;
	};

	class MonoIntegration
	{
		class Impl * m_impl;
	public:
		MonoIntegration();
		~MonoIntegration();

		IScript * AddScript( std::string name, std::string source );
		IGameScript * AddGameScript( std::string name, std::string className, std::string source );
	};
}