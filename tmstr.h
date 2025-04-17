/* tmstr.h - timestamp string. */
/*
# This code and its documentation is Copyright 2025-2025 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/tmstr
*/

#ifndef TMSTR_H
#define TMSTR_H

#ifdef __cplusplus
extern "C" {
#endif

const char *tmstr(void);
const char *tmstr_r(char *buf, size_t buflen);

#ifdef __cplusplus
}
#endif

#endif  /* TMSTR_H */
