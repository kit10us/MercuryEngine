// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/Mesh.h>
#include <me/factory/EffectFactories.h>
#include <terrain/Direction.h>
#include <array>

namespace terrain
{
	struct Connection
	{
		me::render::Geometry::ptr geometry;
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
		void AddConnection( Direction direction, me::render::Geometry::ptr geometry );

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