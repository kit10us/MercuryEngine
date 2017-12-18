// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/game/Game.h>
#include <me/scene/Scene.h>
#include <memory>

namespace me
{
	namespace scene
	{
		class DefaultSceneFactory : public ISceneFactory
		{
			std::string m_name;
		public:
			DefaultSceneFactory( std::string name)
				: m_name{ name }
			{
			}

			IScene::ptr Produce( me::game::Game * gameInstance ) override
			{
				return IScene::ptr(new Scene( gameInstance, m_name ));
			}

			std::string GetName() const override
			{
				return m_name;
			}
		};
	}
}