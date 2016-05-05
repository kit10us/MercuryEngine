package cardice;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.opengles.GL10;

public class Graphic2D {
	private Core m_core;
	
	// Graphical
	private FloatBuffer vertexBuffer;
	private ShortBuffer indexBuffer;
	private FloatBuffer textureCoordinatesBuffer;
	private Texture m_texture;
	
	public Graphic2D( Core core, Texture texture )
	{
		m_core = core;
		m_texture = texture;
		
		float width = texture.getWidth() * 0.5f;
		float height = texture.getHeight() * 0.5f;
		float vertices[] =
			{
				-width, -height, 0,
				width, -height, 0,
				-width, height, 0,
				width, height, 0
			};
			
		short indices[] = 
			{0, 1, 2, 1, 3, 2 };

		float textureCoordinates[] = 
			{   0.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f
			};

		ByteBuffer vbb = ByteBuffer.allocateDirect( vertices.length * 4 );
		vbb.order( ByteOrder.nativeOrder() );
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put( vertices );
		vertexBuffer.position( 0 );
		
		ByteBuffer ibb = ByteBuffer.allocateDirect( indices.length * 2 );
		ibb.order( ByteOrder.nativeOrder() );
		indexBuffer = ibb.asShortBuffer();
		indexBuffer.put( indices );
		indexBuffer.position( 0 );
		
		ByteBuffer tcbb = ByteBuffer.allocateDirect( textureCoordinates.length * 4 );
		tcbb.order( ByteOrder.nativeOrder() );
		textureCoordinatesBuffer = tcbb.asFloatBuffer();
		textureCoordinatesBuffer.put( textureCoordinates );
		textureCoordinatesBuffer.position( 0 );
	}
	
	public void render()
	{
		GL10 gl = m_core.getGL();
		
		// General states
		gl.glFrontFace( GL10.GL_CCW );
		gl.glDisable( GL10.GL_CULL_FACE );
		gl.glCullFace( GL10.GL_BACK );
		
		gl.glEnableClientState( GL10.GL_TEXTURE_COORD_ARRAY );
		gl.glTexCoordPointer( 2 , GL10.GL_FLOAT, 0, textureCoordinatesBuffer );
		m_texture.use();
	
		gl.glEnableClientState( GL10.GL_VERTEX_ARRAY );
		gl.glVertexPointer( 3, GL10.GL_FLOAT, 0, vertexBuffer );
		gl.glDrawElements( GL10.GL_TRIANGLES, 6, GL10.GL_UNSIGNED_SHORT, indexBuffer );
		gl.glDisableClientState( GL10.GL_VERTEX_ARRAY );
		gl.glDisable( GL10.GL_CULL_FACE );

		m_texture.unuse();
		gl.glDisableClientState( GL10.GL_TEXTURE_COORD_ARRAY );
	}
}
