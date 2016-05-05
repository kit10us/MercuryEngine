// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A camera object to give a view reference into the 3D space.
///	Camera--->Object
///</summary>

#pragma once

#include <dxi/scene/Object.h>

#ifdef GetObject
#undef GetObject
#endif  

namespace dxi
{
    namespace scene
    {
	    class Camera
	    {
	    public:
		    typedef std::shared_ptr< Camera > shared_ptr;

		    Camera();
		    Camera( Object::shared_ptr object );
		    virtual ~Camera();
		    bool HasObject() const;
		    void SetObject( Object::shared_ptr object );
            Object::shared_ptr GetObject();

			/// <summary>
			/// Returns a matrix which accumulates the entire camera view (including an optional projection).
			/// </summary>
			unify::Matrix GetMatrix() const;

			/// <summary>
			/// Set a projection matrix to use with this camera.
			/// </summary>
			void SetProjection( const unify::Matrix & projection );
			
			/// <summary>
			/// Clear the projection matrix (equivalent to setting projection to identity).
			/// </summary>
			void ClearProjection();

			void LookAt( const unify::V3< float > & at );

	    protected:
		    Object::weak_ptr m_object; // As a weak pointer, the object the camera is pointing two's lifetime is outside the camera's control.
			
			unify::Matrix m_projection;
			bool m_useProjection;
	    };
    }
}