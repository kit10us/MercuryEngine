// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

///<summary>
/// Geometry for an object
///
/// An object would have geometry to be rendered, as it's apearance. Presently other
/// objects inherit from Geometry that intend to render. Geometry doesn't contain a
/// location, that's held in the object. Geometry can be safely referenced by more than
/// on object.
///</summary>

#pragma once

#include <me/render/GeometryInstanceData.h>
#include <me/UpdateParams.h>
#include <me/render/RenderParams.h>
#include <me/render/MatrixFeed.h>
#include <rm/IResource.h>
#include <unify/Unify.h>
#include <unify/BSphere.h>
#include <unify/BBox.h>
#include <unify/FrameLite.h>
#include <memory>

namespace me
{
	namespace render
	{
		/// <summary>
		// Abstract graphical object.
		class Geometry : public rm::IResource
		{
		public:
			typedef std::shared_ptr< Geometry > ptr;

			Geometry() = default;
			virtual ~Geometry() = default;

			/// <summary>
			/// Enables sharing of geometry amongst multiple objects/owners.
			/// We push the specific geometry instance's state into a custom GeometryInstanceData class.
			/// The final GeometryInstanceData is expected to be customized per entity.
			/// </summary>
			/// <returns></returns>
			virtual GeometryInstanceData::ptr CreateInstanceData() const;

			/// <summary>
			/// Update our geometry instance. This means updating the owning object's instance data,
			/// the geometry is expected to remain non-instanced.
			/// </summary>
			/// <param name="params">update parameters</param>
			/// <param name="instanceData">instance data</param>
			virtual void Update( const UpdateParams & params, GeometryInstanceData * instanceData ) = 0;
			
			/// <summary>
			/// Render the geometry. Geometry state per object instance is expected to be owned by instance data.
			/// </summary>
			/// <param name="params">render parameters</param>
			/// <param name="instanceData">the owning object instance data</param>
			/// <param name="matrixFeed"></param>
			virtual void Render( const Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed ) = 0;

			unify::BBox< float > & GetBBox();
			const unify::BBox< float > & GetBBox() const;

			virtual bool IsTrans() const = 0;

		public: // rm::IResource
			std::string GetSource() const override {return "";}

		protected:
			unify::BBox< float > m_BBox;
		};
	}
}