#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <string>
#include <cstring>

#define FPValue double

#define VERSION "0.01"

enum class Mode : uint32_t
{
  NONE,
  SINGLE_FILE,
  DIRECTORY
};

enum class Dimension : uint32_t
{
  NONE,
  D1,
  D2,
  D3
};

Mode viewerMode = Mode::NONE;
Dimension viewerDim = Dimension::NONE;
std::string path;
int msec = 0;
bool isAnimationMode = false;
int sizeX = 0;
int sizeY = 0;
int sizeZ = 0;
int totalCount = 0;
bool isComplexMode = false;

void getMaxVals (const char *filename, FPValue &min, FPValue &max)
{
  FILE *f = fopen (filename, "r");

  max = 0;
  min = 0;

  for (int index = 0; index < totalCount; ++index)
  {
    // TODO: add complex values case here
    // TODO: add other dimensions
    if (viewerDim == Dimension::D1)
    {
      FPValue val;
      fscanf (f, "%lf\n", &val);

      if (index == 0)
      {
        min = val;
      }

      if (val > max)
      {
        max = val;
      }
      if (val < min)
      {
        min = val;
      }
    }
  }

  fclose (f);
}

void drawFile (const char *filename)
{
  FPValue max, min;
  getMaxVals (filename, min, max);

  FILE *f = fopen (filename, "r");

  glPushMatrix(); /* GL_MODELVIEW is default */

  // TODO: add other dims
  if (viewerDim == Dimension::D1)
  {
    glScalef(1.0 / sizeX, 1.0 / (max - min), 1.0);
    glTranslatef(0, - min, 0.0);
    glColor3f(1.0, 1.0, 1.0);
  }

  glBegin(GL_LINE_STRIP);

  for (int index = 0; index < totalCount; ++index)
  {
    // TODO: add complex values case here
    // TODO: add other dimensions
    if (viewerDim == Dimension::D1)
    {
      FPValue val;
      fscanf (f, "%lf\n", &val);

      glVertex2f (index, val);
    }
  }

  glEnd();

  glPopMatrix();

  fclose (f);
}

/* Redrawing func */
void redraw(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawFile (path.c_str ());

	glutSwapBuffers();
}

/* Idle proc. Redisplays, if called. */
void idle(void)
{
	glutPostRedisplay();
}

/* Key press processing */
void key(unsigned char c, int x, int y)
{
  switch (c)
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
      if (viewerMode == Mode::DIRECTORY)
      {
        isAnimationMode = !isAnimationMode;
      }
    }
    default:
    {
    }
  }
}

/* Window reashape */
void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
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
      // TODO
      printf ("Not implemented.\n");
      return 0;
      ++i;
      path = std::string (argv[i]);

      viewerMode = Mode::DIRECTORY;
    }
    else if (strcmp (argv[i], "--msec-per-frame") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return 0;
      ++i;
      msec = atoi (argv[i]);
    }
    else if (strcmp (argv[i], "--1d") == 0)
    {
      viewerDim = Dimension::D1;
    }
    else if (strcmp (argv[i], "--2d") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return 0;
      viewerDim = Dimension::D2;
    }
    else if (strcmp (argv[i], "--3d") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return 0;
      viewerDim = Dimension::D3;
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
    return 0;
  }

  if (sizeY == 0 && (viewerDim == Dimension::D2 || viewerDim == Dimension::D3))
  {
    printf ("Size is not set! Use --size-y.\n");
    return 0;
  }

  if (sizeY == 0 && viewerDim == Dimension::D3)
  {
    printf ("Size is not set! Use --size-z.\n");
    return 0;
  }

  if (viewerDim == Dimension::D1)
  {
    sizeY = 1;
    sizeZ = 1;
  }
  else if (viewerDim == Dimension::D2)
  {
    sizeZ = 1;
  }

  totalCount = sizeX * sizeY * sizeZ;

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
