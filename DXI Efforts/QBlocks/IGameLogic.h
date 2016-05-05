#pragma once

#include <boost/shared_ptr.hpp>
#include <unify/TimeDelta.h>
#include <dxi/core/Win32Game.h>


class IGameLogic
{
public:
    typedef boost::shared_ptr< IGameLogic > shared_ptr;

    virtual ~IGameLogic() {}

    // Called once during startup. A good place for one time initializations and acqusitions.
    virtual void Startup() {}
    // Called after startup, and after a Halt, when we want to resume. Resume timers and perform state checks.
    virtual void Resume() {}
    // Called before shutdown, and when we want to halt this game logic.
    virtual void Halt() {}
    // Called to update game logic.
    virtual bool Update( unify::Seconds elapsed, dxi::IInput & input ) { elapsed; input; return true; }
    // Called to present the visible aspect of the game logic.
    virtual void Render() {}
    // Called once, finally. Clean up from Startup.
    virtual void Shutdown() {}
};