#include "CustomEvents.h"

////////////////////////////////////////////////

CameraRotation::CameraRotation()
    : m_unitTime( 0 )
{
}

void CameraRotation::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnUpdate & eventData = boost::any_cast< dxi::scene::Object::EventData::OnUpdate >( eventDataAny );

    dxi::scene::Object * object = eventData.get< 0 >();
    unify::Seconds elapsed = eventData.get< 1 >();
	dxi::core::IInput & input = eventData.get< 2 >();

    float rotation( m_unitTime * 3.1415936535f * 2 );
    unify::V3< float > eye( 10.0f * sin( rotation ), 10.0f * sin( rotation ), 10.0f * cos( rotation ) );
    object->GetFrame().SetPosition( eye );
    object->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

    m_unitTime += elapsed * 0.25f;
    if ( m_unitTime > 1.0f )
    {
	    m_unitTime -= 1.0f;
    }
}

////////////////////////////////////////////////

SelectableAnimation::SelectableAnimation() 
: m_time( 0 ) 
{ 
}

void SelectableAnimation::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnUpdate & eventData = boost::any_cast< dxi::scene::Object::EventData::OnUpdate & >( eventDataAny );
    dxi::scene::Object * object = eventData.get< 0 >();
    unify::Seconds elapsed = eventData.get< 1 >();
	dxi::core::IInput & input = eventData.get< 2 >();

    const float MaxTime = 5.0f;

    unify::Matrix matrix = unify::Matrix::MatrixIdentity();
	matrix *= unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ),  unify::Angle::AnglePI() * m_time );
    matrix *= unify::Matrix::MatrixScale( 1.0f + sinf( unify::Angle::AnglePI().GetRadians() * m_time / MaxTime ), 0, 1.0f + sinf( unify::Angle::AnglePI().GetRadians() * m_time / MaxTime ) );
    m_time += elapsed;
    m_time = fmod( m_time, 5.0f );
    object->SetGeometryMatrix( matrix );
}

////////////////////////////////////////////////

OnDragAnimation::OnDragAnimation()
: m_time( 0 ) 
{ 
}

void OnDragAnimation::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnUpdate & eventData = boost::any_cast< dxi::scene::Object::EventData::OnUpdate & >( eventDataAny );
    dxi::scene::Object * object = eventData.get< 0 >();
    unify::Seconds elapsed = eventData.get< 1 >();
	dxi::core::IInput & input = eventData.get< 2 >();

    const float MaxTime = 5.0f;

    unify::Matrix matrix = unify::Matrix::MatrixIdentity();
    matrix *= unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 0, 1, 0 ),  unify::Angle::AnglePI() * m_time );
    matrix *= unify::Matrix::MatrixRotationAboutAxis( unify::V3< float >( 1, 0, 0 ),  unify::Angle::AnglePI() * m_time * 2 );
    matrix *= unify::Matrix::MatrixScale( 1.0f + sinf( unify::Angle::AnglePI().GetRadians() * m_time / MaxTime ), 0, 1.0f + sinf( unify::Angle::AnglePI().GetRadians() * m_time / MaxTime ) );
    m_time += elapsed;
    m_time = fmod( m_time, 5.0f );
    object->SetGeometryMatrix( matrix );
}

////////////////////////////////////////////////

SelectHighlightOnOverEnter::SelectHighlightOnOverEnter( dxi::scene::Object::shared_ptr selectableObjectIn )
: m_selectableObject( selectableObjectIn )
{
}

