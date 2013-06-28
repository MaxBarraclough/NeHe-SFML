/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

bool fullscreen = false;                // Fullscreen flag set to fullscreen mode by default
bool vsync = true;                      // Turn VSYNC on/off

bool twinkle;                           // Twinkling stars

const unsigned int num = 50;            // Number of stars to draw

typedef struct                          // Create a structure for star
{
	int r, g, b;                    // Stars color
	GLfloat dist,                   // Stars distance from center
	        angle;                  // Stars current angle
}
stars;
stars star[num];                        // Need to keep track of 'num' stars

GLfloat zoom = -15.0f;                  // Distance away from stars
GLfloat tilt = 90.0f;                   // Tilt the view
GLfloat spin;                           // Spin stars

GLuint loop;                            // General loop variable
GLuint texture[1];                      // Storage for one textures

int loadGLTextures()                    // Load bitmaps and convert to textures
{
	int Status = false;             // Status indicator

	// Load the bitmap, check for errors, if bitmap's not found quit
	sf::Image Image;
	if (Image.loadFromFile("data/star.bmp"))
	{
		Status=true;            // Set the status to true

		glGenTextures(1, &texture[0]);  // Create one texture

		// Create linear filtered texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		sf::Vector2u imgSz = Image.getSize();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgSz.x, imgSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr());
	}

	return Status;                                                          // Return the status
}

GLvoid resizeGLScene(GLsizei width, GLsizei height)                             // Resize and initialize the GL window
{
	if (height==0)                                                          // Prevent a divide by zero by
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

int initGL()                                                                    // All setup for OpenGL goes here
{
	if (!loadGLTextures())                                                  // Jump to texture loading routine
	{
		return false;                                                   // If texture didn't load return false
	}

	glEnable(GL_TEXTURE_2D);                                                // Enable texture mapping
	glShadeModel(GL_SMOOTH);                                                // Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                   // Black background
	glClearDepth(1.0f);                                                     // Depth buffer setup
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                      // Really nice perspective calculations
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);                                       // Set the blending function for translucency
	glEnable(GL_BLEND);

	for (loop=0; loop<num; loop++)
	{
		star[loop].angle=0.0f;
		star[loop].dist=(float(loop)/num)*5.0f;
		star[loop].r=rand()%256;
		star[loop].g=rand()%256;
		star[loop].b=rand()%256;
	}
	return true;                                                            // Initialization went ok
}

int drawGLScene()                                                               // Here's where we do all the drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // Clear the screen and the depth buffer
	glBindTexture(GL_TEXTURE_2D, texture[0]);                               // Select our texture

	for (loop=0; loop<num; loop++)                                          // Loop through all the stars
	{
		glLoadIdentity();                                               // Reset the view before we draw each star
		glTranslatef(0.0f,0.0f,zoom);                                   // Zoom into the screen (using the value in 'zoom')
		glRotatef(tilt,1.0f,0.0f,0.0f);                                 // Tilt the view (using the value in 'tilt')
		glRotatef(star[loop].angle,0.0f,1.0f,0.0f);                     // Rotate to the current stars angle
		glTranslatef(star[loop].dist,0.0f,0.0f);                        // Move forward on the x plane
		glRotatef(-star[loop].angle,0.0f,1.0f,0.0f);                    // Cancel the current stars angle
		glRotatef(-tilt,1.0f,0.0f,0.0f);                                // Cancel the screen tilt

		if (twinkle)
		{
			glColor4ub(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}

		glRotatef(spin,0.0f,0.0f,1.0f);
		glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();

		spin+=0.01f;
		star[loop].angle+=float(loop)/num;
		star[loop].dist-=0.01f;
		if (star[loop].dist<0.0f)
		{
			star[loop].dist+=5.0f;
			star[loop].r=rand()%256;
			star[loop].g=rand()%256;
			star[loop].b=rand()%256;
		}
	}
	return true;                                                            // Everything went ok
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
	
						case sf::Keyboard::T:
							twinkle = !twinkle;
							break;
					}
					break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
				zoom -= 0.2f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
				zoom += 0.2f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				tilt -= 0.5f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				tilt += 0.5f;
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
