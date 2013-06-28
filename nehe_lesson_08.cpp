/*
 *		This Code Was Created By Tom Stanis / Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

bool fullscreen = false;                // Fullscreen flag set to fullscreen mode by default
bool vsync = true;                      // Turn VSYNC on/off
bool light;                             // Lighting ON/OFF
bool blend;                             // Blending OFF/ON? ( NEW )

GLfloat xrot;                           // x rotation
GLfloat yrot;                           // y rotation
GLfloat xspeed;                         // x rotation speed
GLfloat yspeed;                         // y rotation speed
GLfloat z = -5.0f;                      // Depth into the screen

GLfloat LightAmbient[]  =       { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]  =       { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] =       { 0.0f, 0.0f, 2.0f, 1.0f };

GLuint filter;                          // Which filter to use
GLuint texture[3];                      // Storage for 3 textures

void loadGLTextures()                   // Load bitmaps and convert to textures
{
	// Load the bitmap. If file is not found, then quit.
	sf::Image image;
	if (image.loadFromFile("data/glass.bmp"))
	{
		glGenTextures(3, &texture[0]);                          // Create three textures

		// Create nearest filtered texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		sf::Vector2u imgSz = image.getSize();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgSz.x, imgSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

		// Create linear filtered texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgSz.x, imgSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

		// Create mipmapped texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgSz.x, imgSz.y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	}
	else {
		exit(1);
	}
}

GLvoid resizeGLScene(GLsizei width, GLsizei height)                             // Resize and initialize the GL window
{
	if (height == 0)                                                        // Prevent a divide by zero by
	{
		height=1;                                                       // Making height equal one
	}

	glViewport(0,0,width,height);                                           // Reset the current viewport

	glMatrixMode(GL_PROJECTION);                                            // Select the projection matrix
	glLoadIdentity();                                                       // Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);                                             // Select the modelview matrix
	glLoadIdentity();                                                       // Reset the modelview matrix
}

void initGL()                                                                   // All setup for OpenGL goes here
{
	loadGLTextures();                                                       // Jump to texture loading routine

	glEnable(GL_TEXTURE_2D);                                                // Enable texture mapping
	glShadeModel(GL_SMOOTH);                                                // Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                   // Black background
	glClearDepth(1.0f);                                                     // Depth buffer setup
	glEnable(GL_DEPTH_TEST);                                                // Enables depth testing
	glDepthFunc(GL_LEQUAL);                                                 // The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                      // Really nice perspective calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);                         // Setup the ambient light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);                         // Setup the diffuse light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);                        // Position the light
	glEnable(GL_LIGHT1);                                                    // Enable light one


	glColor4f(1.0f, 1.0f, 1.0f, 0.5);                                       // Full brightness.  50% alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);                                       // Set the blending function for translucency
}

void drawGLScene()                                                              // Here's where we do all the drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // Clear the screen and the depth buffer
	glLoadIdentity();                                                       // Reset the view
	glTranslatef(0.0f,0.0f,z);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[filter]);

	glBegin(GL_QUADS);
	// Front face
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	// Back face
	glNormal3f( 0.0f, 0.0f,-1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top face
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	// Bottom face
	glNormal3f( 0.0f,-1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	// Right face
	glNormal3f( 1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();

	xrot+=xspeed;
	yrot+=yspeed;
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
			switch (event.type) {
				// Close window : exit
				case sf::Event::Closed:
					myWindow.close();
					break;

				// Resize event : adjust viewport
				case sf::Event::Resized:
					resizeGLScene(event.size.width, event.size.height);
					break;

				// Handle keyboard events
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							myWindow.close();
							break;

						case sf::Keyboard::F1:
							fullscreen = !fullscreen;
							myWindow.create(fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(800, 600, 32),
									"SFML/NeHe OpenGL",
							                (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
							{
								sf::Vector2u size = myWindow.getSize();
								resizeGLScene(size.x,size.y);
							}
							break;

						case sf::Keyboard::F5:
							vsync = !vsync;
							break;

						case sf::Keyboard::L:
							light=!light;
							if (!light) {
								glDisable(GL_LIGHTING);
							} else {
								glEnable(GL_LIGHTING);
							}
							break;

						case sf::Keyboard::F:
							filter+=1;
							if (filter>2) {
								filter=0;
							}
							break;

						case sf::Keyboard::B:
							blend = !blend;
							if(blend) {
								glEnable(GL_BLEND);       // Turn blending on
								glDisable(GL_DEPTH_TEST); // Turn depth testing off
							} else {
								glDisable(GL_BLEND);      // Turn blending off
								glEnable(GL_DEPTH_TEST);  // Turn depth testing on
							}
							break;
					}
					break;
			}
		}

		// Handle movement keys.
		// Precedence: in case of more keys being down,
		// then whichever key is checked first wins.

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
			z -= 0.02f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
			z += 0.02f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			xspeed -= 0.01f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			xspeed += 0.01f;
		}

		// Horizontal axis is handled independently from vertical axis

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			yspeed += 0.01f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			yspeed -= 0.01f;
		}

		// Turn VSYNC on so that animations run at a more reasonable speed on new CPU's/GPU's.
		myWindow.setVerticalSyncEnabled(vsync);

		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		myWindow.setActive();

		// Draw some pretty stuff
		drawGLScene();

		// Finally, display rendered frame on screen
		myWindow.display();
	}

	return EXIT_SUCCESS;
}
