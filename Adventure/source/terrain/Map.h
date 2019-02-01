// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once 

#include <me/object/component/CameraComponent.h>
#include <me/render/Geometry.h>
#include <terrain/Base.h>
#include <terrain/Ground.h>
#include <terrain/GroundConnection.h>
#include <map>

namespace terrain
{
	/// <notes>
	/// * For debug purposes, there is a Ground for invalid squares - this should never be seen.
	///      It will only be seen when if we attempt to draw a ground thats out of our index. It's
	///      done this way to enable working even if there is a failure.
	/// 
	/// </notes>

	class Map : public me::object::component::ObjectComponent
	{
	protected:
		Map( Map & component );

	public:
		Map( unify::Size< int > mapSize, unify::Size< float > terraSize );
		virtual ~Map();

	private:
		void Initialize();

	public:
		void SetInvalid( me::render::Geometry::ptr invalid );

		unify::Size< int > GetSize() const;

		void SetBase( Base::ptr base );
		
		int AddGround( std::string name, Ground::ptr ground );

		Ground::ptr GetGround( std::string name );

		void AddFriend( std::string ground, std::string groundFriend );

		bool DrawOnMap( unify::V2< int > pos, std::string ground );

	private:
		int GetMap( int x, int y ) const;

		unify::FrameLite * GetFrame( int x, int y );

		int FindGroundIndex( std::string name ) const;

		me::render::Geometry::ptr GetGroundGeometry( const int * type );

	public: // IObjectComponent...
		void CollectGeometry( me::render::GeometryCache & solids, me::render::GeometryCache & trans, const unify::FrameLite * frame ) override;
		void OnSuspend() override;
		void OnResume() override;
		IObjectComponent::ptr Duplicate() override;

	public: // IThing...
		std::string GetWhat() const override;

	protected:
		std::vector< unify::FrameLite > m_frames;
		unify::Size< int > m_mapSize;
		unify::Size< float > m_terraSize;

		me::render::Geometry::ptr m_invalid;

		Base::ptr m_base;

		std::vector< Ground::ptr > m_groundList;
		std::map< std::string, size_t, unify::string::CaseInsensitiveLessThanTest > m_groundMap;

		int * m_map;
	};
}