void SelectHighlightOnOverEnter::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnOverEnter & eventData( boost::any_cast< dxi::scene::Object::EventData::OnOverEnter & >( eventDataAny ) );
    dxi::scene::Object * object = eventData.get< 0 >();
    float distance = eventData.get< 1 >();
	dxi::core::IInput & input = eventData.get< 2 >();

    if ( m_selectableObject.expired() == false )
    {
        dxi::scene::Object * so = m_selectableObject.lock().get();
        so->SetVisible( true );
        so->GetFrame().SetPosition( object->GetFrame().GetPosition() );
        dxi::scene::Camera & camera = object->GetScene()->GetCamera();
        so->GetFrame().LookAt( camera.GetObject()->GetFrame().GetPosition() );
        so->GetFrame().Rotate( unify::V3< float >( 1, 0, 0 ), unify::Angle( unify::Angle::AnglePIHalf() ) );
    }
}

////////////////////////////////////////////////

SelectHighlightOnOverExit::SelectHighlightOnOverExit( dxi::scene::Object::shared_ptr selectableObjectIn ) 
: m_selectableObject( selectableObjectIn ) 
{ 
}

void SelectHighlightOnOverExit::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnOverExit & eventData( boost::any_cast< dxi::scene::Object::EventData::OnOverExit & >( eventDataAny ) );
    dxi::scene::Object * object = eventData.get< 0 >();

    if ( m_selectableObject.expired() == false )
    {
        dxi::scene::Object * so = m_selectableObject.lock().get();
        so->SetVisible( false );
    }
}

SceneLostFocus::SceneLostFocus()
{ 
}

void SceneLostFocus::operator()( boost::any & eventDataAny )
{
    dxi::scene::Scene::EventData::OnFocusLost & eventData = boost::any_cast< dxi::scene::Scene::EventData::OnFocusLost >( eventDataAny );
    dxi::scene::Scene & scene = *eventData.get< 0 >();
    dxi::scene::Object::EventData::OnOverExit onOverExitData( scene.GetObjectOver().get(), 0 );
    if ( scene.GetObjectOver() )
    {
        scene.GetObjectOver()->GetListenerMap().Fire( "onOverExit", boost::any( onOverExitData ) );
    }
}

////////////////////////////////////////////////

OnDown::OnDown() 
{
}

void OnDown::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnDown & eventData = boost::any_cast< dxi::scene::Object::EventData::OnDown >( eventDataAny );
    dxi::scene::Object * object = eventData.get< 0 >();
    unify::V3< float > mousePosition = eventData.get< 1 >();
}

////////////////////////////////////////////////

OnDrag::OnDrag( dxi::scene::Object::shared_ptr dragAnimation ) 
: m_dragAnimation( dragAnimation ) 
{ 
}

void OnDrag::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnDrag & eventData = boost::any_cast< dxi::scene::Object::EventData::OnDrag >( eventDataAny );
    dxi::scene::Object * object = eventData.get< 0 >();
    unify::V3< float > mousePosition = eventData.get< 1 >();
    unify::Ray< float > ray = eventData.get< 2 >();
    bool once = eventData.get< 3 >();
    if ( once )
    {
        m_originalMousePosition = mousePosition;
        m_originalRay = ray;
        m_originalObjectPosition = object->GetFrame().GetPosition();
        m_dragAnimation->SetVisible( true );
    }
    else
    {
        unify::V3< float > mouseDifference = m_originalMousePosition - mousePosition;
        unify::V3< float > newPosition = ray.origin + ray.direction * ( ray.origin.DistanceTo( object->GetFrame().GetPosition() ) );

        object->GetFrame().SetPosition( newPosition );
        m_dragAnimation->GetFrame().SetPosition( newPosition );
    }
}

////////////////////////////////////////////////

OnDragStop::OnDragStop( dxi::scene::Object::shared_ptr dragAnimation ) 
: m_dragAnimation( dragAnimation ) 
{ 
}

void OnDragStop::operator()( boost::any & eventDataAny )
{
    dxi::scene::Object::EventData::OnDragStop & eventData = boost::any_cast< dxi::scene::Object::EventData::OnDragStop >( eventDataAny );
    dxi::scene::Object * object = eventData.get< 0 >();

    m_dragAnimation->SetVisible( false );
}

