// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/pathing/PathSystem.h>
#include <dxi/pathing/general/Split.h>
#include <dxi/pathing/general/Parallel.h>
#include <dxi/pathing/position/RotateAbout.h>
#include <dxi/pathing/position/AttachTo.h>
#include <dxi/pathing/orientation/LookAt.h>
#include <dxi/XMLConvert.h>

using namespace dxi;
using namespace pathing;

PathSystem::PathSystem()
{
}

PathSystem::~PathSystem()
{
}

void PathSystem::AddSource( PathSource * source )
{
	m_sources.push_back( source );
}

void PathSystem::RemoveSource( PathSource * source )
{
	m_sources.remove( source );
}

void PathSystem::AddPosition( const std::string & name, const unify::V3< float > & position )
{
	m_positions[ name ] = position;
}

void PathSystem::AddAxis( const std::string & name, const unify::Quaternion & axis )
{
	m_axes[ name ] = axis;
}

void PathSystem::AddFrame( const std::string & name, const unify::Frame & frame )
{
	m_frames[ name ] = frame;
}

void PathSystem::AddPath( const std::string & name, Path::shared_ptr path )
{
	m_paths[ name ] = path;
}

bool PathSystem::FindPosition( const std::string & name, const unify::V3< float > * & position ) const
{
	std::map< std::string, unify::V3< float > >::const_iterator itr = m_positions.find( name );
	if( itr != m_positions.end() )
	{
		position = &itr->second; 
		return true;
	}

	for( std::list< PathSource * >::const_iterator itr = m_sources.begin(), end = m_sources.end(); itr != end; ++itr )
	{
		if( (*itr)->FindPosition( name, position ) )
		{
			return true;
		}
	}

	return false;
}

bool PathSystem::FindAxis( const std::string & name, const unify::Quaternion * & axis ) const
{
	std::map< std::string, unify::Quaternion >::const_iterator itr = m_axes.find( name );
	if( itr != m_axes.end() )
	{
		axis = &itr->second; 
		return true;
	}

	for( std::list< PathSource * >::const_iterator itr = m_sources.begin(), end = m_sources.end(); itr != end; ++itr )
	{
		if( (*itr)->FindAxis( name, axis ) )
		{
			return true;
		}
	}

	return false;
}

bool PathSystem::FindFrame( const std::string & name, const unify::Frame * & frame ) const
{
	std::map< std::string, unify::Frame >::const_iterator itr = m_frames.find( name );
	if( itr != m_frames.end() )
	{
		frame = &itr->second;
		return true;
	}

	for( std::list< PathSource * >::const_iterator itr = m_sources.begin(), end = m_sources.end(); itr != end; ++itr )
	{
		if( (*itr)->FindFrame( name, frame ) )
		{
			return true;
		}
	}

	return false;
}

Path::shared_ptr PathSystem::GetPath( const std::string & name ) const
{
	return m_paths.find( name )->second;
}

void PathSystem::ListPositions( std::vector< std::string > & list ) const
{
	for( std::map< std::string, unify::V3< float > >::const_iterator itr = m_positions.begin(),
		end = m_positions.end(); itr != end; ++itr )
	{
		list.push_back( itr->first );
	}

	for( std::list< PathSource * >::const_iterator itr = m_sources.begin(), end = m_sources.end(); itr != end; ++itr )
	{
		(*itr)->ListPositions( list );
	}
}

void PathSystem::ListAxes( std::vector< std::string > & list ) const
{
	for( std::map< std::string, unify::Quaternion >::const_iterator itr = m_axes.begin(),
		end = m_axes.end(); itr != end; ++itr )
	{
		list.push_back( itr->first );
	}

	for( std::list< PathSource * >::const_iterator itr = m_sources.begin(), end = m_sources.end(); itr != end; ++itr )
	{
		(*itr)->ListAxes( list );
	}
}

