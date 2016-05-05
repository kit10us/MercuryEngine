/*
 * Copyright (c) 2002 Quentin S. Smith
 * All Rights Reserved
 *
 */
#pragma once

#define DIRECTINPUT_VERSION		0x0800

#include <dxi/QDIK.h>

namespace dxi
{
	#define SHIFTSTATE_NONE				FLAGNULL
	#define SHIFTSTATE_SHIFT			FLAG01
	#define SHIFTSTATE_CAPSLOCK			FLAG02
	#define DIK_ENTER				DIK_RETURN

	#define INPUTDIR_NONE	FLAGNULL
	#define INPUTDIR_UP		FLAG01
	#define INPUTDIR_DOWN	FLAG02
	#define INPUTDIR_LEFT	FLAG03
	#define INPUTDIR_RIGHT	FLAG04

	enum InputStatus
	{
		STATUS_UP,
		STATUS_PRESSED,	// same as STATUS_JUSTUP
		STATUS_DOWN,
		STATUS_JUSTDOWN,
		NUM_STATUS
	};

	class Key
	{
	public:
		enum TYPE {
			Key0 = DIK_0,
			Zero = DIK_0,
			Key1 = DIK_1,
			One = DIK_1,
			Key2 = DIK_2,
			Two = DIK_2,
			Key3 = DIK_3,
			Three = DIK_3,
			Key4 = DIK_4,
			Four = DIK_4,
			Key5 = DIK_5,
			Five = DIK_5,
			Key6 = DIK_6,
			Six = DIK_6,
			Key7 = DIK_7,
			Seven = DIK_7,
			Key8 = DIK_8,
			Eight = DIK_8,
			Key9 = DIK_9,
			Nine = DIK_9,
			A = DIK_A, 
			//DIK_ABNT_C1,

