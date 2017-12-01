#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

enum Status
{
  STATUS_OK = 0,
  STATUS_OK_EXIT,
  STATUS_FAIL
};

enum class Mode
{
  NONE,
  SINGLE_FILE,
  DIRECTORY
};

enum class Dimension
{
  NONE,
  D1,
  D2,
  D3
};

class Settings
{
public:

  Mode viewerMode;
  Dimension viewerDim;

  std::string filePath;

  int msec;

  int sizeX;
  int sizeY;
  int sizeZ;

  int totalCount;

  bool isComplexMode;

  bool doUseInitialScale;

public:

  Settings ()
    : viewerMode (Mode::NONE)
  , viewerDim (Dimension::NONE)
  , msec (1000)
  , sizeX (0)
  , sizeY (0)
  , sizeZ (0)
  , totalCount (0)
  , isComplexMode (false)
  , doUseInitialScale (false)
  {
  }

  ~Settings ()
  {
  }

  Status parseCmd (int, char **);
};

extern Settings settings;

#endif /* !SETTINGS_H */
