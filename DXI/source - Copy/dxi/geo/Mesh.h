// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A mesh geometry.
///	Supported file types: x, xml, ase
/// Supports Shape creation methods.
///</summary>

#pragma once

#include <dxi/geo/Geometry.h>
#include <dxi/shapes/ShapeCreators.h>
#include <dxi/RenderMethod.h>
#include <dxi/RenderInfo.h>
#include <dxi/TextureManager.h>
#include <dxi/PixelShaderManager.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/EffectManager.h>
#include <dxi/MeshInstanceData.h>
#include <unify/unify.h>
#include <dxi/loader/MeshLoaders.h>

namespace dxi
{
	namespace geo
	{
		class Mesh : public Geometry
		{
		public:
			Mesh();
			Mesh( const unify::Path & filePath, dae::util::IEffectSolver * effectSolver, Managers managers );
			
			// A shape based on Parameters.
			Mesh( unify::Parameters & parameters );
			~Mesh();

			GeometryInstanceData * CreateInstanceData();

			void Destroy();

			void CreateFromFile( const unify::Path & filePath, dae::util::IEffectSolver * effectSolver, Managers manager );
			void CreateFromShape( unify::Parameters & parameters );

			// Overrides..
			void Update( unify::Seconds elapsed, GeometryInstanceData * instanceData );
			void Render( RenderInfo renderInfo, GeometryInstanceData * instanceData );

			const unify::BBox< float > & ComputeBounds();
			PrimitiveList & GetPrimitiveList();

		private:
			std::string m_source;
			PrimitiveList m_primitiveList;

			bool m_renderNormals;
			float m_normalLength;
			VertexBuffer m_normalsVB;

			bool m_needsSync;
		};
	} // namespace geo
} // namespace dxi