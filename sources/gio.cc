//
// FILE: gio.cc -- Provide global standard stream instances
//
// $Id$
//

#include "gstream.h"

gFileInput _gin(stdin);
gInput &gin = _gin;

gFileOutput _gout(stdout);
gOutput &gout = _gout;

gFileOutput _gerr(stderr);
gOutput &gerr = _gerr;


