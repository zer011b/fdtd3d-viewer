#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

enum Status
{
  STATUS_OK = 0,
  STATUS_OK_EXIT,
  STATUS_FAIL
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

  Dimension viewerDim;

  std::vector<std::string> filePath;
  std::vector<std::string> filePath1;

  int msec;

  int sizeX;
  int sizeY;
  int sizeZ;

  int totalCount;

  bool isComplexMode;

  bool doUseInitialScale;

  bool doUseManualScale;
  FPValue min;
  FPValue max;

public:

  Settings ()
    : viewerDim (Dimension::NONE)
  , msec (1000)
  , sizeX (0)
  , sizeY (0)
  , sizeZ (0)
  , totalCount (0)
  , isComplexMode (false)
  , doUseInitialScale (false)
  , doUseManualScale (false)
  , min (0.0)
  , max (0.0)
  {
  }

  ~Settings ()
  {
  }

  Status parseCmd (int, char **);
};

extern Settings settings;

#endif /* !SETTINGS_H */
