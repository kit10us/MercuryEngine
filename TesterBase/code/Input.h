#pragma once

#include <vector>
#include <pod.h>

class InputLock
{
	friend class Input;
public:
	InputLock();
	~InputLock();

	bool Locked() const;

private:
	void SetLocked( bool locked );
	bool m_locked;
};


class Input
{
public:
	Input();
	~Input();

	bool IsShift() const;
	bool IsCtrl() const;
	bool IsAlt() const;
	bool IsKeyDown( unsigned int key ) const;
	bool WasKeyPressed( unsigned int key ) const;
	V2 GetMousePosition() const;
	bool IsLeftMouseDown() const;
	bool IsRightMouseDown() const;
	bool WasLeftMousePressed() const;
	bool WasRightMousePressed() const;
	size_t GetDisplayIndex() const;

	void SetKey( unsigned int key, bool down );
	void SetShift( bool down );
	void SetCtrl( bool down );
	void SetAlt( bool down );
	void SetMousePosition( size_t displayIndex, V2 position );
	void SetLeftMouse( bool down );
	void SetRightMouse( bool down );

	void CallAfterUpdate();

	bool Locked() const;
	bool Lock( InputLock & lock );
	void Unlock( InputLock & lock );

private:
	bool m_shift;
	bool m_ctrl;
	bool m_alt;
	std::vector< unsigned int > m_keys;
	V2 m_mousePosition;
    unsigned int m_leftMouse;
	unsigned int m_rightMouse;
	InputLock * m_lock;
	size_t m_displayIndex;
};