void PathSystem::ListFrames( std::vector< std::string > & list ) const
{
	for( std::map< std::string, unify::Frame >::const_iterator itr = m_frames.begin(),
		end = m_frames.end(); itr != end; ++itr )
	{
		list.push_back( itr->first );
	}

	for( std::list< PathSource * >::const_iterator itr = m_sources.begin(), end = m_sources.end(); itr != end; ++itr )
	{
		(*itr)->ListFrames( list );
	}
}

void PathSystem::ListPaths( std::vector< std::string > & list ) const
{
	for( std::map< std::string, Path::shared_ptr >::const_iterator itr = m_paths.begin(),
		end = m_paths.end(); itr != end; ++itr )
	{
		list.push_back( itr->first );
	}
}

void PathSystem::LoadFromFile( const unify::Path & path )
{
	qxml::Document doc( path );
	if( doc.GetRoot()->GetTagName() != "pathing" )
	{
		throw unify::Exception( "XML document is not a valid Pathing document! Root element is not \"pathing\"!" );
	}

	qxml::Element * node = doc.GetRoot()->GetFirstChild();
	while( node )
	{
		if( node->GetTagName() == "position" )
		{
			unify::V3< float > position;
			XMLConvert( node, position );
			AddPosition( node->GetStringAttribute( "name" ), position );
		}
		else if( node->GetTagName() == "axis" )
		{
			unify::Quaternion axis;
			XMLConvert( node, axis );
			AddAxis( node->GetStringAttribute( "name" ), axis );
		}
		else if( node->GetTagName() == "path" )
		{
			std::string name = node->GetStringAttribute( "name" );
			unify::Seconds duration = node->GetFloatAttribute( "duration" );
			Modifier::shared_ptr modifier = FactoryModifierFromXMLElement( node->GetFirstChild() );
			Path * path = new Path( modifier, duration );
			AddPath( name, Path::shared_ptr( path ) ); 
		}
		node = node->GetNext();
	}

}

PathController::shared_ptr PathSystem::CreatePathController( const std::string & pathName, bool looping ) const
{
	PathController::shared_ptr pathController( new PathController( GetPath( pathName ) ) );
	pathController->SetLooping( looping );
	pathController->Restart();
	return pathController;
}

Modifier::shared_ptr PathSystem::FactoryModifierFromXMLElement( const qxml::Element * node ) const
{
	std::string type = node->GetStringAttribute( "type" );
	Modifier::shared_ptr modifier;
	if( unify::StringIs( type, "parallel" ) )
	{
		return CreateModifierParallelFromXMLElement( node );
	}
	else if( unify::StringIs( type, "split" ) )
	{
		return CreateModifierSplitFromXMLElement( node );
	}
	else if( unify::StringIs( type, "rotateAbout" ) )
	{
		return CreateModifierRotateAboutFromXMLElement( node );
	}
	else if( unify::StringIs( type, "attachTo" ) )
	{
		return CreateModifierAttachToFromXMLElement( node );
	}
	else if( unify::StringIs( type, "lookAt" ) )
	{
		return CreateModifierLookAtFromXMLElement( node );
	}
	else
	{
		throw unify::Exception( "Invalid modifier type! (" + type + "\"" );
	}
}

Modifier::shared_ptr PathSystem::CreateModifierParallelFromXMLElement( const qxml::Element * node ) const
{
	general::Parallel::vector list;
	const qxml::Element * parallelChild = node->GetFirstChild();
	while( parallelChild )
	{
		if( parallelChild->GetTagName() == "modifier" )
		{
			list.push_back( FactoryModifierFromXMLElement( parallelChild ) );
		}
		parallelChild = parallelChild->GetNext();
	}
	return Modifier::shared_ptr( new general::Parallel( list ) );
}

