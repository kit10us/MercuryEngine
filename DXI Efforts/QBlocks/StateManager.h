#pragma once

#include <IGameLogic.h>

class StateManager
{
public:
    StateManager();
    ~StateManager();

    void Startup();
    bool Update( unify::Seconds elapsed, dxi::IInput & input );
    void Render();
    void Shutdown();

    void AddState( const std::string name, IGameLogic::shared_ptr logic );

private:
    typedef std::pair< std::string, IGameLogic::shared_ptr > StateType;
    std::vector< StateType > m_stateVector;
    std::map< std::string, size_t > m_stateLookup;
    size_t m_currentState;
};