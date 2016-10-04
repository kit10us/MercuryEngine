// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/Effect.h>
#include <dae/library_effects/Shading.h>

namespace dae
{
	class DocumentNode;
	//class Shading;

	/// <summary>
	/// </summary>
	class IDocument
	{
	public:
		virtual ~IDocument(){}
		virtual const std::string & GetVersion() const = 0;

		/// <summary>
		/// Returns an effect which can best represent the shading's intention.
		/// </summary>
		virtual dxi::Effect::ptr GetEffect( const Shading & shading ) const = 0;

		virtual const class LibraryNodes & GetLibraryNodes() const = 0;
		virtual const class LibraryMaterials & GetLibraryMaterials() const = 0;
		virtual const class LibraryImages & GetLibraryImages() const = 0;
		virtual const class LibraryEffects & GetLibraryEffects() const = 0;
		virtual const class LibraryGeometries & GetLibraryGeometries() const = 0;
		virtual const class LibraryVisualScenes & GetLibraryVisualScenes() const = 0;
		virtual const class Scene & GetScene() const = 0;
		virtual void Add( const std::string & name, DocumentNode * node ) = 0;
		virtual const DocumentNode * Find( const std::string & name ) const = 0;

	};
}