Modifier::shared_ptr PathSystem::CreateModifierSplitFromXMLElement( const qxml::Element * node ) const
{
	unify::Seconds timeSpentOnA = node->GetFloatAttribute( "timeSpentOnA" );
	Modifier::shared_ptr modifierA;
	Modifier::shared_ptr modifierB;
	int modifiers = 0;
	const qxml::Element * childNode = node->GetFirstChild();
	while( childNode )
	{
		switch( modifiers )
		{
		case 0:
			modifierA = FactoryModifierFromXMLElement( childNode );
			modifiers += 1;
			break;
		case 1:
			modifierB = FactoryModifierFromXMLElement( childNode );
			modifiers += 1;
			break;
		default:
			throw unify::Exception( "Too many Modifiers added to Pair modifier!" );
		}
		childNode = childNode->GetNext();
	}
	if( modifiers < 2 )
	{
		throw unify::Exception( "Not enough modifiers specified for a Pai modifier!" );
	}
	return Modifier::shared_ptr( new general::Split( modifierA, modifierB, timeSpentOnA ) );
}

Modifier::shared_ptr PathSystem::CreateModifierRotateAboutFromXMLElement( const qxml::Element * node ) const
{
	std::string positionName = node->GetStringAttribute( "position" );
	float distance = node->GetFloatAttribute( "distance" );
	std::string axisFromName = node->GetStringAttribute( "axisFrom" );
	std::string axisToName = node->GetStringAttribute( "axisTo" );

	const unify::V3< float > * position;
	const unify::Quaternion * axisFrom;
	const unify::Quaternion * axisTo;
	if( ! FindPosition( positionName, position ) || ! FindAxis( axisFromName, axisFrom ) || ! FindAxis( axisToName, axisTo ) )
	{
		throw unify::Exception( "Could not find properties for ModifierRotateAbout!" );
	}
	return Modifier::shared_ptr( new position::RotateAbout( position, distance, axisFrom, axisTo ) );
}

Modifier::shared_ptr PathSystem::CreateModifierAttachToFromXMLElement( const qxml::Element * node ) const
{
	float distance = node->GetFloatAttribute( "distance" );
	std::string axisFromTargetName = node->GetStringAttribute( "axisFromTarget" );
	std::string targetFrameName = node->GetStringAttributeElse( "targetFrame", std::string() );
	std::string targetPositionName = node->GetStringAttributeElse( "targetPosition", std::string() );

	const unify::Quaternion * axisFromTarget;
	if( ! FindAxis( axisFromTargetName, axisFromTarget ) )
	{
		throw unify::Exception( "Could not find properties for Modifier::AttachTo!" );
	}
	
	if( ! targetFrameName.empty() )
	{
		const unify::Frame * frame;
		if( ! FindFrame( targetFrameName, frame ) )
		{
			throw unify::Exception( "Could not find properties for Modifier::AttachTo!" );
		}
		return Modifier::shared_ptr( new position::AttachTo( frame, distance, *axisFromTarget ) );
	}
	else
	{
		const unify::V3< float > * position;
		if( ! FindPosition( targetPositionName, position ) )
		{
			throw unify::Exception( "Could not find properties for Modifier::AttachTo!" );
		}
		return Modifier::shared_ptr( new position::AttachTo( position, distance, *axisFromTarget ) );
	}
}

Modifier::shared_ptr PathSystem::CreateModifierLookAtFromXMLElement( const qxml::Element * node ) const
{
	if( node->HasAttributes( "position" ) )
	{
		const unify::V3< float > * position;
		if(  ! FindPosition( node->GetStringAttribute( "position" ), position ) )
		{
			throw unify::Exception( "Could not find properties for Modifier::LookAt!" );
		}
		return Modifier::shared_ptr( new orientation::LookAt( position ) );
	}
	else if( node->HasAttributes( "frame" ) )
	{
		const unify::Frame * frame;
		if(  ! FindFrame( node->GetStringAttribute( "frame" ), frame ) )
		{
			throw unify::Exception( "Could not find properties for Modifier::LookAt!" );
		}
		return Modifier::shared_ptr( new orientation::LookAt( frame ) );
	}
	else
	{
		throw unify::Exception( "Missing attributes for Modifier::LookAt!" );
	}
}
