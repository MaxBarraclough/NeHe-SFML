/*
 *                This Code Was Created By Lionel Brits & Jeff Molofee 2000
 *                A HUGE Thanks To Fredric Echols For Cleaning Up
 *                And Optimizing The Base Code, Making It More Flexible!
 *                If You've Found This Code Useful, Please Let Me Know.
 *                Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <stdio.h>                      // Header file for standard input/output
#include <cmath>                        // for sin and cos

bool fullscreen = false;                // Fullscreen flag set to fullscreen mode by default
bool vsync      = true;                 // Turn VSYNC on/off

bool blend;                             // Blending ON/OFF

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

GLfloat yrot;                           // y rotation
GLfloat walkBias = 0;
GLfloat walkBiasangle = 0;
GLfloat lookUpDown = 0.0f;
GLfloat z = 0.0f;                       // Depth into the screen

GLuint filter;                          // Which filter to use
GLuint texture[3];                      // Storage for 3 textures

typedef struct tagVERTEX
{
        float x, y, z;
        float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
        VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR
{
        int numTriangles;
        TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;                         // Our model goes here:

void readstr(FILE *f,char *string)
{
        do
        {
                fgets(string, 255, f);
        } while ((string[0] == '/') || (string[0] == '\n'));
        return;
}

void setupWorld()
{
        float x, y, z, u, v;
        int numTriangles;
        char oneline[255];

        FILE *filein = fopen("data/world.txt", "rt");                 // File from which to load world data

        if (NULL == filein)                                           // Ensure file opened ok.
        {                                                             // If not, file was probably not found.
                fputs("File not found: data/world.txt\nExiting\n", stderr);
                exit(1);
        }

        readstr(filein,oneline);
        sscanf(oneline, "NUMPOLLIES %d\n", &numTriangles);

        sector1.triangle = new TRIANGLE[numTriangles];
        sector1.numTriangles = numTriangles;
        for (int loop = 0; loop < numTriangles; ++loop)
        {
                for (int vert = 0; vert < 3; vert++)
                {
                        readstr(filein,oneline);
                        sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
                        sector1.triangle[loop].vertex[vert].x = x;
                        sector1.triangle[loop].vertex[vert].y = y;
                        sector1.triangle[loop].vertex[vert].z = z;
                        sector1.triangle[loop].vertex[vert].u = u;
                        sector1.triangle[loop].vertex[vert].v = v;
                }
        }
        fclose(filein);
}

void loadGLTextures()                                                                   // Load bitmaps and convert to textures
{
        // Load the bitmap. If file is not found, then quit.
        sf::Image image;
        if (image.loadFromFile("data/mud.bmp"))
        {
                glGenTextures(3, &texture[0]);                                          // Create three textures

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
        loadGLTextures();                                                       // Jump to texture loading routine

        glEnable(GL_TEXTURE_2D);                                                // Enable texture mapping
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);                                       // Set the blending function for translucency
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                                   // This will clear the background color to black
        glClearDepth(1.0);                                                      // Enables clearing of the depth buffer
        glDepthFunc(GL_LESS);                                                   // The type of depth test to do
        glEnable(GL_DEPTH_TEST);                                                // Enables depth testing
        glShadeModel(GL_SMOOTH);                                                // Enables smooth color shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                      // Really nice perspective calculations

        setupWorld();
}

void drawGLScene()                                                              // Here's where we do all the drawing
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                     // Clear the screen and the depth buffer
        glLoadIdentity();                                                       // Reset the view

        GLfloat x_m, y_m, z_m, u_m, v_m;
        GLfloat xTrans = -xpos;
        GLfloat zTrans = -zpos;
        GLfloat yTrans = -walkBias-0.25f;
        GLfloat sceneRotY = 360.0f - yrot;

        glRotatef(lookUpDown,1.0f,0,0);
        glRotatef(sceneRotY,0,1.0f,0);

        glTranslatef(xTrans, yTrans, zTrans);
        glBindTexture(GL_TEXTURE_2D, texture[filter]);

        int numTriangles = sector1.numTriangles;

        // Process each triangle
        for (int loop_m = 0; loop_m < numTriangles; ++loop_m)
        {
                glBegin(GL_TRIANGLES);
                  glNormal3f( 0.0f, 0.0f, 1.0f);
                  x_m = sector1.triangle[loop_m].vertex[0].x;
                  y_m = sector1.triangle[loop_m].vertex[0].y;
                  z_m = sector1.triangle[loop_m].vertex[0].z;
                  u_m = sector1.triangle[loop_m].vertex[0].u;
                  v_m = sector1.triangle[loop_m].vertex[0].v;
                  glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);

                  x_m = sector1.triangle[loop_m].vertex[1].x;
                  y_m = sector1.triangle[loop_m].vertex[1].y;
                  z_m = sector1.triangle[loop_m].vertex[1].z;
                  u_m = sector1.triangle[loop_m].vertex[1].u;
                  v_m = sector1.triangle[loop_m].vertex[1].v;
                  glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);

                  x_m = sector1.triangle[loop_m].vertex[2].x;
                  y_m = sector1.triangle[loop_m].vertex[2].y;
                  z_m = sector1.triangle[loop_m].vertex[2].z;
                  u_m = sector1.triangle[loop_m].vertex[2].u;
                  v_m = sector1.triangle[loop_m].vertex[2].v;
                  glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
                glEnd();
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

                                                case sf::Keyboard::F:
                                                        ++filter;
                                                        if (filter > 2) { //// TODO use modulo
                                                                filter = 0;
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

                // Handle movement keys

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
                                        z-=0.02f;
                                        lookUpDown-= 1.0f;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
                                        z+=0.02f;
                                        lookUpDown+= 1.0f;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                                        xpos -= (float)sin(heading*piover180) * 0.05f;
                                        zpos -= (float)cos(heading*piover180) * 0.05f;
                                        if (walkBiasangle >= 359.0f) {
                                                walkBiasangle = 0.0f;
                                        } else {
                                                walkBiasangle+= 10;
                                        }
                                        walkBias = (float)sin(walkBiasangle * piover180) / 20.0f;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                                        xpos += (float)sin(heading*piover180) * 0.05f;
                                        zpos += (float)cos(heading*piover180) * 0.05f;
                                        if (walkBiasangle <= 1.0f) {
                                                walkBiasangle = 359.0f;
                                        } else {
                                                walkBiasangle -= 10;
                                        }
                                        walkBias = (float)sin(walkBiasangle * piover180) / 20.0f;
                }

                // Horizontal axis is handled independently from vertical axis

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                                        heading -= 1.0f;
                                        yrot = heading;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                                        heading += 1.0f;
                                        yrot = heading;
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
