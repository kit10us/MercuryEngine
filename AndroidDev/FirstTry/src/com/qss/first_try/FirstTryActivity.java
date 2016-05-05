package com.qss.first_try;

import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.view.Window;
import android.view.WindowManager;

public class FirstTryActivity extends android.app.Activity
{	
	private GLSurfaceView m_GLSurface;
	
    @Override
    public void onCreate( Bundle savedInstanceState ) 
    {
        super.onCreate( savedInstanceState );
        
        requestWindowFeature( Window.FEATURE_NO_TITLE );
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN );
        
        m_GLSurface = new android.opengl.GLSurfaceView( this );
        m_GLSurface.setRenderer( new MyRenderer( this ) );
        setContentView( m_GLSurface );
    }
    
    @Override
    public void onStart() 
    {
    	super.onStart();
    }
    
    @Override
    public void onPause() 
    {
    	m_GLSurface.onPause();
    	super.onPause();
    }
    
    @Override
    public void onResume()
    {
    	m_GLSurface.onResume();
    	super.onResume();
    }
}