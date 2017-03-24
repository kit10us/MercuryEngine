// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mesh.h>
#include <me/factory/EffectFactories.h>
#include <terrain/Direction.h>
#include <array>

namespace terrain
{
	struct Connection
	{
		me::Geometry::ptr geometry;
	};

	/// <summary>
	/// Describes a piece of ground that connects between two other grounds.
	/// </summary>
	class GroundConnection
	{
	public:
		typedef std::shared_ptr< GroundConnection > ptr;

		GroundConnection();
		~GroundConnection();

		/// <summary>
		/// Add a connection.
		/// </summary>
		void AddConnection( Direction direction, me::Geometry::ptr geometry );

		/// <summary>
		/// Gets a connection.
		/// </summary>
		const Connection & GetConnection( Direction direction );

		/// <summary>
		/// Reverses the directions of a connection.
		/// </summary>
		void Reverse();

	private:
		Connection m_connection[ DirectionCount ];
	};
}