package cardice;

public interface Scene {
	/// Update the scene based on changes to the environment.
	void changed( int width, int height );
	
	/// Render the scene.
	void render();
}
