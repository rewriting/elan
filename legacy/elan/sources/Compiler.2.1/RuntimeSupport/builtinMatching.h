#ifndef _builtinMatching_h
#define _builtinMatching_h
extern struct term *term_syntacticMatching(struct term *pattern,
					   struct term *subject,
					   struct term *listVar,
					   struct term *fail);
#endif
