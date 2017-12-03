#include "Viewer.h"

#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <string>

bool Viewer::isAnimationMode = false;
int Viewer::curFileIndex = 0;

FPValue Viewer::initialMax = 0.0;
FPValue Viewer::initialMin = 0.0;

int Viewer::directionMultiplier = 1;

void
Viewer::getMaxVals (const char *filename, FPValue &min, FPValue &max)
{
  FILE *f = fopen (filename, "r");

  max = 0;
  min = 0;

  for (int index = 0; index < settings.totalCount; ++index)
  {
    // TODO: add complex values case here
    // TODO: add other dimensions
    if (settings.viewerDim == Dimension::D1)
    {
      int posX;
      FPValue val;
      int count = fscanf (f, "%d " FPMOD "\n", &posX, &val);
      ASSERT (count == 2);

      ASSERT (posX == index);

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
    else
    {
      UNREACHABLE;
    }
  }

  if (curFileIndex == 0)
  {
    initialMin = min;
    initialMax = max;
  }

  fclose (f);
}

void
Viewer::drawFile (const char *filename)
{
  FPValue max, min;

  if (settings.doUseInitialScale)
  {
    min = initialMin;
    max = initialMax;
  }
  else
  {
    getMaxVals (filename, min, max);
  }

  FILE *f = fopen (filename, "r");

  glPushMatrix (); /* GL_MODELVIEW is default */

  // TODO: add other dims
  if (settings.viewerDim == Dimension::D1)
  {
    FPValue scaleX = settings.sizeX * 0.05;
    FPValue scaleY = (max - min) * 0.05;
    glScalef (1.0 / (settings.sizeX + 2 * scaleX), 1.0 / (max - min + 2 * scaleY), 1.0);
    glTranslatef (scaleX, - min + scaleY, 0.0);
    glColor3f (1.0, 1.0, 1.0);
  }
  else
  {
    UNREACHABLE;
  }

  glBegin (GL_LINE_STRIP);

  for (int index = 0; index < settings.totalCount; ++index)
  {
    // TODO: add complex values case here
    // TODO: add other dimensions
    if (settings.viewerDim == Dimension::D1)
    {
      int posX;
      FPValue val;
      int count = fscanf (f, "%d " FPMOD "\n", &posX, &val);
      ASSERT (count == 2);

      ASSERT (posX == index);

      glVertex2f (posX, val);
    }
    else
    {
      UNREACHABLE;
    }
  }

  glEnd ();

  glPopMatrix ();

  fclose (f);
}

void
Viewer::init (int argc, char **argv)
{
  glutInit (&argc, argv);

  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow ("fdtd3d-viewer");

  /*
   * Register GLUT callbacks.
   */
  glutDisplayFunc (redraw);
  glutKeyboardFunc (key);
  glutReshapeFunc (reshape);
  glutIdleFunc (idle);
}

/*
 * Main loop
 */
void
Viewer::loop ()
{
  curFileIndex = 0;

  glutTimerFunc (settings.msec, timer, 0);
  getMaxVals (settings.filePath[0].c_str (), initialMin, initialMax);

  glutMainLoop();
}

/**
 * Redrawing func
 */
void
Viewer::redraw ()
{
	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	drawFile (settings.filePath[curFileIndex].c_str ());

	glutSwapBuffers ();

  glLineWidth (1.0);
}

/**
 * Idle proc. Redisplays, if called.
 */
void
Viewer::idle ()
{
	glutPostRedisplay ();
}

/**
 * Key press processing
 */
void
Viewer::key (unsigned char c, int x, int y)
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
      isAnimationMode = !isAnimationMode;
      break;
    }
    case 'r':
    {
      // Reverse direction
      directionMultiplier = - directionMultiplier;
      break;
    }
    case 'c':
    {
      curFileIndex = 0;
    }
    default:
    {
    }
  }
}

/**
 * Window reshape
 */
void
Viewer::reshape (int w, int h)
{
  glViewport (0, 0, w, h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, 1, 0, 1, -1, 1);
  glMatrixMode (GL_MODELVIEW);
}

void
Viewer::timer (int value)
{
  if (isAnimationMode)
  {
    curFileIndex += directionMultiplier;

    if (curFileIndex == settings.filePath.size ())
    {
      curFileIndex = 0;
    }
    else if (curFileIndex == -1)
    {
      curFileIndex = settings.filePath.size () - 1;
    }
  }

  glutTimerFunc (settings.msec, timer, 0);
}
