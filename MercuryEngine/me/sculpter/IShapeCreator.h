// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/sculpter/Sculpter.h>
#include <me/sculpter/IVertexBuilder.h>
#include <me/sculpter/IFaceBuilder.h>
#include <me/render/PrimitiveList.h>
#include <unify/Parameters.h>

namespace me
{
	namespace sculpter
	{
		class IShapeCreator
		{
		public:
			typedef std::shared_ptr< IShapeCreator > ptr;

			virtual ~IShapeCreator() {};

			virtual void Create( me::render::PrimitiveList & primitiveList, const unify::Parameters & parameters ) const = 0;
		};
	}
}