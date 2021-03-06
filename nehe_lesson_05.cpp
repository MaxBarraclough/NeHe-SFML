/*
 *                This Code Was Created By Jeff Molofee 2000
 *                A HUGE Thanks To Fredric Echols For Cleaning Up
 *                And Optimizing The Base Code, Making It More Flexible!
 *                If You've Found This Code Useful, Please Let Me Know.
 *                Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

bool fullscreen = false;                // Fullscreen flag set to fullscreen mode by default
bool vsync      = true;                 // Turn VSYNC on/off

GLfloat rtri;                           // Angle for the triangle ( NEW )
GLfloat rquad;                          // Angle for the quad ( NEW )

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

void initGL()                                                                   // All setup for OpenGL goes here
{
        glShadeModel(GL_SMOOTH);                                                // Enable smooth shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                   // Black background
        glClearDepth(1.0f);                                                     // Depth buffer setup
        glEnable(GL_DEPTH_TEST);                                                // Enables depth testing
        glDepthFunc(GL_LEQUAL);                                                 // The type of depth testing to do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                      // Really nice perspective calculations
}

// In this function we assume that the modelview matrix is the selected matrix
void drawGLScene()                                                              // Here's where we do all the drawing
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // Clear screen and depth buffer
        glLoadIdentity();                                                       // Reset the current modelview matrix
        glTranslatef(-1.5f,0.0f,-6.0f);                                         // Move left 1.5 units and into the screen 6.0
        glRotatef(rtri,0.0f,1.0f,0.0f);                                         // Rotate the triangle on the Y axis ( NEW )

        glBegin(GL_TRIANGLES);                                                  // Start drawing a triangle
          glColor3f(1.0f,0.0f,0.0f);                                            // Red
          glVertex3f( 0.0f, 1.0f, 0.0f);                                        // Top of triangle (front)

          glColor3f(0.0f,1.0f,0.0f);                                            // Green
          glVertex3f(-1.0f,-1.0f, 1.0f);                                        // Left of triangle (front)

          glColor3f(0.0f,0.0f,1.0f);                                            // Blue
          glVertex3f( 1.0f,-1.0f, 1.0f);                                        // Right of triangle (front)

          glColor3f(1.0f,0.0f,0.0f);                                            // Red
          glVertex3f( 0.0f, 1.0f, 0.0f);                                        // Top of triangle (right)

          glColor3f(0.0f,0.0f,1.0f);                                            // Blue
          glVertex3f( 1.0f,-1.0f, 1.0f);                                        // Left of triangle (right)

          glColor3f(0.0f,1.0f,0.0f);                                            // Green
          glVertex3f( 1.0f,-1.0f, -1.0f);                                       // Right of triangle (right)

          glColor3f(1.0f,0.0f,0.0f);                                            // Red
          glVertex3f( 0.0f, 1.0f, 0.0f);                                        // Top of triangle (back)

          glColor3f(0.0f,1.0f,0.0f);                                            // Green
          glVertex3f( 1.0f,-1.0f, -1.0f);                                       // Left of triangle (back)

          glColor3f(0.0f,0.0f,1.0f);                                            // Blue
          glVertex3f(-1.0f,-1.0f, -1.0f);                                       // Right of triangle (back)

          glColor3f(1.0f,0.0f,0.0f);                                            // Red
          glVertex3f( 0.0f, 1.0f, 0.0f);                                        // Top of triangle (left)

          glColor3f(0.0f,0.0f,1.0f);                                            // Blue
          glVertex3f(-1.0f,-1.0f,-1.0f);                                        // Left of triangle (left)

          glColor3f(0.0f,1.0f,0.0f);                                            // Green
          glVertex3f(-1.0f,-1.0f, 1.0f);                                        // Right of triangle (left)
        glEnd();                                                                // Done drawing the pyramid

        glLoadIdentity();                                                       // Reset the current modelview matrix
        glTranslatef(1.5f,0.0f,-7.0f);                                          // Move right 1.5 units and into the screen 7.0
        glRotatef(rquad,1.0f,1.0f,1.0f);                                        // Rotate the quad on the x axis ( new )

        glBegin(GL_QUADS);                                                      // Draw a quad
          glColor3f(0.0f,1.0f,0.0f);                                            // Set the color to green
          glVertex3f( 1.0f, 1.0f,-1.0f);                                        // Top right of the quad (top)
          glVertex3f(-1.0f, 1.0f,-1.0f);                                        // Top left of the quad (top)
          glVertex3f(-1.0f, 1.0f, 1.0f);                                        // Bottom left of the quad (top)
          glVertex3f( 1.0f, 1.0f, 1.0f);                                        // Bottom right of the quad (top)

          glColor3f(1.0f,0.5f,0.0f);                                            // Set the color to orange
          glVertex3f( 1.0f,-1.0f, 1.0f);                                        // Top right of the quad (bottom)
          glVertex3f(-1.0f,-1.0f, 1.0f);                                        // Top left of the quad (bottom)
          glVertex3f(-1.0f,-1.0f,-1.0f);                                        // Bottom left of the quad (bottom)
          glVertex3f( 1.0f,-1.0f,-1.0f);                                        // Bottom right of the quad (bottom)

          glColor3f(1.0f,0.0f,0.0f);                                            // Set the color to red
          glVertex3f( 1.0f, 1.0f, 1.0f);                                        // Top right of the quad (front)
          glVertex3f(-1.0f, 1.0f, 1.0f);                                        // Top left of the quad (front)
          glVertex3f(-1.0f,-1.0f, 1.0f);                                        // Bottom left of the quad (front)
          glVertex3f( 1.0f,-1.0f, 1.0f);                                        // Bottom right of the quad (front)

          glColor3f(1.0f,1.0f,0.0f);                                            // Set the color to yellow
          glVertex3f( 1.0f,-1.0f,-1.0f);                                        // Top right of the quad (back)
          glVertex3f(-1.0f,-1.0f,-1.0f);                                        // Top left of the quad (back)
          glVertex3f(-1.0f, 1.0f,-1.0f);                                        // Bottom left of the quad (back)
          glVertex3f( 1.0f, 1.0f,-1.0f);                                        // Bottom right of the quad (back)

          glColor3f(0.0f,0.0f,1.0f);                                            // Set the color to blue
          glVertex3f(-1.0f, 1.0f, 1.0f);                                        // Top right of the quad (left)
          glVertex3f(-1.0f, 1.0f,-1.0f);                                        // Top left of the quad (left)
          glVertex3f(-1.0f,-1.0f,-1.0f);                                        // Bottom left of the quad (left)
          glVertex3f(-1.0f,-1.0f, 1.0f);                                        // Bottom right of the quad (left)

          glColor3f(1.0f,0.0f,1.0f);                                            // Set the color to violet
          glVertex3f( 1.0f, 1.0f,-1.0f);                                        // Top right of the quad (right)
          glVertex3f( 1.0f, 1.0f, 1.0f);                                        // Top left of the quad (right)
          glVertex3f( 1.0f,-1.0f, 1.0f);                                        // Bottom left of the quad (right)
          glVertex3f( 1.0f,-1.0f,-1.0f);                                        // Bottom right of the quad (right)
        glEnd();                                                                // Done drawing the quad

        rtri  += 0.2f;                                                          // Increase the rotation variable for the triangle ( new )
        rquad -= 0.15f;                                                         // Decrease the rotation variable for the quad ( new )
}

int main()
{
        // Create the main window

        sf::ContextSettings settings;  // We need this to enable OpenGL to use a Z-buffer.
        settings.depthBits = 24;       // By default, one is *not* allocated.

        sf::Window myWindow(sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL", sf::Style::Default, settings);

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
                                        }
                                        break;
                        }
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
