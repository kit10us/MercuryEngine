#pragma once
#include <dxi/events/Listener.h>
#include <dxi/scene/Scene.h>

class CameraRotation : public dxi::events::Listener
{
    float m_unitTime;
public:
    CameraRotation();
    void operator()( boost::any & eventDataAny );
};

class SelectableAnimation : public dxi::events::Listener 
{
    float m_time;
public:
    SelectableAnimation();
    void operator()( boost::any & eventDataAny );
};

class OnDragAnimation : public dxi::events::Listener 
{
    float m_time;
public:
    OnDragAnimation();
    void operator()( boost::any & eventDataAny );
};

class SelectHighlightOnOverEnter : public dxi::events::Listener
{
    dxi::scene::Object::weak_ptr m_selectableObject;
public:
    SelectHighlightOnOverEnter( dxi::scene::Object::shared_ptr selectableObjectIn );
    void operator()( boost::any & eventDataAny );
};

class SelectHighlightOnOverExit : public dxi::events::Listener
{
    dxi::scene::Object::weak_ptr m_selectableObject;
public:
    SelectHighlightOnOverExit( dxi::scene::Object::shared_ptr selectableObjectIn );
    void operator()( boost::any & eventDataAny );
};

class SceneLostFocus : public dxi::events::Listener
{
public:
    SceneLostFocus();
    void operator()( boost::any & eventDataAny );
};

class OnDown : public dxi::events::Listener
{
public:
    OnDown();
    void operator()( boost::any & eventDataAny );
};

class OnDrag : public dxi::events::Listener
{
    dxi::scene::Object::shared_ptr m_dragAnimation;
    unify::V3< float > m_originalMousePosition;
    unify::Ray< float > m_originalRay;
    unify::V3< float > m_originalObjectPosition;
public:
    OnDrag( dxi::scene::Object::shared_ptr dragAnimation );
    void operator()( boost::any & eventDataAny );
};

class OnDragStop : public dxi::events::Listener
{
    dxi::scene::Object::shared_ptr m_dragAnimation;
public:
    OnDragStop( dxi::scene::Object::shared_ptr dragAnimation );
    void operator()( boost::any & eventDataAny );
};
