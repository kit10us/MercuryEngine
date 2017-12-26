// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics2D.h>
#include <me/scene/SceneComponent.h>
#include <mephy2d/collider/ColliderBase.h>

namespace mephy2d
{
	class SceneComponent : public me::scene::SceneComponent
	{
	public:
		static char* Name();

		SceneComponent( me::IOS * os );
		~SceneComponent();

		void AddCollider( collider::ColliderBase * collider );

	public: // ISceneComponent...
		void OnEarlyUpdate( const me::UpdateParams & params ) override;
		void OnLateUpdate( const me::UpdateParams & params ) override;

	private:
		std::vector< collider::ColliderBase* > m_colliders;
	};
}
