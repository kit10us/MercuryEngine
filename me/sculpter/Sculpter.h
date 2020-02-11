// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/sculpter/SculptureStats.h>
#include <me/sculpter/SheetStats.h>
#include <me/sculpter/IVertexBuilder.h>
#include <me/sculpter/IFaceBuilder.h>
#include <me/sculpter/Sheet.h>
#include <me/render/Effect.h>
#include <me/render/PrimitiveList.h>
#include <vector>

namespace me
{
	namespace sculpter
	{
		/// <summary>
		/// Actual creates the geometry.
		/// </summary>
		class Sculpter
		{
			SculptureStats m_stats;
			std::vector< Sheet > m_sheets;

		public:
			Sculpter();
			~Sculpter();

			/// <summary>
			/// Add a sheet.
			/// </summary>
			void AddSheet( SheetStats sheetStats, IVertexBuilder::ptr vertexBuilder, IFaceBuilder::ptr faceBuilder );

			/// <summary>
			/// Builds the shape from the sheets.
			/// </summary>
			bool Build( me::render::PrimitiveList & primitiveList, me::render::Effect::ptr effect );
		};
	}
}