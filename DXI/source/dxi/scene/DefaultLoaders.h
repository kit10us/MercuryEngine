// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/scene/SceneManager.h>
#include <dxi/scene/SceneLoader.h>

namespace dxi
{
	namespace scene
	{
		class ImportLF : public SceneLoader::LoaderFunctor
		{
			SceneLoader & m_sceneLoader;
		public:
			ImportLF( SceneLoader & sceneLoader );

			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class AddPathsLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class TextureLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class ShapeLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class MeshLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class TerraLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class AddObjectLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};

		class SetCameraLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		}; 

		class ControllerLF : public SceneLoader::LoaderFunctor
		{
		public:
			void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene );
		};
	}
}



