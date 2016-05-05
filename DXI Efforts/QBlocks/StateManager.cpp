#include <StateManager.h>

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
}

void StateManager::Startup()
{
    AddState( "origin", IGameLogic::shared_ptr() );
    AddState( "final", IGameLogic::shared_ptr() );
    m_currentState = 0;
}

bool StateManager::Update( unify::Seconds elapsed, dxi::IInput & input )
{
    return true;
}

void StateManager::Render()
{
}

void StateManager::Shutdown()
{
}

void StateManager::AddState( const std::string name, IGameLogic::shared_ptr logic )
{
    m_stateVector.push_back( StateType( name, logic ) );
    m_stateLookup[ name ] = m_stateVector.size() - 1;
}
