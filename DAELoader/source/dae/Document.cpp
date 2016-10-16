// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/Document.h>
#include <dxi/core/Game.h>

using namespace dae;

Document::Document( dxi::core::IRenderer * renderer, const unify::Path & filePath, dae::util::IEffectSolver * effectSolver )
: m_renderer( renderer )
, m_effectSolver( effectSolver )
{
	qxml::Document doc( filePath );
	qxml::Element * node = doc.GetRoot();

	// Validity check...
	if ( ! node->IsTagName( "COLLADA" ) )
	{
		throw Exception_MissingCOLLADA( node->GetDocument()->GetPath().ToString() );
	}

	// Attributes...
	m_version = node->GetAttribute< std::string >( "version" );

	// Child elements...
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "asset" ) )
		{
			// TODO:
		}
		else if ( childNode->IsTagName( "library_cameras" ) )
		{
			// TODO:
		}
		else if ( childNode->IsTagName( "library_lights" ) )
		{
			// TODO:
		}
		else if ( childNode->IsTagName( "library_nodes" ) )
		{
			m_library_nodes.reset( new LibraryNodes( *this, childNode ) );
		}
		else if ( childNode->IsTagName( "library_images" ) )
		{
			m_library_images.reset( new LibraryImages( *this, childNode ) );
		}
		else if ( childNode->IsTagName( "library_materials" ) )
		{
			m_library_materials.reset( new LibraryMaterials( *this, childNode ) );
		}
		else if ( childNode->IsTagName( "library_effects" ) )
		{
			m_library_effects.reset( new LibraryEffects( *this, childNode ) );
		}
		else if ( childNode->IsTagName( "library_geometries" ) )
		{
			m_library_geometries.reset( new LibraryGeometries( *this, childNode ) );
		}
		else if ( childNode->IsTagName( "library_visual_scenes" ) )
		{
			m_library_visual_scenes.reset( new LibraryVisualScenes( *this, childNode ) );
		}
		else if ( childNode->IsTagName( "scene" ) )
		{
			m_scene.reset( new Scene( *this, childNode ) );
		}
	}
}

const std::string & Document::GetVersion() const
{
	return m_version;
}

dxi::Effect::ptr Document::GetEffect( const Shading & shading ) const
{
	return m_effectSolver->GetEffect( shading );
}

const LibraryNodes & Document::GetLibraryNodes() const
{
	return *m_library_nodes;
}

const LibraryMaterials & Document::GetLibraryMaterials() const
{
	return *m_library_materials;
}

const LibraryImages & Document::GetLibraryImages() const
{
	return *m_library_images;
}

const LibraryEffects & Document::GetLibraryEffects() const
{
	return *m_library_effects;
}

const LibraryGeometries & Document::GetLibraryGeometries() const
{
	return *m_library_geometries;
}

const LibraryVisualScenes & Document::GetLibraryVisualScenes() const
{
	return *m_library_visual_scenes;
}

const Scene & Document::GetScene() const
{
	return *m_scene;
}

void Document::Add( const std::string & name, DocumentNode * node )
{
	std::map< const std::string, DocumentNode * >::const_iterator itr = m_nodes.find( name );
	if ( itr != m_nodes.end() )
	{
		throw unify::Exception( "DAE Document Add: Node \"" + name + "\" already exists!" );
	}
	else
	{
		m_nodes[ name ] = node;
	}
}

const DocumentNode * Document::Find( const std::string & name ) const
{
	std::string fixedName;
	if ( name.at( 0 ) == '#' )
	{
		fixedName = name.substr( 1 );
	}
	else
	{
		fixedName = name;
	}
	std::map< const std::string, DocumentNode * >::const_iterator itr = m_nodes.find( fixedName );
	if ( itr != m_nodes.end() )
	{
		return itr->second;
	}
	else
	{
		return 0;
	}
}

dxi::core::IRenderer * Document::GetRenderer()
{
	return m_renderer;
}
