/*******************************************
*                                          *
*    Max Barraclough's SFML 2 Port of      *
*   Paul Frazee's Vertex Array Example     *
*           nehe.gamedev.net               *
*              2003 / 2013                 *
*                                          *
*******************************************/

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// Mesh generation paramaters
static const float MESH_RESOLUTION  = 4.0f;                                       // Pixels per vertex
static const float MESH_HEIGHTSCALE = 1.0f;                                       // Mesh height scale

static const char * TERRAIN_FILE_PATH = "data/terrain.bmp";

const sf::Clock theClock;


struct Vertex                                                                     // Vertex
{
        float x;                                                                  // X Component
        float y;                                                                  // Y Component
        float z;                                                                  // Z Component
};

struct TexCoord                                                                   // Texture coordinate class
{
        float u;                                                                  // U Component
        float v;                                                                  // V Component
};

class Mesh
{
public:
  // Mesh data
  unsigned int m_nVertexCount;                                                    // Vertex count
  Vertex*      m_pVertices;                                                       // Vertex data
  TexCoord*    m_pTexCoords;                                                      // Texture coordinates
  unsigned int m_nTextureId;                                                      // Texture ID

  // Vertex Buffer Object names
  unsigned int m_nVBOVertices;                                                    // Vertex VBO name
  unsigned int m_nVBOTexCoords;                                                   // Texture coordinate VBO name

  // Heightmap data
  sf::Image m_image;

  Mesh();                                                                         // Mesh constructor
  ~Mesh();                                                                        // Mesh deconstructor

  void loadHeightmap( float flHeightScale, float flResolution );

  float pointHeight( int nX, int nY );

  void buildVBOs();
};


static Mesh         g_mesh;                                                       // Mesh data
static float        g_degreesRotation = 0.0f;                                     // Rotation
static unsigned int g_FPS     = 0;                                                // FPS
static unsigned int g_frames  = 0;                                                // FPS frame counter
static unsigned int g_lastFPS = 0;                                                // Last FPS check time


void initGL()
{
        // Load the mesh data
        g_mesh.loadHeightmap(MESH_HEIGHTSCALE, MESH_RESOLUTION);                  // Load our heightmap

        // Load vertex data into graphics card memory
        g_mesh.buildVBOs();                                                       // Build the VBOs

        // Setup GL states
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                     // Black background
        glClearDepth(1.0f);                                                       // Depth buffer setup
        glDepthFunc(GL_LEQUAL);                                                   // The type of depth testing (less or equal)
        glEnable(GL_DEPTH_TEST);                                                  // Enable depth testing
        glShadeModel(GL_SMOOTH);                                                  // Select smooth shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                        // Set perspective calculations to most accurate
        glEnable(GL_TEXTURE_2D);                                                  // Enable textures
        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );                                      // Set the color to white
}

void updateAngle()
{
        // Rotation[in degrees] = time[in seconds] * 25
        // We do some fixed-point arithmetic to modulo-off any extra

        const int   MILLISECONDS_PER_DEGREE   = 40;
        const float MILLISECONDS_PER_DEGREE_F = (float)MILLISECONDS_PER_DEGREE;

        // Consistently rotate
        g_degreesRotation = (float) (( theClock.getElapsedTime().asMilliseconds() % (360 * MILLISECONDS_PER_DEGREE)) / MILLISECONDS_PER_DEGREE_F);
}

