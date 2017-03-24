// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneComponent.h>
#include <terrain/Map.h>

class TerrainSceneComponent : public me::scene::SceneComponent
{
public:
	TerrainSceneComponent( me::IOS * os, unify::Size< int > mapSize, unify::Size< int > terraSize );
	virtual ~TerrainSceneComponent();

	unify::Size< int > GetSize() const;

	void SetInvalidGeometry( me::Geometry::ptr geo );	   

	size_t AddGround( std::string, terrain::Ground::ptr ground );

	bool DrawOnMap( unify::V2< int > pos, size_t ground );
	bool DrawOnMap( unify::V2< int > pos, std::string name );

public: // IComponent...
	std::string GetWhat() const override {
		return std::string();
	}

protected:
	me::Geometry::ptr m_invalidGeo;
	std::vector< unify::FrameLite > m_frames;
	unify::Size< int > m_mapSize;
	unify::Size< int > m_terraSize;
	
	std::vector< terrain::Ground::ptr > m_descsList;
	std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_descMap;

	size_t * m_map;
};