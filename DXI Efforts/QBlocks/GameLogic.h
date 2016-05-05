#pragma once

#include <dxi/shapes/ShapeCreators.h>
#include <dxi/scene/Scene.h>

#include <IGameLogic.h>

struct BlockSet
{
    dxi::Geometry::shared_ptr block[ 2 ];
    dxi::Geometry::shared_ptr blockEmphasized[ 2 ];
};

enum BlockType
{
    BlockEmpty,
    BlockA,
    BlockB
};

class GameLogic : public IGameLogic
{
public:
    GameLogic();
    ~GameLogic();

	void Startup();
    bool Update( unify::Seconds elapsed, dxi::IInput & input );
	void Render();
	void Shutdown();

    // Helper functions that allow us to access outside of the grid - simplifies logic.
    bool CollisionCheck( int h, int v ) const;
    BlockType GetBlockType( int h, int v ) const;
    bool IsBlockAlreadyInCombo( int h, int v ) const;

    void GenerateRandomCurrentBlock();
    // Returns true if we had a collision.
    bool GridCollisionCheck();
    void GridDropEvent();
    void ResetCurrentBlock();
    void GameOver();
    void TallyCombos();
    void ScaneLineCompleteColumn( size_t column );
    void ScaneLineCompleteGrid();

private:
    std::vector< BlockSet > m_blockSets;
    unify::Size< size_t > m_gridSize;
    std::vector< BlockType > m_grid;
    size_t m_currentSet;
    dxi::Geometry::shared_ptr m_scanLineGeometry;
    float m_scanTimeInSeconds;
    float m_scanLineProgress;
    unify::V2< int > m_currentPositionDR;
    float m_dropProgress;
    float m_dropRateInSeconds;
    std::vector< BlockType > m_currentBlockPattern;
    bool m_gameOver;
    dxi::scene::Scene m_background;
    std::vector< bool > m_squareIsInCombo;
};