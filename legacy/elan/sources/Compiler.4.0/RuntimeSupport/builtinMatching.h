#include "termCommon.h"
#include "builtin.h"

#ifndef _builtinMatching_h
#define _builtinMatching_h
extern Gterm *term_syntacticMatching(Gterm *pattern,
					   Gterm *subject,
					   Gterm *listVar,
					   Gterm *fail);
#endif
