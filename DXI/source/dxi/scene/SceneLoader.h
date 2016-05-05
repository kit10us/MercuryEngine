// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/scene/Scene.h>

namespace dxi
{
	namespace scene
	{
		// Combining lower level loaders, this loader packages all loadable content into a scene, and the scene's various managers.
		class SceneLoader
		{
		public:
			class LoaderFunctor
			{
			public:
				typedef std::shared_ptr< LoaderFunctor > shared_ptr;
				virtual void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene ) = 0;
			};
            
			SceneLoader();
			virtual ~SceneLoader();

			///<summary>
			/// Add a loader functor, attached to an element name. On encountering said element name,
			/// this loader functor will be executed.
			///</summary>
			void AddLoaderFunctor( const std::string & elementName, LoaderFunctor::shared_ptr loaderFunctor );

			std::string ListLoaderFunctors() const;
			void LoadSceneFromXML( Scene::shared_ptr scene, const unify::Path path );
			void LoadSceneFromNode( Scene::shared_ptr scene, qxml::Element * node );

		public:
			std::map< std::string, std::shared_ptr< LoaderFunctor > > m_loaderFunctors;
		};
	}
}