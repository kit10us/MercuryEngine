// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/loader/MeshLoaders.h>
#include <dxi/Effect.h>
#include <dxi/dae/library_nodes/DAE_LibraryNodes.h>
#include <dxi/dae/library_images/DAE_LibraryImages.h>
#include <dxi/dae/library_materials/DAE_LibraryMaterials.h>
#include <dxi/dae/library_effects/DAE_LibraryEffects.h>
#include <dxi/dae/library_geometries/DAE_LibraryGeometries.h>
#include <dxi/dae/library_visual_scenes/DAE_LibraryVisualScenes.h>
#include <dxi/dae/DAE_Scene.h>
#include <dxi/dae/util/IEffectSolver.h>
#include <dxi/dae/DAE_Exceptions.h>
#include <qxml/QXML.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA
		/// The root element for a COLLADA DAE. To find the area of code where work is needed, identify the DAE node, then drill down from here.
		/// See the "DAE node" appended to every DAE class.
		/// </summary>
		class Document : public IDocument
		{
		public:
			Document( const qlib::file::Path & filePath, util::IEffectSolver * effectSolver, Managers manager );

			/// <summary>
			/// DAE attribute: version
			/// </summary>
			const std::string & GetVersion() const;

			dxi::Effect::shared_ptr GetEffect( const Shading & shading ) const;

			const LibraryNodes & GetLibraryNodes() const;
			const LibraryMaterials & GetLibraryMaterials() const;
			const LibraryImages & GetLibraryImages() const;
			const LibraryEffects & GetLibraryEffects() const;
			const LibraryGeometries & GetLibraryGeometries() const;
			const LibraryVisualScenes & GetLibraryVisualScenes() const;
			const Scene & GetScene() const;

			void Add( const std::string & name, DocumentNode * node );
			const DocumentNode * Find( const std::string & name ) const;

			Managers GetManagers();
			TextureManager::shared_ptr GetTextureManager();
			PixelShaderManager::shared_ptr GetPixelShaderManager();
			VertexShaderManager::shared_ptr GetVertexShaderManager();
			EffectManager::shared_ptr GetEffectManager();

		private:
			std::string m_version;
			boost::shared_ptr< LibraryNodes > m_library_nodes;
			boost::shared_ptr< LibraryMaterials > m_library_materials;
			boost::shared_ptr< LibraryEffects > m_library_effects;
			boost::shared_ptr< LibraryGeometries > m_library_geometries;
			boost::shared_ptr< LibraryVisualScenes > m_library_visual_scenes;
			boost::shared_ptr< LibraryImages > m_library_images;
			boost::shared_ptr< Scene > m_scene;
			util::IEffectSolver * m_effectSolver;
			Managers m_managers;
			std::map< const std::string, DocumentNode * > m_nodes;
		};
	}
}