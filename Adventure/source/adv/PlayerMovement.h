// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/object/Object.h>
#include <memory>

namespace adv
{
	/// <summary>
	/// Direction used for player movement.
	/// </summary>
	enum class MovementDirection
	{
		Up, 
		Down, 
		Left, 
		Right
	};

	/// <summary>
	/// Handles player movement.
	/// </summary>
	class PlayerMovement	
	{
		bool m_newMovement;
		unify::V3< float > m_movementDirection;
		float m_speed;
	public:
		typedef std::shared_ptr< PlayerMovement > ptr;

		PlayerMovement( float speed );

		void Perform( MovementDirection direction );

		void Perform( unify::V3< float > direction );

		void Update( me::object::Object* target, const me::UpdateParams & params );
	};
}