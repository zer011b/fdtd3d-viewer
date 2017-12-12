#include "Viewer.h"

#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <string>

bool Viewer::isAnimationMode = false;
int Viewer::curFileIndex = 0;

FPValue Viewer::initialMax = 0.0;
FPValue Viewer::initialMin = 0.0;

FPValue Viewer::currentMax = 0.0;
FPValue Viewer::currentMin = 0.0;

int Viewer::directionMultiplier = 1;

void
Viewer::getMaxVals (const char *filename, FPValue &out_min, FPValue &out_max)
{
  FILE *f = fopen (filename, "r");

  FPValue max = 0;
  FPValue min = 0;

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
        max = val;
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
    else if (settings.viewerDim == Dimension::D2)
    {
      int posX;
      int posY;
      FPValue val;
      int count = fscanf (f, "%d %d " FPMOD "\n", &posX, &posY, &val);
      ASSERT (count == 3);

      if (index == 0)
      {
        min = val;
        max = val;
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

  if (min < out_min)
  {
    out_min = min;
  }
  if (max > out_max)
  {
    out_max = max;
  }

  fclose (f);
}

void
Viewer::drawFile (const char *filename, FPValue red, FPValue green, FPValue blue)
{
  FILE *f = fopen (filename, "r");

  glPushMatrix (); /* GL_MODELVIEW is default */

  if (settings.viewerDim == Dimension::D1)
  {
    scale ();
    glColor3f (red, green, blue);

    glBegin (GL_LINE_STRIP);

    for (int index = 0; index < settings.totalCount; ++index)
    {
      // TODO: add complex values case here
      // TODO: add other dimensions

      int posX;
      FPValue val;
      int count = fscanf (f, "%d " FPMOD "\n", &posX, &val);
      ASSERT (count == 2);

      ASSERT (posX == index);

      glVertex2f (posX, val);
    }

    glEnd ();
  }
  else if (settings.viewerDim == Dimension::D2)
  {
    scale ();

    for (int index = 0; index < settings.totalCount; ++index)
    {
      // TODO: add complex values case here
      // TODO: add other dimensions
      int posX;
      int posY;
      FPValue val;
      int count = fscanf (f, "%d %d " FPMOD "\n", &posX, &posY, &val);
      ASSERT (count == 3);

      val = val / (currentMax-currentMin);
      val = val > 0 ? val : -val;
      glColor3f (val, val, val);
      glRectd(posX-0.5, posY-0.5, posX+0.5, posY+0.5);
      //glVertex2f (posX, posY);
    }
  }
  else
  {
    UNREACHABLE;
  }

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

  getMaxVals (settings.filePath[0].c_str (), currentMin, currentMax);
  if (!settings.filePath1.empty ())
  {
    getMaxVals (settings.filePath1[0].c_str (), currentMin, currentMax);
  }

  if (settings.doUseManualScale)
  {
    currentMin = settings.min;
    currentMax = settings.max;
  }

  initialMin = currentMin;
  initialMax = currentMax;

  glutMainLoop();
}

void
Viewer::scale ()
{
  // TODO: add other dims
  if (settings.viewerDim == Dimension::D1)
  {
    FPValue scaleX = settings.sizeX * 0.05;
    FPValue scaleY = (currentMax - currentMin) * 0.05;
    glScalef (1.0 / (settings.sizeX + 2 * scaleX), 1.0 / (currentMax - currentMin + 2 * scaleY), 1.0);
    glTranslatef (scaleX, - currentMin + scaleY, 0.0);
  }
  else if (settings.viewerDim == Dimension::D2)
  {
    FPValue scaleX = settings.sizeX * 0.2;
    FPValue scaleY = settings.sizeY * 0.2;
    glScalef (1.0 / (settings.sizeX + 2 * scaleX), 1.0 / (settings.sizeY + 2 * scaleY), 1.0);
    glTranslatef (scaleX, scaleY, 0.0);
  }
  else
  {
    UNREACHABLE;
  }
}

void
Viewer::drawAxes ()
{
  glPushMatrix (); /* GL_MODELVIEW is default */

  if (settings.viewerDim == Dimension::D1)
  {
    glLineWidth (2.0);
    scale ();
    glColor3f (0.0, 0.0, 0.0);

    glBegin (GL_LINE_STRIP);
    glVertex2f (0.0, 0.0);
    glVertex2f (settings.totalCount, 0.0);
    glEnd ();

    glBegin (GL_LINE_STRIP);
    glVertex2f (0.0, currentMin);
    glVertex2f (0.0, currentMax);
    glEnd ();

    glLineWidth (1.0);
  }
  else if (settings.viewerDim == Dimension::D2)
  {
    glLineWidth (2.0);
    scale ();
    // glColor3f (0.0, 0.0, 0.0);
    //
    // glBegin (GL_LINE_STRIP);
    // glVertex2f (-35.0, 0.0);
    // glVertex2f (-5.0, 0.0);
    // glVertex2f (-5.0, settings.sizeY);
    // glVertex2f (-35.0, settings.sizeY);
    // glVertex2f (-35.0, 0.0);
    // glEnd ();

    for (int i = 0; i < 9; ++i)
    {
      // glBegin (GL_LINE_STRIP);
      // glVertex2f (-35.0, i*settings.sizeY / 10);
      // glVertex2f (-5.0, i*settings.sizeY / 10);
      // glEnd ();
      FPValue val = i*1.0/10.0;
      glColor3f (val, val, val);
      glRectd (-35.0, i*settings.sizeY/10.0, -5.0, (i+1)*settings.sizeY/10.0);
    }

    std::string str1 = std::to_string (currentMin);
    //printf ("!! %s\n", str.c_str());
    for (int cc = 0; cc < 10 && cc < str1.length (); ++cc)
    {
      glPushMatrix ();
      glColor3f (1.0, 0.0, 0.0);
      glTranslatef (-80+cc*20,-30,0);
      glScalef (1.0 / 5, 1.0 / 5, 1.0);
      glutStrokeCharacter(GLUT_STROKE_ROMAN, str1[cc]);
      glPopMatrix ();
    }

    std::string str2 = std::to_string (currentMax);
    //printf ("!! %s\n", str.c_str());
    for (int cc = 0; cc < 10 && cc < str2.length (); ++cc)
    {
      glPushMatrix ();
      glColor3f (1.0, 0.0, 0.0);
      glTranslatef (-80+cc*20,500,0);
      glScalef (1.0 / 5, 1.0 / 5, 1.0);
      glutStrokeCharacter(GLUT_STROKE_ROMAN, str2[cc]);
      glPopMatrix ();
    }

    glLineWidth (1.0);
  }
  else
  {
    UNREACHABLE;
  }

  glPopMatrix ();
}

/**
 * Redrawing func
 */
void
Viewer::redraw ()
{
	glClearColor (1.0, 1.0, 1.0, 0);
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

  if (settings.doUseInitialScale || settings.doUseManualScale)
  {
    currentMin = initialMin;
    currentMax = initialMax;
  }
  else
  {
    getMaxVals (settings.filePath[curFileIndex].c_str (), currentMin, currentMax);
    if (!settings.filePath1.empty ())
    {
      getMaxVals (settings.filePath1[curFileIndex].c_str (), currentMin, currentMax);
    }
  }

  drawAxes ();
  drawFile (settings.filePath[curFileIndex].c_str (), 1.0, 0.0, 0.0);

  if (!settings.filePath1.empty ())
  {
    drawFile (settings.filePath1[curFileIndex].c_str (), 0.0, 1.0, 0.0);
  }

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
