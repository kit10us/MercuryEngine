// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Skeleton.h>
#include <me/Mesh.h>
#include <sg/ShapeCreators.h>
#include <sg/SphereParameters.h>

using namespace me;

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::AddJoint( std::string name, unify::Matrix matrix, std::string parent )
{
	size_t index = m_jointMatrix.size();
	m_jointMatrix.push_back( matrix );
	m_names[name] = index;
	if( parent.empty() )
	{
		m_parent.push_back( Root );
		m_jointFinalMatrix.push_back( matrix );
	}
	else
	{
		size_t parentIndex = FindJointIndex( parent );
		m_parent.push_back( parentIndex );
		m_jointFinalMatrix.push_back( m_jointFinalMatrix[ parentIndex ] * matrix );
	}
}

const unify::Matrix * Skeleton::FindJoint( std::string name ) const
{
	size_t index = FindJointIndex( name );
	if( index < m_jointMatrix.size() )
	{
		return &m_jointMatrix[index];
	}

	return nullptr;
}

bool Skeleton::Empty() const
{
	return m_jointMatrix.empty();
}

void Skeleton::Render( RenderParams params, MatrixFeed & matrixFeed, Effect::ptr effect, float size, unify::Color color )
{
	if( Empty() ) return;

	if( ! m_jointGeo )
	{
		me::Mesh * mesh = new me::Mesh( params.renderer );
		m_jointGeo.reset( mesh );

		sg::SphereParameters parameters;
		parameters.SetRadius( size );
		parameters.SetDiffuse( color );
		parameters.SetEffect( effect );
		sg::CreateShape_Sphere( params.renderer, mesh->GetPrimitiveList(), parameters );
	}

	unify::Matrix matrix;
	while( matrixFeed.Consume( &matrix, 1 ) )
	{
		for( size_t i = 0; i < m_jointFinalMatrix.size(); i++ )
		{
			unify::Matrix matrixFinal =	m_jointFinalMatrix[i];
			MatrixFeed feed{ &matrixFinal, 1 };
			m_jointGeo->Render( params, nullptr, feed );
		}
	}
	matrixFeed.Restart();
}

size_t Skeleton::FindJointIndex( std::string name ) const
{
	auto itr = m_names.find( name );
	if( itr == m_names.end() )
	{
		return Root;
	}
	else
	{
		return itr->second;
	}
}
