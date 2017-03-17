// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/collider/ColliderBase.h>

namespace mephy
{
	namespace collider
	{
		class BoxCollider : public ColliderBase
		{
		protected:
			BoxCollider( BoxCollider & component );
			
		public:
			BoxCollider( unify::BBox< float > bbox );
			~BoxCollider();

		public: // IObjectComponent...
			me::object::IObjectComponent::ptr Duplicate() override;

		private:
			unify::BBox< float > m_bbox;
		};
	}
}