			Add = DIK_ADD, 
			Apostrophe = DIK_APOSTROPHE, 
			Apps = DIK_APPS, 
			At = DIK_AT, 
			Ax = DIK_AX, 
			B = DIK_B,  
			Back = DIK_BACK, 
			Backslash = DIK_BACKSLASH, 
			C = DIK_C, 
		//	DIK_CALCULATOR, 
			Capital = DIK_CAPITAL, 
			Colon = DIK_COLON, 
			Comma = DIK_COMMA, 
			Convert = DIK_CONVERT,
			D = DIK_D, 
			Decimal = DIK_DECIMAL, 
			Delete = DIK_DELETE, 
			Divide = DIK_DIVIDE, 
			Down = DIK_DOWN, 
			E = DIK_E, 
			End = DIK_END,  
			Equals = DIK_EQUALS,
			Escape = DIK_ESCAPE, 
			F = DIK_F, 
			F1 = DIK_F1, 
			F2 = DIK_F2, 
			F3 = DIK_F3, 
			F4 = DIK_F4, 
			F5 = DIK_F5, 
			F6 = DIK_F6, 
			F7 = DIK_F7, 
			F8 = DIK_F8,
			F9 = DIK_F9, 
			F10 = DIK_F10, 
			F11 = DIK_F11, 
			F12 = DIK_F12, 
			F13 = DIK_F13, 
			F14 = DIK_F14, 
			F15 = DIK_F15, 
			G = DIK_G, 
			Grave = DIK_GRAVE, 
			H = DIK_H,
			Home = DIK_HOME, 
			I = DIK_I, 
			Insert = DIK_INSERT, 
			J = DIK_J, 
			K = DIK_K, 
			Kana = DIK_KANA, 
			Kanji = DIK_KANJI, 
			L = DIK_L, 
			LBracket = DIK_LBRACKET,
			LeftBracket = DIK_LBRACKET,
			LControl = DIK_LCONTROL,
			LeftControl = DIK_LCONTROL,
			LCtrl = DIK_LCONTROL,
			LeftCtrl = DIK_LCONTROL,
			Left = DIK_LEFT, 
			LMenu = DIK_LMENU,
			LeftMenu = DIK_LMENU,
			LShift = DIK_LSHIFT,
			LeftShift = DIK_LSHIFT,
			LWind = DIK_LWIN,
			LeftWin = DIK_LWIN,
			M = DIK_M, 
		//	DIK_MAIL, 
		//	DIK_MEDIASELECT,
		//	DIK_MEDIASTOP, 
			Minus = DIK_MINUS, 
			Multiply = DIK_MULTIPLY, 
		//	DIK_MUTE, 
		//	DIK_MYCOMPUTER, 
			N = DIK_N, 
			Next = DIK_NEXT, 
		//	DIK_NEXTTRACK, 
			NoConvert = DIK_NOCONVERT, 
			NumLock = DIK_NUMLOCK, 
			Numpad0 = DIK_NUMPAD0, 
			Numpad1 = DIK_NUMPAD1, 
			Numpad2 = DIK_NUMPAD2,
			Numpad3 = DIK_NUMPAD3, 
			Numpad4 = DIK_NUMPAD4, 
			Numpad5 = DIK_NUMPAD5, 
			Numpad6 = DIK_NUMPAD6, 
			Numpad7 = DIK_NUMPAD7, 
			Numpad8 = DIK_NUMPAD8, 
			Numpad9 = DIK_NUMPAD9,
			NumpadComma = DIK_NUMPADCOMMA, 
			NumpadEnter = DIK_NUMPADENTER, 
			NumpadEquals = DIK_NUMPADEQUALS, 
			NumpadMinus = DIK_NUMPADMINUS, 
			NumpadPeriod = DIK_NUMPADPERIOD, 
			NumpadPlus = DIK_NUMPADPLUS, 
			NumpadSlash = DIK_NUMPADSLASH, 
			NumpadStar = DIK_NUMPADSTAR, 
			O = DIK_O, 
		//	DIK_OEM_102, 
			P = DIK_P, 
			Pause = DIK_PAUSE,
			Period = DIK_PERIOD, 
		//	DIK_PLAYPAUSE, 
			Power = DIK_POWER, 
		//	DIK_PREVTRACK, 
			Prior = DIK_PRIOR, 
			Q = DIK_Q, 
			R = DIK_R, 
			RBracket = DIK_RBRACKET,
			RightBracket = DIK_RBRACKET,
			RControl = DIK_RCONTROL,
			RightControl = DIK_RCONTROL,
			RCtrl = DIK_RCONTROL,
			RightCtrl = DIK_RCONTROL,
			Return = DIK_RETURN,
			Enter = DIK_RETURN,
			Right = DIK_RIGHT, 
			RMenu = DIK_RMENU, 
			RightMenu = DIK_RMENU,
			RShift = DIK_RSHIFT,
			RightShift = DIK_RSHIFT,
			RWin = DIK_RWIN, 
			RightWin = DIK_RWIN,
			S = DIK_S, 
			Scroll = DIK_SCROLL,
			Semicolon = DIK_SEMICOLON, 
			Slash = DIK_SLASH, 
			Sleep = DIK_SLEEP, 
			Space = DIK_SPACE, 
			Stop = DIK_STOP, 
			Subtract = DIK_SUBTRACT, 
			SysRQ = DIK_SYSRQ, 
			T = DIK_T,
			Tab = DIK_TAB, 
			U = DIK_U, 
			Underline = DIK_UNDERLINE, 
			Unlabeled = DIK_UNLABELED, 
			Up = DIK_UP, 
			V = DIK_V, 
		//	DIK_VOLUMEDOWN, 
		//	DIK_VOLUMEUP,
			W = DIK_W, 
		//	DIK_WAKE,  
		//	DIK_WEBBACK, 
		//	DIK_WEBFAVORITES, 
		//	DIK_WEBFORWARD, 
		//	DIK_WEBHOME, 
		//	DIK_WEBREFRESH,
		//	DIK_WEBSEARCH, 
		//	DIK_WEBSTOP, 
			X = DIK_X, 
			Y = DIK_Y, 
			Yen = DIK_YEN, 
			Z = DIK_Z, 
			Backspace = DIK_BACKSPACE, 
			Capslock = DIK_CAPSLOCK, 
			Circumlfex = DIK_CIRCUMFLEX,
			Downarrow = DIK_DOWNARROW, 
			LAlt = DIK_LALT, 
			LeftAlt = DIK_LALT,
			LeftArrow = DIK_LEFTARROW, 
			PGDN = DIK_PGDN,
			PageDown = DIK_PGDN,
			PGUP = DIK_PGUP,
			PageUp = DIK_PGUP,
			RAlt = DIK_RALT,
			RightAlt = DIK_RALT,
			RightArrow = DIK_RIGHTARROW, 
			UpArrow = DIK_UPARROW,
			
			Tilde = DIK_GRAVE
		};
	};
}