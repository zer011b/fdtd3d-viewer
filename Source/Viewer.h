#ifndef VIEWER_H
#define VIEWER_H

#include "Assert.h"
#include "Settings.h"

#include <vector>

class Viewer
{
  static bool isAnimationMode;
  static int curFileIndex;

  static FPValue initialMax;
  static FPValue initialMin;

  static int directionMultiplier;

private:

  static void getMaxVals (const char *, FPValue &, FPValue &);

public:

  /* Callbacks */
  static void redraw ();
  static void idle ();
  static void key (unsigned char, int, int);
  static void reshape (int, int);
  static void timer (int);

  static void drawFile (const char *);
  static void init (int, char **);
  static void loop ();
};

#endif /* !VIEWER_H */
