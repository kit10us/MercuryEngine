package com.qss.first_try;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class MyGLSurfaceView extends GLSurfaceView {
	public MyGLSurfaceView( Context context ) 
	{
		super( context );
		setEGLContextClientVersion( 3 );
	}
}
