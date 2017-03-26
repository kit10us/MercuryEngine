// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/scene/SceneComponent.h>
#include <mephy/collider/ColliderBase.h>
#include <mephy/Entity.h>

namespace mephy
{
	class SceneComponent : public me::scene::SceneComponent
	{
	public:
		static char* Name();

		SceneComponent( me::IOS * os );
		~SceneComponent();

		void AddCollider( collider::ColliderBase * collider );
		void AddEntity( Entity * entity );

	public: // ISceneComponent...
		void OnEarlyUpdate( me::UpdateParams params ) override;
		void OnLateUpdate( me::UpdateParams params ) override;

	private:
		std::vector< collider::ColliderBase* > m_colliders;
		std::vector< Entity* > m_entities;
	};
}