/* IN:  MATRIX = GL_MODELVIEW   */
/* OUT: MATRIX = unchanged      */
void drawGLScene()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                       // Clear screen and depth buffer
        glLoadIdentity();                                                         // Reset the modelview matrix

        // Get FPS
        {
                const int milliseconds = theClock.getElapsedTime().asMilliseconds();
                if ( milliseconds - g_lastFPS >= 1000 )                           // When 1 second has passed...
                {
                        g_lastFPS = milliseconds;                                 // Update our time variable
                        g_FPS     = g_frames;                                     // Save the FPS
                        g_frames  = 0;                                            // Reset the FPS counter

                        printf( "With VBOs: %d Triangles, %d FPS\n", g_mesh.m_nVertexCount / 3, g_FPS );
                }
        }
        ++g_frames;                                                               // Increment our FPS counter

        // Move the camera
        glTranslatef( 0.0f, -220.0f, 0.0f );                                      // Move above the terrain
        glRotatef( 10.0f, 1.0f, 0.0f, 0.0f );                                     // Look down slightly
        glRotatef( g_degreesRotation, 0.0f, 1.0f, 0.0f );                         // Rotate the camera

        // Enable pointers
        glEnableClientState( GL_VERTEX_ARRAY );                                   // Enable vertex arrays
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );                            // Enable texture coord arrays

        // Set pointers to our data
        glBindBuffer( GL_ARRAY_BUFFER, g_mesh.m_nVBOVertices );
        glVertexPointer(   3, GL_FLOAT, 0, (char *) NULL );                       // Set the vertex pointer to the vertex buffer
        glBindBuffer( GL_ARRAY_BUFFER, g_mesh.m_nVBOTexCoords );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );                       // Set the texcoord pointer to the texcoord buffer

        // Render
        glDrawArrays( GL_TRIANGLES, 0, g_mesh.m_nVertexCount );                   // Draw all of the triangles at once

        // Disable pointers
        glDisableClientState( GL_VERTEX_ARRAY );                                  // Disable vertex arrays
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );                           // Disable texture coord arrays
}

Mesh :: Mesh() :
        // Set pointers to NULL
        m_nVertexCount (0),
        m_pVertices    (NULL),
        m_pTexCoords   (NULL),
        m_nTextureId   (0),
        m_nVBOVertices (0),
        m_nVBOTexCoords(0),
        m_image        ()
{}

Mesh :: ~Mesh()
{
        // Delete VBOs
        const unsigned int nBuffers[2] = { m_nVBOVertices, m_nVBOTexCoords };
        glDeleteBuffers( 2, nBuffers );                                           // Free the memory

        // Delete data
        if ( m_pVertices != NULL ) {                                              // Deallocate vertex data
                delete[] m_pVertices;
                m_pVertices = NULL;
        }

        if ( m_pTexCoords != NULL ) {                                             // Deallocate texture coord data
                delete[] m_pTexCoords;
                m_pTexCoords = NULL;
        }
}

void Mesh :: loadHeightmap( float flHeightScale, float flResolution )
{
        // Generate vertex field
        if ( !m_image.loadFromFile(TERRAIN_FILE_PATH) ) {
                fputs("Error loading heightmap file: ", stderr);
                fputs(TERRAIN_FILE_PATH, stderr);
                fputc('\n', stderr);
                exit(7);
        }

        sf::Vector2u imageSize = m_image.getSize();

        m_nVertexCount = (unsigned int)( imageSize.x * imageSize.y * 6 / ( flResolution * flResolution ) );
        m_pVertices  = new Vertex[m_nVertexCount];                                // Allocate vertex data
        m_pTexCoords = new TexCoord[m_nVertexCount];                              // Allocate tex coord data

        unsigned int nIndex = 0;                                                  // Create variables
        for( unsigned int nZ = 0; nZ < imageSize.y; nZ += (unsigned int)flResolution )
        {
                for( unsigned int nX = 0; nX < imageSize.x; nX += (unsigned int)flResolution )
                {
                        for( unsigned int nTri = 0; nTri < 6; ++nTri )
                        {
                                // Using this quick hack, figure the x,z position of the point
                                const float flX = (float)(nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? flResolution : 0.0f ) );
                                const float flZ = (float)(nZ + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? flResolution : 0.0f ) );

                                // Set the data, using ptheight to obtain the y value
                                m_pVertices[nIndex].x = flX - ( imageSize.x / 2 );
                                m_pVertices[nIndex].y = pointHeight( (int) flX, (int) flZ ) * flHeightScale;
                                m_pVertices[nIndex].z = flZ - ( imageSize.y / 2 );

                                // Stretch the texture across the entire mesh
                                m_pTexCoords[nIndex].u = flX / imageSize.x;
                                m_pTexCoords[nIndex].v = flZ / imageSize.y;

                                // Increment our index
                                ++nIndex;
                        }
                }
        }

        // Load the texture into OpenGL
        glGenTextures  (1, &m_nTextureId);                              // Get an available ID number
        glBindTexture  (GL_TEXTURE_2D, m_nTextureId);                   // Bind the texture
        glTexImage2D   (GL_TEXTURE_2D, 0, 3, imageSize.x, imageSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, m_image.getPixelsPtr() );
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

