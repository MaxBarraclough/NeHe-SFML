/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing This Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

bool fullscreen = false;        // Fullscreen flag set to fullscreen mode by default
bool vsync      = true;         // Turn VSYNC on/off

GLvoid resizeGLScene(GLsizei width, GLsizei height)                             // Resize and initialize the GL window
{
//// TODO optimise by using min function, rather than if
	if (height==0)                                                          // Prevent a divide by zero by
	{
		height=1;                                                       // making height equal one
	}

	glViewport(0,0,width,height);                                           // Reset the current viewport

	glMatrixMode(GL_PROJECTION);                                            // Select the projection matrix
	glLoadIdentity();                                                       // Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);                                             // Select the modelview matrix
	glLoadIdentity();                                                       // Reset the modelview matrix
}

bool initGL()                                                                   // All setup for OpenGL goes here
{
	glShadeModel(GL_SMOOTH);                                                // Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                   // Black background
	glClearDepth(1.0f);                                                     // Depth buffer setup
	glEnable(GL_DEPTH_TEST);                                                // Enables depth testing
	glDepthFunc(GL_LEQUAL);                                                 // The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                      // Really nice perspective calculations
	return true;                                                            // Initialization went ok
}

bool drawGLScene()                                                              // Here's where we do all the drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // Clear screen and depth buffer
	glLoadIdentity();                                                       // Reset the current modelview matrix
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
			// Close window : exit


// TODO switch/case ???
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
