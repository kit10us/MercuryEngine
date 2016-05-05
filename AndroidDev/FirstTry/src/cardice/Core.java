package cardice;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;

public class Core {
	private Context m_context;
	private GL10 m_gl;
	
	public Core( Context context, GL10 gl )
	{
		m_context = context;
		m_gl = gl;
	}
	
	public Context getContext()
	{
		return m_context;
	}
	
	public GL10 getGL()
	{
		return m_gl;
	}
}
