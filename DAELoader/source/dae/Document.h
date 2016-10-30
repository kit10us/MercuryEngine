// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dae/library_nodes/LibraryNodes.h>
#include <dae/library_images/LibraryImages.h>
#include <dae/library_materials/LibraryMaterials.h>
#include <dae/library_effects/LibraryEffects.h>
#include <dae/library_geometries/LibraryGeometries.h>
#include <dae/library_controllers/LibraryControllers.h>
#include <dae/library_visual_scenes/LibraryVisualScenes.h>
#include <dae/Scene.h>
#include <dae/util/IEffectSolver.h>
#include <dae/Exceptions.h>
#include <dxi/core/IRenderer.h>
#include <dxi/Effect.h>
#include <qxml/QXML.h>
#include <unify/Path.h>

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
		Document( dxi::core::IRenderer * renderer, const unify::Path & filePath, util::IEffectSolver * effectSolver );

		/// <summary>
		/// DAE attribute: version
		/// </summary>
		const std::string & GetVersion() const;

		dxi::Effect::ptr GetEffect( const Shading & shading ) const;

		const LibraryNodes & GetLibraryNodes() const;
		const LibraryMaterials & GetLibraryMaterials() const;
		const LibraryImages & GetLibraryImages() const;
		const LibraryEffects & GetLibraryEffects() const;
		const LibraryGeometries & GetLibraryGeometries() const;
		const LibraryControllers & GetLibraryControllers() const;
		const LibraryVisualScenes & GetLibraryVisualScenes() const;
		const Scene & GetScene() const;

		void Add( const std::string & name, DocumentNode * node );
		const DocumentNode * Find( const std::string & name ) const;

		dxi::core::IRenderer * GetRenderer() override;

	private:
		dxi::core::IRenderer * m_renderer;
		std::string m_version;
		std::shared_ptr< LibraryNodes > m_library_nodes;
		std::shared_ptr< LibraryMaterials > m_library_materials;
		std::shared_ptr< LibraryEffects > m_library_effects;
		std::shared_ptr< LibraryGeometries > m_library_geometries;
		std::shared_ptr< LibraryControllers > m_library_controllers;
		std::shared_ptr< LibraryVisualScenes > m_library_visual_scenes;
		std::shared_ptr< LibraryImages > m_library_images;
		std::shared_ptr< Scene > m_scene;
		util::IEffectSolver * m_effectSolver;
		std::map< const std::string, DocumentNode * > m_nodes;
	};
}