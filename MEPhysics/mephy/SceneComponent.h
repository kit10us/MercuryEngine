// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/scene/SceneComponent.h>
#include <mephy/collider/ColliderBase.h>

namespace mephy
{
	class SceneComponent : public me::scene::SceneComponent
	{
	public:
		static char* Name();

		SceneComponent( me::IOS * os );
		~SceneComponent();

		void AddCollider( collider::ColliderBase::ptr collider );

		void CleanTheDead();

	public: // ISceneComponent...
		void OnEarlyUpdate( const me::UpdateParams & params ) override;
		void OnLateUpdate( const me::UpdateParams & params ) override;

	private:
		// Store colliders for quick physics access
		std::list< collider::ColliderBase::weak_ptr > m_colliders;
	};
}
