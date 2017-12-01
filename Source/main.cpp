#include "Viewer.h"

int main (int argc, char **argv)
{
  Viewer::init (argc, argv);

  Status status = settings.parseCmd (argc, argv);
  if (status != STATUS_OK)
  {
    return status;
  }

  Viewer::loop ();

  return 0;
}
