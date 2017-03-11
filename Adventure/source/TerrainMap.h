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

#include <GroundDesc.h>

using namespace me;

class TerrainMap : public scene::ObjectComponent
{
protected:
	TerrainMap( TerrainMap & component );

public:
	TerrainMap( unify::Size< int > mapSize, unify::Size< int > terraSize );
	virtual ~TerrainMap();

	std::string GetWhat() const override;
	bool Renderable() const override;
	void CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform ) override;
	void OnSuspend() override;
	void OnResume() override;
	IObjectComponent * Duplicate() override;

	unify::Size< int > GetSize() const;

	void SetInvalidGeometry( Geometry::ptr geo );	   

	size_t AddGround( std::string, GroundDesc::ptr ground );

	bool DrawOnMap( unify::V2< int > pos, size_t ground );
	bool DrawOnMap( unify::V2< int > pos, std::string name );

protected:
	Geometry::ptr m_invalidGeo;
	std::vector< unify::FrameLite > m_frames;
	unify::Size< int > m_mapSize;
	unify::Size< int > m_terraSize;
	
	std::vector< GroundDesc::ptr > m_descsList;
	std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_descMap;

	size_t * m_map;
};
