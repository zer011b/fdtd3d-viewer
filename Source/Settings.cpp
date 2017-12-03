#include "Assert.h"
#include "Settings.h"

#include <cstring>

Settings settings;

Status
Settings::parseCmd (int argc, char **argv)
{
  Status status = STATUS_OK;

  for (int i = 1; i < argc; ++i)
  {
    if (strcmp (argv[i], "--help") == 0)
    {
      printf ("fdtd3d viewer. Version " VERSION ".\n\nUsage:\n");
      printf ("--files <path>,<path>,<path>...: load files from comma-separated list\n");
      printf ("--file-list <path>: load files from file <path>");
      printf ("--msec-per-frame <N>: display each file for <N> mseconds (1000 by default) \n");
      printf ("--1d: load one-dimensional files\n");
      printf ("--2d\n");
      printf ("--3d\n");
      printf ("--size-x <N>\n");
      printf ("--size-y <N>\n");
      printf ("--size-z <N>\n");
      printf ("--complex-values\n");
      printf ("--initial-scale\n");

      printf ("\nSpecial keys:\n");
      printf ("  Space - start/stop animation\n");
      printf ("  'r' - reverse animation direction\n");
      printf ("  Esc - exit\n");
      return STATUS_OK_EXIT;
    }
    else if (strcmp (argv[i], "--files") == 0)
    {
      ++i;

      std::string files (argv[i]);
      ASSERT (files.length () > 0);

      std::string singleFile ("");

      for (int i = 0; i < files.length (); ++i)
      {
        if (files[i] == ',')
        {
          filePath.push_back (singleFile);
          singleFile = std::string ("");
        }
        else
        {
          singleFile += files[i];
        }
      }

      filePath.push_back (singleFile);
    }
    else if (strcmp (argv[i], "--file-list") == 0)
    {
      ++i;

      char *line = NULL;
      size_t len = 0;
      ssize_t read;

      FILE *fp = fopen (argv[i], "r");
      if (fp == NULL)
      {
        return STATUS_FAIL;
      }

      while ((read = getline (&line, &len, fp)) != -1)
      {
        line[read - 1] = '\0';
        filePath.push_back (std::string (line));
      }

      fclose (fp);

      if (line)
      {
        free (line);
      }
    }
    else if (strcmp (argv[i], "--msec-per-frame") == 0)
    {
      ++i;
      msec = STOI (argv[i]);
    }
    else if (strcmp (argv[i], "--1d") == 0)
    {
      viewerDim = Dimension::D1;
    }
    else if (strcmp (argv[i], "--2d") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return STATUS_FAIL;
      viewerDim = Dimension::D2;
    }
    else if (strcmp (argv[i], "--3d") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return STATUS_FAIL;
      viewerDim = Dimension::D3;
    }
    else if (strcmp (argv[i], "--size-x") == 0)
    {
      ++i;
      sizeX = STOI (argv[i]);
    }
    else if (strcmp (argv[i], "--size-y") == 0)
    {
      ++i;
      sizeY = STOI (argv[i]);
    }
    else if (strcmp (argv[i], "--size-z") == 0)
    {
      ++i;
      sizeZ = STOI (argv[i]);
    }
    else if (strcmp (argv[i], "--complex-values") == 0)
    {
      // TODO
      printf ("Not implemented.\n");
      return STATUS_FAIL;
    }
    else if (strcmp (argv[i], "--initial-scale") == 0)
    {
      doUseInitialScale = true;
    }
    else
    {
      printf ("unknown option %s\n", argv[i]);
      return STATUS_FAIL;
    }
  }

  if (filePath.size () == 0)
  {
    printf ("Files not specified! Use --files or --file-list.\n");
    return STATUS_FAIL;
  }

  if (viewerDim == Dimension::NONE)
  {
    printf ("Dimension is not set! Use --1d, --2d or --3d.\n");
    return STATUS_FAIL;
  }

  if (sizeX == 0)
  {
    printf ("Size is not set! Use --size-x.\n");
    return STATUS_FAIL;
  }

  if (sizeY == 0 && (viewerDim == Dimension::D2 || viewerDim == Dimension::D3))
  {
    printf ("Size is not set! Use --size-y.\n");
    return STATUS_FAIL;
  }

  if (sizeY == 0 && viewerDim == Dimension::D3)
  {
    printf ("Size is not set! Use --size-z.\n");
    return STATUS_FAIL;
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

  return status;
}
