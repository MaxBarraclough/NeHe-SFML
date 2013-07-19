/*
 *                This Code Was Created By bosco / Jeff Molofee 2000
 *                A HUGE Thanks To Fredric Echols For Cleaning Up
 *                And Optimizing The Base Code, Making It More Flexible!
 *                If You've Found This Code Useful, Please Let Me Know.
 *                Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

bool fullscreen = false;                // Fullscreen flag set to fullscreen mode by default
bool vsync      = true;                 // Turn VSYNC on/off

GLuint texture[1];                      // Storage for 1 texture
GLuint box;                             // Storage for the box display list
GLuint top;                             // Storage for the top display list
GLuint xloop;                           // Loop for x axis
GLuint yloop;                           // Loop for y axis

GLfloat xrot;                           // Rotates cube on the x axis
GLfloat yrot;                           // Rotates cube on the y axis

static GLfloat boxcol[5][3] =
{
        {1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
};

static GLfloat topcol[5][3] =
{
        {.5f,0.0f,0.0f},{0.5f,0.25f,0.0f},{0.5f,0.5f,0.0f},{0.0f,0.5f,0.0f},{0.0f,0.5f,0.5f}
};

// Build cube display lists
GLvoid buildLists()
{
        box = glGenLists(2);                                                            // Generate 2 different lists
        glNewList(box,GL_COMPILE);                                                      // Start with the box list
          glBegin(GL_QUADS);

            // Bottom face
            glNormal3f( 0.0f,-1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

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
        glEndList();

        top = box + 1;                                                                  // Storage for "top" is "box" plus one
        glNewList(top,GL_COMPILE);                                                      // Now the "top" display list
          glBegin(GL_QUADS);

            // Top face
            glNormal3f( 0.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

          glEnd();
        glEndList();
}

void loadGLTextures()                                                                   // Load bitmaps and convert to textures
{
        // Load the bitmap. If file is not found, then quit.
        sf::Image image;
        if (image.loadFromFile("data/cube.bmp"))
        {
                glGenTextures(1, &texture[0]);                                          // Create the texture

                // Typical texture generation using data from the bitmap
                glBindTexture(GL_TEXTURE_2D, texture[0]);

                sf::Vector2u imgSz = image.getSize();

                glTexImage2D(GL_TEXTURE_2D, 0, 4, imgSz.x, imgSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        }
        else {
                exit(1);
        }
}

GLvoid resizeGLScene(GLsizei width, GLsizei height)                                     // Resize and initialize the GL window
{
        if (height == 0)                                                                // Prevent a divide by zero by
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

void initGL()                                                                           // All setup for OpenGL goes here
{
        loadGLTextures();                                                               // Jump to texture loading routine

        buildLists();                                                                   // Jump to the code that creates our display lists

        glEnable(GL_TEXTURE_2D);                                                        // Enable texture mapping
        glShadeModel(GL_SMOOTH);                                                        // Enable smooth shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                           // Black background
        glClearDepth(1.0f);                                                             // Depth buffer setup
        glEnable(GL_DEPTH_TEST);                                                        // Enables depth testing
        glDepthFunc(GL_LEQUAL);                                                         // The type of depth testing to do
        glEnable(GL_LIGHT0);                                                            // Quick and dirty lighting (assumes light0 is set up)
        glEnable(GL_LIGHTING);                                                          // Enable lighting
        glEnable(GL_COLOR_MATERIAL);                                                    // Enable material coloring
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                              // Really nice perspective calculations
}

void drawGLScene()                                                                      // Here's where we do all the drawing
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                             // Clear the screen and the depth buffer

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        for (yloop = 1; yloop < 6; ++yloop)
        {
                for (xloop = 0; xloop < yloop; ++xloop)
                {
                        glLoadIdentity();                                               // Reset the view
                        glTranslatef(   1.4f + (float(xloop)  * 2.8f) - (float(yloop) * 1.4f),
                                      ((6.0f -  float(yloop)) * 2.4f) - 7.0f,
                                        -20.0f);

                        glRotatef( xrot + 45.0f - (2.0f * yloop),
                                   1.0f,
                                   0.0f,
                                   0.0f);

                        glRotatef(yrot + 45.0f, 0.0f, 1.0f, 0.0f);
                        glColor3fv(boxcol[yloop-1]);
                        glCallList(box);
                        glColor3fv(topcol[yloop-1]);
                        glCallList(top);
                }
        }
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
                                case sf::Event::KeyPressed: {
                                        switch (event.key.code) {
                                                case sf::Keyboard::Escape:
                                                        myWindow.close();
                                                        break;

                                                case sf::Keyboard::F1:
                                                        fullscreen = !fullscreen;
                                                        myWindow.create(fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(800, 600, 32),
                                                                        "SFML/NeHe OpenGL",
                                                                        (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
                                                        initGL();
                                                        {
                                                                sf::Vector2u size = myWindow.getSize();
                                                                resizeGLScene(size.x,size.y);
                                                        }
                                                        break;

                                                case sf::Keyboard::F5:
                                                        vsync = !vsync;
                                                        break;
                                        }
                                }
                                break;
                        }
                }

                // Handle movement keys.
                // Precedence: in case of more keys being down,
                // then whichever key is checked first wins.

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        xrot -= 0.2f;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        xrot += 0.2f;
                }

                // Horizontal axis is handled independently from vertical axis

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        yrot += 0.2f;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        yrot -= 0.2f;
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
