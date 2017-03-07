// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <MEWinMain.h>
#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Mesh.h>
#include <me/Terra.h>
#include <me/factory/EffectFactories.h>
#include <me/scene/GeometryComponent.h>

using namespace me;

class TerrainMap : public scene::ObjectComponent
{
protected:
	TerrainMap( TerrainMap & component );

public:
	TerrainMap( IOS * os, unify::Size< int > mapSize, unify::Size< int > terraSize );
	virtual ~TerrainMap();

	std::string GetType() const override;
	std::string GetWhat() const override;
	bool Renderable() const override;
	void OnInit() override;
	void OnStart() override;
	void OnUpdate( UpdateParams params ) override;
	void CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform ) override;
	void OnSuspend() override;
	void OnResume() override;
	IObjectComponent * Duplicate() override;

	void SetInvalidGeometry( Geometry::ptr geo );

protected:
	Geometry::ptr m_invalidGeo;
	std::vector< unify::FrameLite > m_frames;
	unify::Size< int > m_mapSize;
	unify::Size< int > m_terraSize;
	size_t * m_map;
};
