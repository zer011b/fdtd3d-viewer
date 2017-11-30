#include <cstdlib>
#include <cmath>
#include <GL/glut.h>

#define VERSION 0.01

enum class Mode : uint32_t
{
  NONE,
  SINGLE_FILE,
  DIRECTORY
};

enum class Dimension : uint32_t
{
  NONE,
  1D,
  2D,
  3D
};

Mode viewerMode = Mode::NONE;
Dimension viewerDim = Dimension::NONE;
std::string path;
int msec = 0;
bool isAnimationMode = false;
int sizeX = 0;
int sizeY = 0;
int sizeZ = 0;
bool isComplexMode = false;

/* Sample func itself */
float func(float x)
{
	return A*x*x;
}
 
/* Function plotting func */
void draw(float (* func)(float x), float x1, float x2, float y1, float y2, int N)
{
	float x, dx = 1.0/N;
 
	glPushMatrix(); /* GL_MODELVIEW is default */
 
	glScalef(1.0 / (x2 - x1), 1.0 / (y2 - y1), 1.0);
	glTranslatef(-x1, -y1, 0.0);
	glColor3f(1.0, 1.0, 1.0);
 
	glBegin(GL_LINE_STRIP);
 
	for(x = x1; x < x2; x += dx)
	{
		glVertex2f(x, func(x));
	}
 
	glEnd();
 
	glPopMatrix();
};
 
/* Redrawing func */
void redraw(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	draw(func, -3, 3, 0, 10, 10);
 
	glutSwapBuffers();
};
 
/* Idle proc. Redisplays, if called. */
void idle(void)
{
        A++;
	glutPostRedisplay();
};
 
/* Key press processing */
void key(unsigned char c, int x, int y)
{
  switch (key)
  {
    case 27:
    {
      // Esc
      exit (0);
      break;
    }
    case ' ':
    {
      // Space: start/stop animation
      if (viewerMode == Mode::Directory)
      {
        isAnimationMode = !isAnimationMode;
      }
    }
    default:
    {
    }
  }
};
 
/* Window reashape */
void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
};

void loadSingleFile (const char *filename)
{
  FILE *f = fopen (filename, "r");

  int totalCount = sizeX * sizeY * sizeZ;

  for (int index = 0; i < totalCount; ++i)
  {
    if (viewerDim == Dimension::1D)
    {
      fscanf (f, "%d %f\n", 
    }
  }

  fclose (f);
}

int main (int argc, char **argv)
{
  glutInit(&argc, argv);

  for (int i = 1; i < argc; ++i)
  {
    if (strcmp (argv[i], "--version") == 0)
    {
      printf ("fdtd3d viewer. Version " VERSION ". See --help for details.\n");
      return 0;
    }
    else if (strcmp (argv[i], "--help") == 0)
    {
      printf ("fdtd3d viewer\n\nUsage:\n");
      printf ("--file <path> : load single file\n");
      printf ("--dir <dir> : load all files from directory\n");
      printf ("--msec-per-frame <N> : display each file for <N> mseconds (0 by default, i.e. as fast as possible) \n");
      printf ("--1d\n");
      printf ("--2d\n");
      printf ("--3d\n");
      printf ("--size-x <N>\n");
      printf ("--size-y <N>\n");
      printf ("--size-z <N>\n");
      printf ("--complex-values\n");
      return 0;
    }
    else if (strcmp (argv[i], "--file") == 0)
    {
      ++i;
      path = std::string (argv[i]);

      viewerMode = Mode::SINGLE_FILE;
    }
    else if (strcmp (argv[i], "--dir") == 0)
    {
      ++i;
      path = std::string (argv[i]);

      viewerMode = Mode::DIRECTORY;
    }
    else if (strcmp (argv[i], "--msec-per-frame") == 0)
    {
      ++i;
      msec = atoi (argv[i]);
    }
    else if (strcmp (argv[i], "--1d") == 0)
    {
      viewerDim = Dimension::1D;
    }
    else if (strcmp (argv[i], "--2d") == 0)
    {
      viewerDim = Dimension::2D;
    }
    else if (strcmp (argv[i], "--3d") == 0)
    {
      viewerDim = Dimension::3D;
    }
    else if (strcmp (argv[i], "--size-x") == 0)
    {
      ++i;
      sizeX = atoi (argv[i]);
    }
    else if (strcmp (argv[i], "--size-y") == 0)
    {
      ++i;
      sizeY = atoi (argv[i]);
    }
    else if (strcmp (argv[i], "--size-z") == 0)
    {
      ++i;
      sizeZ = atoi (argv[i]);
    }
    else if (strcmp (argv[i], "--complex-values") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return 0;
    }
    else
    {
      printf ("unknown option %s\n", argv[i]);
      return 1;
    }
  }

  if (viewerMode == Mode::NONE)
  {
    printf ("Mode is not set! Use --file or --dir.\n");
    return 0;
  }

  if (viewerDim == Dimension::NONE)
  {
    printf ("Dimension is not set! Use --1d, --2d or --3d.\n");
    return 0;
  }

  if (sizeX == 0)
  {
    printf ("Size is not set! Use --size-x.\n");
  }

  if (sizeY == 0 && (viewerDim == Dimension::2D || viewerDim == Dimension::3D))
  {
    printf ("Size is not set! Use --size-y.\n");
  }

  if (sizeY == 0 && viewerDim == Dimension::3D)
  {
    printf ("Size is not set! Use --size-z.\n");
  }

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("fdtd3d-viewer");
 
  /*
   * Register GLUT callbacks.
   */
  glutDisplayFunc(redraw);
  glutKeyboardFunc(key);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
 
  /*
   * Init the GL state
   */
  glLineWidth(1.0);
 
  /*
   * Main loop
   */
  glutMainLoop();

  return 0;
}
