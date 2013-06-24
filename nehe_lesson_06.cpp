/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

bool fullscreen = false;          // Fullscreen flag set to fullscreen mode by default
bool vsync = true;            // Turn VSYNC on/off

GLfloat xrot;                           // X rotation ( NEW )
GLfloat yrot;                           // Y rotation ( NEW )
GLfloat zrot;                           // Z rotation ( NEW )

GLuint texture[1];                      // Storage for one texture ( NEW )

int loadGLTextures()                                                                    // Load bitmaps and convert to textures
{
	int status = false;                                                             // Status indicator

	// Load the bitmap, check for errors, if bitmap's not found quit
	sf::Image Image;
	if (Image.loadFromFile("data/NeHe.bmp"))
	{
		status=true;                                                            // Set the status to true

		glGenTextures(1, &texture[0]);                                          // Create the texture

		// Typical texture generation using data from the bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		{
			sf::Vector2u vec = Image.getSize();
			glTexImage2D(GL_TEXTURE_2D, 0, 4, vec.x, vec.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr());
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	return status;                                                                  // Return the status
}

GLvoid resizeGLScene(GLsizei width, GLsizei height)                                     // Resize and initialize the GL window
{
	if (height==0)                                                                  // Prevent a divide by zero by
	{
		height=1;                                                               // Making height equal one
	}

	glViewport(0,0,width,height);                                                   // Reset the current viewport

	glMatrixMode(GL_PROJECTION);                                                    // Select the projection matrix
	glLoadIdentity();                                                               // Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);                                                     // Select the modelview matrix
	glLoadIdentity();                                                               // Reset the modelview matrix
}

int initGL()                                                                            // All setup for OpenGL goes here
{
	if (!loadGLTextures())                                                          // Jump to texture loading routine ( NEW )
	{
		return false;                                                           // If texture didn't load return false
	}

	glEnable(GL_TEXTURE_2D);                                                        // Enable texture mapping ( NEW )
	glShadeModel(GL_SMOOTH);                                                        // Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                           // Black background
	glClearDepth(1.0f);                                                             // Depth buffer setup
	glEnable(GL_DEPTH_TEST);                                                        // Enables depth testing
	glDepthFunc(GL_LEQUAL);                                                         // The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                              // Really nice perspective calculations
	return true;                                                                    // Initialization went ok
}

int drawGLScene()                                                                       // Here's where we do all the drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                             // Clear the screen and the depth buffer
	glLoadIdentity();                                                               // Reset the view
	glTranslatef(0.0f,0.0f,-5.0f);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	// Front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	// Back face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	// Bottom face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;
	return true;                                                                    // Keep going
}

int main()
{
	// Create the main window
	sf::Window myWindow(sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL");

	initGL();
	resizeGLScene(800, 600);

	// Start game loop
	while (myWindow.isOpen())
	{
		// Process events
		sf::Event event;
		while (myWindow.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				myWindow.close();

			// Resize event : adjust viewport
			if (event.type == sf::Event::Resized)
				resizeGLScene(event.size.width, event.size.height);

			// Handle keyboard events
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					myWindow.close();
					break;
				case sf::Keyboard::F1:
					fullscreen = !fullscreen;
					myWindow.create(fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL",
					                (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
					{
						sf::Vector2u size = myWindow.getSize();
						resizeGLScene(size.x,size.y);
					}
					break;
				case sf::Keyboard::F5:
					vsync = !vsync;
					break;
				default:
					break;
				}
			}
		}

		// Turn VSYNC on so that animations run at a more reasonable speed on new CPU's/GPU's.
		myWindow.setVerticalSyncEnabled(vsync);

		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		myWindow.setActive();

		//Draw some pretty stuff
		drawGLScene();

		// Finally, display rendered frame on screen
		myWindow.display();
	}

	return EXIT_SUCCESS;
}