float Mesh :: pointHeight( int nX, int nY )
{
        sf::Vector2u imageSize = m_image.getSize();

        // Calculate the position in the texture, being careful not to overflow
        int  nPos = ( ( nX % imageSize.x ) + ( ( nY % imageSize.y ) * imageSize.x ) ) * 3;
        float flR = (float) m_image.getPixelsPtr()[ nPos ];             // Get the red component
        float flG = (float) m_image.getPixelsPtr()[ nPos + 1 ];         // Get the green component
        float flB = (float) m_image.getPixelsPtr()[ nPos + 2 ];         // Get the blue component
        return ( 0.299f * flR ) + ( 0.587f * flG ) + ( 0.114f * flB );  // Calculate the height using the luminance algorithm
}

void Mesh :: buildVBOs()
{
        // Generate and bind the vertex buffer
        glGenBuffers( 1, &m_nVBOVertices );                             // Get a valid name
        glBindBuffer( GL_ARRAY_BUFFER, m_nVBOVertices );                // Bind the buffer

        // Load the data
        glBufferData( GL_ARRAY_BUFFER, m_nVertexCount * 3 * sizeof(float), m_pVertices, GL_STATIC_DRAW );

        // Generate and bind the texture coordinate buffer
        glGenBuffers( 1, &m_nVBOTexCoords );                            // Get a valid name
        glBindBuffer( GL_ARRAY_BUFFER, m_nVBOTexCoords );               // Bind the buffer

        // Load the data
        glBufferData( GL_ARRAY_BUFFER, m_nVertexCount * 2 * sizeof(float), m_pTexCoords, GL_STATIC_DRAW );

        // Our copy of the data is no longer necessary, it is safe in the graphics card
        delete[] m_pVertices;  m_pVertices  = NULL;
        delete[] m_pTexCoords; m_pTexCoords = NULL;
}


/* IN:  MATRIX = GL_MODELVIEW   */
/* OUT: MATRIX = GL_MODELVIEW   */
void resizeGLScene (int width, int height)                             // Reshape the window when it's moved or resized
{
        glLoadIdentity ();                                             // Reset the modelview matrix

        glMatrixMode(GL_PROJECTION);                                   // Select the projection matrix
        glLoadIdentity();                                              // Reset the projection matrix
        gluPerspective(45.0f, (GLfloat)(width)/(GLfloat)(height),      // Calculate the aspect ratio of the window
                        1.0f, 1000.0f);

        glMatrixMode(GL_MODELVIEW);                                    // Select the modelview matrix
}


int main()
{
        // Create the main window
        sf::Window myWindow(sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL");

        initGL();
        resizeGLScene(800,600);

        // Start game loop
        while (myWindow.isOpen())
        {
                // Process events
                sf::Event event;
                while (myWindow.pollEvent(event))
                {
                        switch (event.type) {
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
                                        }
                                        break;
                        }
                }

                // Turn VSYNC on so that animations run at a more reasonable speed on new CPU's/GPU's.
                myWindow.setVerticalSyncEnabled(true);

                // Set the active window before using OpenGL commands
                // It's useless here because active window is always the same,
                // but don't forget it if you use multiple windows or controls
                myWindow.setActive();

                updateAngle();
                drawGLScene();

                // Finally, display rendered frame on screen
                myWindow.display();
        }

        return EXIT_SUCCESS;
}

