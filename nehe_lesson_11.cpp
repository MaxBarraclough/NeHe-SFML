/*
 *		This Code Was Created By bosco / Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>                        // for sin function

bool fullscreen=false;          // Fullscreen flag set to fullscreen mode by default
bool vsync=true;            // Turn VSYNC on/off

float points[45][45][3];    // The array for the points on the grid of our "wave"
int wiggle_count = 0;           // Counter used to control how fast flag waves

GLfloat xrot;                           // X Rotation ( NEW )
GLfloat yrot;                           // Y Rotation ( NEW )
GLfloat zrot;                           // Z Rotation ( NEW )
GLfloat hold;                           // Temporarily holds a floating point value

GLuint texture[1];                      // Storage for one texture ( NEW )

int loadGLTextures()                                                            // Load bitmaps and convert to textures
{
	int Status=false;                                                       // Status indicator

	// Load the bitmap, check for errors, if bitmap's not found quit
	sf::Image Image;
	if (Image.loadFromFile("data/tim.bmp"))
	{
		Status=true;                                                    // Set the status to true

		glGenTextures(1, &texture[0]);                                  // Create the texture

		// Typical texture generation using data from the bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		sf::Vector2u imgSz = Image.getSize();

		glTexImage2D(GL_TEXTURE_2D, 0, 4, imgSz.x, imgSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
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
	if (!loadGLTextures())                                                  // Jump to texture loading routine ( NEW )
	{
		return false;                                                   // If texture didn't load return false
	}

	glEnable(GL_TEXTURE_2D);                                                // Enable texture mapping ( NEW )
	glShadeModel(GL_SMOOTH);                                                // Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                   // Black background
	glClearDepth(1.0f);                                                     // Depth buffer setup
	glEnable(GL_DEPTH_TEST);                                                // Enables depth testing
	glDepthFunc(GL_LEQUAL);                                                 // The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                      // Really nice perspective calculations
	glPolygonMode( GL_BACK, GL_FILL );                                      // Back face is solid
	glPolygonMode( GL_FRONT, GL_LINE );                                     // Front face is made of lines

	for(int x=0; x<45; x++)
	{
		for(int y=0; y<45; y++)
		{
			points[x][y][0]=float((x/5.0f)-4.5f);
			points[x][y][1]=float((y/5.0f)-4.5f);
			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
		}
	}

	return true;                                                            // Initialization went ok
}

int drawGLScene()                                                               // Here's where we do all the drawing
{
	int x, y;
	float float_x, float_y, float_xb, float_yb;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // Clear the screen and the depth buffer
	glLoadIdentity();                                                       // Reset the view

	glTranslatef(0.0f,0.0f,-12.0f);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	for( x = 0; x < 44; x++ )
	{
		for( y = 0; y < 44; y++ )
		{
			float_x = float(x)/44.0f;
			float_y = float(y)/44.0f;
			float_xb = float(x+1)/44.0f;
			float_yb = float(y+1)/44.0f;

			glTexCoord2f( float_x, float_y);
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2f( float_x, float_yb );
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2f( float_xb, float_yb );
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2f( float_xb, float_y );
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();

	if( wiggle_count == 2 )
	{
		for( y = 0; y < 45; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < 44; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;
		}
		wiggle_count = 0;
	}

	wiggle_count++;

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;

	return true;                                                            // Keep going
}

int main()
{
	// Create the main window
	sf::Window App(sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL");

	initGL();
	resizeGLScene(800, 600);

	// Start game loop
	while (App.isOpen())
	{
		// Process events
		sf::Event Event;
		while (App.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
				App.close();

			// Resize event : adjust viewport
			if (Event.type == sf::Event::Resized)
				resizeGLScene(Event.size.width, Event.size.height);

			// Handle keyboard events
			if (Event.type == sf::Event::KeyPressed) {
				switch (Event.key.code) {
				case sf::Keyboard::Escape:
					App.close();
					break;
				case sf::Keyboard::F1:
					fullscreen = !fullscreen;
					App.create(fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL",
					           (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
					initGL();
					{
						sf::Vector2u size = App.getSize();
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
		App.setVerticalSyncEnabled(vsync);

		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		App.setActive();

		// Draw some pretty stuff
		drawGLScene();

		// Finally, display rendered frame on screen
		App.display();
	}

	return EXIT_SUCCESS;
}