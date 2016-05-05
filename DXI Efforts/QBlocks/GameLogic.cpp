#include <GameLogic.h>

GameLogic::GameLogic()
{
}

GameLogic::~GameLogic()
{
}

void GameLogic::Startup()
{
    m_gridSize.width = 12;
    m_gridSize.height = 8;
    m_grid.resize( m_gridSize.Units(), BlockEmpty );

    m_grid[ m_gridSize.LinearIndex( 0, 0 ) ] = BlockA;
    m_grid[ m_gridSize.LinearIndex( m_gridSize.width - 1, m_gridSize.height - 1 ) ] = BlockB;

    m_squareIsInCombo.resize( m_gridSize.Units(), false );

    dxi::shapes::CubeParameters boxParameters;
    boxParameters.SetSize( unify::Size3< float >( 1, 1, 1 ) );

    BlockSet set;

    boxParameters.SetDiffuse( unify::Color::ColorRed() + unify::Color::ColorGrey( 20 ) );
    set.block[ 0 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorBlue() + unify::Color::ColorGrey( 20 ) );
    set.block[ 1 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorRed() + unify::Color::ColorGrey( 150 ) );
    set.blockEmphasized[ 0 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorBlue() + unify::Color::ColorGrey( 150 ) );
    set.blockEmphasized[ 1 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetSize( unify::Size3< float >( 0.1f, 2.0f + 2.0f * static_cast< float >( m_gridSize.height ), 3.0f ) );
    boxParameters.SetDiffuse( unify::Color::ColorGreen( 255, 1 ) );
    dxi::Mesh * scanLineMesh = dxi::shapes::CreateShape( boxParameters );
    scanLineMesh->GetPrimitiveList().GetEffect( 0 ).SetBlend( dxi::Blend( dxi::Usage::True ) );
    m_scanLineGeometry.reset( scanLineMesh );

    m_blockSets.push_back( set );
    m_currentSet = 0;

    dxi::Geometry::shared_ptr backgroundBlockWhatever[ 2 ];
    dxi::Geometry::shared_ptr backgroundBlockGoodArea[ 2 ];
    dxi::Geometry::shared_ptr backgroundBlockBadArea[ 2 ];
    
    boxParameters.SetSize( unify::Size3< float >( 1, 1, 1 ) );

    boxParameters.SetDiffuse( unify::Color::ColorGrey( 200 ) );
    backgroundBlockWhatever[ 0 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorGrey( 100 ) );
    backgroundBlockWhatever[ 1 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorGreen() + unify::Color::ColorGrey( 200 ) );
    backgroundBlockGoodArea[ 0 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorGreen() + unify::Color::ColorGrey( 100 ) );
    backgroundBlockGoodArea[ 1 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorRed() + unify::Color::ColorGrey( 200 ) );
    backgroundBlockBadArea[ 0 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    boxParameters.SetDiffuse( unify::Color::ColorRed() + unify::Color::ColorGrey( 100 ) );
    backgroundBlockBadArea[ 1 ].reset( dxi::shapes::CreateShape( boxParameters ) );

    bool flipFlop = true;
    unify::V3< float > origin( -7.5f, -6.0f, 1.0f );
    int squares1 = m_gridSize.width + 2;
    int squares2 = 17;
    for( int i2 = 0; i2 != squares2; ++i2 )
    {
        flipFlop = !flipFlop;
        for( int i1 = 0; i1 != squares1; ++i1 )
        {
            dxi::scene::Object::shared_ptr object;

            // Back
            object.reset( new dxi::scene::Object );
            if( i2 <= static_cast< int >( m_gridSize.height ) )
            {
                object->SetGeometry( backgroundBlockGoodArea[ ( i1 + i2 ) % 2 ] );
            }
            else if( i2 < static_cast< int >( m_gridSize.height ) + 3 )
            {
                object->SetGeometry( backgroundBlockBadArea[ ( i1 + i2 ) % 2 ] );
            }
            else
            {
                object->SetGeometry( backgroundBlockWhatever[ ( i1 + i2 ) % 2 ] );
            }
            unify::V3< float > position( static_cast< float >( i1 ) * 1.0f, static_cast< float >( i2 ) * 1.0f, 0.0f );
            position += origin + unify::V3< float >( 0, 0, 0 );
            object->GetFrame().SetPosition( position );
            m_background.Add( object );

            // Bottom
            object.reset( new dxi::scene::Object );
            if( i2 == 2 )
            {
                object->SetGeometry( backgroundBlockGoodArea[ ( i1 + i2 ) % 2 ] );
            }
            else
            {
                object->SetGeometry( backgroundBlockWhatever[ ( i1 + i2 ) % 2 ] );
            }
            position = unify::V3< float >( static_cast< float >( i1 ) * 1.0f, 0.0f, static_cast< float >( i2 ) * -1.0f );
            position += origin + unify::V3< float >( 0, 0, 1.0f );
            object->GetFrame().SetPosition( position );
            m_background.Add( object );

            // Left
            object.reset( new dxi::scene::Object );
            if ( i2 == 0 && i1 < static_cast< int >( m_gridSize.height ) )
            {
                object->SetGeometry( backgroundBlockGoodArea[ ( i1 + i2 ) % 2 ] );
            }
            else if ( i2 == 0 && i1 < static_cast< int >( m_gridSize.height ) + 2 )
            {
                object->SetGeometry( backgroundBlockBadArea[ ( i1 + i2 ) % 2 ] );
            }
            else
            {
                object->SetGeometry( backgroundBlockWhatever[ ( i1 + i2 ) % 2 ] );
            }
            position = unify::V3< float >( 0, static_cast< float >( i1 ) * 1.0f, static_cast< float >( i2 ) * -1.0f );
            position += origin + unify::V3< float >( 0, 1.0f, -1.0f );
            object->GetFrame().SetPosition( position );
            m_background.Add( object );

            // Right
            object.reset( new dxi::scene::Object );
            if ( i2 == 0 && i1 < static_cast< int >( m_gridSize.height ) )
            {
                object->SetGeometry( backgroundBlockGoodArea[ ( i1 + i2 ) % 2 ] );
            }
            else if ( i2 == 0 && i1 < static_cast< int >( m_gridSize.height ) + 2 )
            {
                object->SetGeometry( backgroundBlockBadArea[ ( i1 + i2 ) % 2 ] );
            }
            else
            {
                object->SetGeometry( backgroundBlockWhatever[ ( i1 + i2 ) % 2 ] );
            }
            position = unify::V3< float >( ( squares1 ) * 1.0f , static_cast< float >( i1 ) * 1.0f, static_cast< float >( i2 ) * -1.0f );
            position += origin + unify::V3< float >( 0, 1.0f, -1.0f );
            object->GetFrame().SetPosition( position );
            m_background.Add( object );
        }
    }

    dxi::scene::Object::shared_ptr cameraObject;
    cameraObject.reset( new dxi::scene::Object );
    cameraObject->GetFrame().SetPosition( unify::V3< float >( 0, 0, -20 ) );
    cameraObject->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
    m_background.Add( "camera", cameraObject );
    m_background.SetCamera( "camera" );

    m_scanTimeInSeconds = 5;
    m_scanLineProgress = 0;

    m_currentPositionDR.x = m_gridSize.width / 2;

    m_dropRateInSeconds = 1.0f;

    ResetCurrentBlock();

    m_gameOver = false;
}

bool GameLogic::Update( unify::Seconds elapsed, dxi::IInput & input )
{
    m_background.Update( elapsed, input );
    if ( m_gameOver )
    {
        return true;
    }

    float previousScanLineProgress = m_scanLineProgress;
    m_scanLineProgress += elapsed / m_scanTimeInSeconds;
    if ( m_scanLineProgress > 1.0f )
    {
        m_scanLineProgress = 1.0f;
    }

    // Determine when our scanline passes a major point.
    float f1 = floor( previousScanLineProgress * m_gridSize.width );
    float f2 = floor( m_scanLineProgress * m_gridSize.width );
    if ( f1 != f2 )
    {
        if ( f2 > f1 + 1 )
        {
            m_scanLineProgress = (f1 + 1) / m_gridSize.width;
        }

        // Guarantee we hit every major point, including the final.
        if ( m_scanLineProgress == 1.0f )
        {
            m_scanLineProgress = 0.0f;
        }
        ScaneLineCompleteColumn( static_cast< int >( f1 ) );
    }

    if ( input.KeyPressed( dxi::Key::Left ) )
    {
        if ( ! CollisionCheck( m_currentPositionDR.x - 1, m_currentPositionDR.y ) && ! CollisionCheck( m_currentPositionDR.x - 1, m_currentPositionDR.y + 1 ) )
        {
            m_currentPositionDR.x -= 1;
            float cameraX = static_cast< float >( m_currentPositionDR.x - static_cast< int >(m_gridSize.width / 2) );
            m_background.GetCamera().GetObject()->GetFrame().SetPosition( unify::V3< float >( cameraX, 0, -20 ) );
            m_background.GetCamera().GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
        }
    }
    else if ( input.KeyPressed( dxi::Key::Right ) )
    {
        if ( ! CollisionCheck( m_currentPositionDR.x + 2, m_currentPositionDR.y ) && ! CollisionCheck( m_currentPositionDR.x + 2, m_currentPositionDR.y + 1 ) )
        {
            m_currentPositionDR.x += 1;
            float cameraX = static_cast< float >( m_currentPositionDR.x - static_cast< int >(m_gridSize.width / 2) );
            m_background.GetCamera().GetObject()->GetFrame().SetPosition( unify::V3< float >( cameraX, 0, -20 ) );
            m_background.GetCamera().GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
        }
    }

    // Rotate Left (clockwise)
    if ( input.KeyPressed( dxi::Key::Z ) )
    {
        BlockType temp = m_currentBlockPattern[ 0 ];
        m_currentBlockPattern[ 0 ] = m_currentBlockPattern[ 2 ];
        m_currentBlockPattern[ 2 ] = m_currentBlockPattern[ 3 ];
        m_currentBlockPattern[ 3 ] = m_currentBlockPattern[ 1 ];
        m_currentBlockPattern[ 1 ] = temp;
    }
    // Rotate Right (counter-clockwise)
    else if ( input.KeyPressed( dxi::Key::X ) || input.KeyPressed( dxi::Key::Up ) )
    {
        BlockType temp = m_currentBlockPattern[ 0 ];
        m_currentBlockPattern[ 0 ] = m_currentBlockPattern[ 1 ];
        m_currentBlockPattern[ 1 ] = m_currentBlockPattern[ 3 ];
        m_currentBlockPattern[ 3 ] = m_currentBlockPattern[ 2 ];
        m_currentBlockPattern[ 2 ] = temp;
    }

    if ( input.KeyPressed( dxi::Key::Down ) )
    {
        m_dropProgress = 0.0f;
        if ( ! GridCollisionCheck() )
        {
            m_currentPositionDR.y -= 1;
        }
    }
    else
    {
        m_dropProgress += elapsed / m_dropRateInSeconds;
        if ( m_dropProgress > 1.0f )
        {
            m_dropProgress = 0.0f;
            if ( ! GridCollisionCheck() )
            {
                m_currentPositionDR.y -= 1;
            }
        }
    }

	return true;
}

void GameLogic::Render()
{
    assert( m_currentSet < m_blockSets.size() );
    
    m_background.Render();

    dxi::scene::Object drawObject;

    // Draw current grid
    for ( unsigned int v = 0; v != m_gridSize.height; ++v )
    {
        for ( unsigned int h = 0; h != m_gridSize.width; ++h )
        {
            unify::Size< float > blockSize( 1, 1 );
            unify::V3< float > position( 
                static_cast< float >( m_gridSize.width ) * -0.5f + static_cast< float>( h ) * blockSize.width, 
                -5.0f + static_cast< float >( v ) * blockSize.height, 
                0.0f 
                );
            BlockSet & set = m_blockSets[ m_currentSet ];

            size_t index = m_gridSize.LinearIndex( h, v );
            BlockType blockType = m_grid[ index ];
            switch ( blockType )
            {
            case BlockEmpty:
                drawObject.SetGeometry( dxi::Geometry::shared_ptr() );
                break;
            case BlockA:
                drawObject.SetGeometry( m_squareIsInCombo[ index ] ? set.blockEmphasized[ 0 ] : set.block[ 0 ] );
                break;
            case BlockB:
                drawObject.SetGeometry( m_squareIsInCombo[ index ] ? set.blockEmphasized[ 1 ] : set.block[ 1 ] );
                break;
            }

            drawObject.GetFrame().SetPosition( position );
            drawObject.Render();
        }
    }

    // Draw current block...
    for ( unsigned int v = 0; v != 2; ++v )
    {
        for ( unsigned int h = 0; h != 2; ++h )
        {
            unify::Size< float > blockSize( 1, 1 );
            unify::V3< float > position( 
                static_cast< float >( m_gridSize.width ) * -0.5f + static_cast< float>( h + m_currentPositionDR.x ) * blockSize.width, 
                -5.0f + static_cast< float >( v + m_currentPositionDR.y ) * blockSize.height, 
                0.0f 
                );
            BlockSet & set = m_blockSets[ m_currentSet ];

            BlockType blockType = m_currentBlockPattern[ h + v * 2 ];
            switch ( blockType )
            {
            case BlockEmpty:
                drawObject.SetGeometry( dxi::Geometry::shared_ptr() );
                break;
            case BlockA:
                drawObject.SetGeometry( set.block[ 0 ] );
                break;
            case BlockB:
                drawObject.SetGeometry( set.block[ 1 ] );
                break;
            }

            drawObject.GetFrame().SetPosition( position );
            drawObject.Render();
        }
    }

    if ( ! m_gameOver )
    {
        // Draw scanline...
        float scanLineProgress = m_scanLineProgress;
        unify::V3< float > scanLinePosition(
            static_cast< float >( m_gridSize.width ) * -0.5f + static_cast< float >( m_gridSize.width ) * scanLineProgress, 
            0.0f, 
            0.0f 
            );
        drawObject.SetGeometry( m_scanLineGeometry );
        drawObject.GetFrame().SetPosition( scanLinePosition );
        drawObject.Render();
    }
}

void GameLogic::Shutdown()
{
}

bool GameLogic::CollisionCheck( int h, int v ) const
{
    // Always colide below ground.
    if ( h < 0 )
    {
        return true;
    }

    // Always colide to the left or right of grid.
    if ( h < 0 || h >= static_cast< int >( m_gridSize.width ) )
    {
        return true;
    }

    // Never colide above grid.
    if ( v >= static_cast< int >( m_gridSize.height ) )
    {
        return false;
    }

    // The rest is within the grid, so a basic check of the grid.
    return m_grid[ m_gridSize.LinearIndex( h, v ) ] != 0;
}

BlockType GameLogic::GetBlockType( int h, int v ) const
{
    // below ground.
    if ( v < 0 )
    {
        return BlockEmpty;
    }

    // to the left or right of grid.
    if ( h < 0 || h >= static_cast< int >( m_gridSize.width ) )
    {
        return BlockEmpty;
    }

    // above grid.
    if ( v >= static_cast< int >( m_gridSize.height ) )
    {
        return BlockEmpty;
    }

    // The rest is within the grid, so a basic check of the grid.
    BlockType type = m_grid[ m_gridSize.LinearIndex( h, v ) ];
    return type;
}

bool GameLogic::IsBlockAlreadyInCombo( int h, int v ) const
{
    // below ground.
    if ( v < 0 )
    {
        return false;
    }

    // to the left or right of grid.
    if ( h < 0 || h >= static_cast< int >( m_gridSize.width ) )
    {
        return false;
    }

    // above grid.
    if ( v >= static_cast< int >( m_gridSize.height ) )
    {
        return false;
    }

    // The rest is within the grid, so a basic check of the grid.
    return m_squareIsInCombo[ m_gridSize.LinearIndex( h, v ) ];
}

void GameLogic::GenerateRandomCurrentBlock()
{
    m_currentBlockPattern.resize( 2 * 2 );
    m_currentBlockPattern[ 0 ] = static_cast< BlockType >( rand() % 2 + 1 );
    m_currentBlockPattern[ 1 ] = static_cast< BlockType >( rand() % 2 + 1 );
    m_currentBlockPattern[ 2 ] = static_cast< BlockType >( rand() % 2 + 1 );
    m_currentBlockPattern[ 3 ] = static_cast< BlockType >( rand() % 2 + 1 );
}

bool GameLogic::GridCollisionCheck()
{
    // Check for our collision.
    if ( m_currentPositionDR.y == 0 || m_grid[ m_gridSize.LinearIndex( m_currentPositionDR.x, m_currentPositionDR.y - 1 ) ] != 0 || m_grid[ m_gridSize.LinearIndex( m_currentPositionDR.x + 1, m_currentPositionDR.y - 1 ) ] != 0 )
    {
        // Check for above bounds (game over).
        if ( m_currentPositionDR.y >= static_cast< int >( m_gridSize.height ) )
        {
            GameOver();
        }
        else
        {
            // Transfer current block to grid.
            m_grid[ m_gridSize.LinearIndex( m_currentPositionDR.x, m_currentPositionDR.y ) ] = m_currentBlockPattern[ 0 + 0 * 2 ];
            m_grid[ m_gridSize.LinearIndex( m_currentPositionDR.x + 1, m_currentPositionDR.y ) ] = m_currentBlockPattern[ 1 + 0 * 2 ];
            m_grid[ m_gridSize.LinearIndex( m_currentPositionDR.x, m_currentPositionDR.y + 1 ) ] = m_currentBlockPattern[ 0 + 1 * 2 ];
            m_grid[ m_gridSize.LinearIndex( m_currentPositionDR.x + 1, m_currentPositionDR.y + 1 ) ] = m_currentBlockPattern[ 1 + 1 * 2 ];
            GridDropEvent();
            TallyCombos();
            ResetCurrentBlock();
        }
        return true;
    }
    else
    {
        return false;
    }
}

void GameLogic::GameOver()
{
    m_gameOver = true;
}

void GameLogic::GridDropEvent()
{
    bool changed = false;
    do
    {
        changed = false;
        for( size_t v = 1; v < m_gridSize.height; ++v )
        {
            for( size_t h = 0; h < m_gridSize.width; ++h )
            {
                if ( m_grid[ m_gridSize.LinearIndex( h, v ) ] != 0 && m_grid[ m_gridSize.LinearIndex( h, v - 1 ) ] == 0 )
                {
                    std::swap( m_grid[ m_gridSize.LinearIndex( h, v ) ], m_grid[ m_gridSize.LinearIndex( h, v - 1 ) ] );
                    changed = true;
                }
            }
        }
    } while( changed );
}

void GameLogic::ResetCurrentBlock()
{
    GenerateRandomCurrentBlock();
    m_currentPositionDR.y = m_gridSize.height;
}

void GameLogic::TallyCombos()
{
    for( size_t v = 0; v < m_gridSize.height - 1; ++v )
    {
        for( size_t h = 0; h < m_gridSize.width - 1; ++h )
        {
            if ( 
                GetBlockType( h + 0, v + 0 ) != BlockEmpty && 
                GetBlockType( h + 1, v + 0 ) == GetBlockType( h + 0, v + 0 ) && 
                GetBlockType( h + 0, v + 1 ) == GetBlockType( h + 0, v + 0 ) &&
                GetBlockType( h + 1, v + 1 ) == GetBlockType( h + 0, v + 0 ) 
                ) 
            {
                size_t count = 0;
                count += IsBlockAlreadyInCombo( h + 0, v + 0 ) ? 0 : 1;
                count += IsBlockAlreadyInCombo( h + 1, v + 0 ) ? 0 : 1;
                count += IsBlockAlreadyInCombo( h + 0, v + 1 ) ? 0 : 1;
                count += IsBlockAlreadyInCombo( h + 1, v + 1 ) ? 0 : 1;
                
                m_squareIsInCombo[ m_gridSize.LinearIndex( h + 0, v + 0 ) ] = true;
                m_squareIsInCombo[ m_gridSize.LinearIndex( h + 1, v + 0 ) ] = true;
                m_squareIsInCombo[ m_gridSize.LinearIndex( h + 0, v + 1 ) ] = true;
                m_squareIsInCombo[ m_gridSize.LinearIndex( h + 1, v + 1 ) ] = true;
            }
        }
    }
}

void GameLogic::ScaneLineCompleteColumn( size_t column )
{
    // Clear all combos in our column
    size_t h = column;
    size_t clears = 0;
    for ( size_t v = 0; v < m_gridSize.height; ++v )
    {
        if ( IsBlockAlreadyInCombo( h, v ) )
        {
            size_t index = m_gridSize.LinearIndex( h, v );
            m_grid[ index ] = BlockEmpty;
            m_squareIsInCombo[ index ] = false;
            clears++;
        }
    }

    if ( clears != 0 )
    {
        GridDropEvent();
    }

    if( column == m_gridSize.width - 1 )
    {
        ScaneLineCompleteGrid();
    }
}

void GameLogic::ScaneLineCompleteGrid()
{
}

