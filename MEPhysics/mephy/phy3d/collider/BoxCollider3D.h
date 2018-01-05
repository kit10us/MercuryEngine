// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/phy3d/collider/ColliderBase3D.h>

namespace mephy
{
	namespace phy3d
	{
		namespace collider
		{
			class BoxCollider : public ColliderBase
			{
			protected:
				BoxCollider( BoxCollider & component );

			public:
				static char* Name();

				BoxCollider( unify::V3< float > halfExt, bool moveable = false, float mass = 0.0f );
				~BoxCollider();

				void TestCollision( ColliderBase * collider, const me::UpdateParams & params ) override;

			public: // IObjectComponent...
				me::object::component::IObjectComponent::ptr Duplicate() override;

			private:
				unify::V3< float > m_halfExt;
				unify::BBox< float > m_bbox;
				bool m_once;
			};
		}
	}
}
