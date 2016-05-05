/*
 * Copyright (c) 2004 Quentin S. Smith
 * All Rights Reserved
 *
 * A gizmo interacts in -some way- with -something-, in theory we should be able to create lists of gizmos to
 * perform actions for us including animations, translations, transitions, etc.
 * We build other gizmos from this base gizmo.
 *
 */
#pragma once

namespace dxi
{
	#define GIZMO_TYPE(_x)	const char * Type() const { return _x; };

	class QGizmo
	{
	public:
		QGizmo() {}
		virtual ~QGizmo() {}
		virtual const char* Type() const = 0;	// Returns a name for the type of gizmo this is
		virtual void Update( unify::Seconds time ) = 0;	// animate gizmo
	};
}