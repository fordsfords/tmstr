/* tmstr.c - simple timestamp string. */
/*
# This code and its documentation is Copyright 2024-2024 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/tmstr
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "tmstr.h"

const char *tmstr_r(char* buf, size_t buflen) {
  static const char* error_msg = "*** ERROR in tmstr_r()";
  time_t current_time;
  struct tm time_info;

  if (buf == NULL || buflen < 26) {
    return (const char *)error_msg;
  }

  time(&current_time);

  if (localtime_r(&current_time, &time_info) == NULL) {
    return (const char *)error_msg;
  }

  if (asctime_r(&time_info, buf) == NULL) {
    return (const char *)error_msg;
  }

  /* Remove the trailing newline that asctime_r adds */
  size_t str_len = strlen(buf);
  if (str_len > 0 && buf[str_len-1] == '\n') {
    buf[str_len-1] = '\0';
  }

  return (const char *)buf;
}  /* tmstr_r */


const char *tmstr() {
  static char buffer[32];

  return tmstr_r(buffer, sizeof(buffer));
}  /* tmstr */
