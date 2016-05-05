package cardice;

import android.opengl.Matrix;

public class Frame3D {
	private float[] m_localMatrix = new float[ 4 * 4 ];
	
	public Frame3D()
	{
		android.opengl.Matrix.setIdentityM( m_localMatrix, 0 );
	}
	
	public float[] getLocalMatrix()
	{
		return m_localMatrix;
	}

	public void translate( float x, float y, float z )
	{
		Matrix.translateM( m_localMatrix, 0, x, y, z );
	}
	
	//Matrix.rotateM( m_frame.getLocalMatrix(), 0, 45, 0, 1, 0 );
}
