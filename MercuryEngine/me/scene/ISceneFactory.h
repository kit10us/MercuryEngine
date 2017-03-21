// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <memory>
#include <string>

namespace me
{
	class Game;

	namespace scene
	{
		class IScene;

		class ISceneFactory
		{
		public:
			typedef std::shared_ptr< ISceneFactory > ptr;

			~ISceneFactory() {}

			virtual std::shared_ptr< IScene > Produce(me::Game * game ) = 0;
			virtual std::string GetName() const = 0;
		};

	}
}