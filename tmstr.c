/* tmstr.c - simple timestamp string. */
/*
# This code and its documentation is Copyright 2025-2025 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/tmstr
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "tmstr.h"

/* Global format string, settable during initialization. */
static char *tmstr_format = NULL;
static size_t tmstr_act_len = 0;
/* Constant string returned in case of error. */
const char tmstr_error_msg[] = "*** ERROR in tmstr_r()";

/* Default format: Thu Apr 17 09:39:20 EDT 2025 */
const char tmstr_default_format[] = "L%a %b %d %H:%M:%S %Z %Y";
/* Sortable format: 2024-05-11 14:33:34 */
const char tmstr_sortable_format[] = "L%F %T";

/* To be c99 compliant, write own strdup. */
char *tmstr_strdup(const char *str) {
  size_t len = strlen(str) + 1;
  char *new_str = malloc(len);
  if (new_str == NULL) { return NULL; }
  memcpy(new_str, str, len);
  return new_str;
}  /* tmstr_strdup */


size_t tmstr_set_fmt(const char *fmt_str) {
  if (tmstr_format != NULL) {
    free(tmstr_format);
  }
  tmstr_format = (char *)tmstr_strdup(fmt_str);

  /* Check to see how big the resulting formatted time is. */
  size_t out_len = strlen(tmstr_format) * 3 + 8;  /* Should be big enough. */
  char *out_str = malloc(out_len);
  if (out_str == NULL) { return 0; }  /* Error. */

  (void)tmstr_r(out_str, out_len);

  tmstr_act_len = strlen(out_str);
  free(out_str);
  if (tmstr_act_len == 0) {
    return 0;  /* Error. */
  }

  /* Allow for null and a bit extra in case the output can vary. */
  tmstr_act_len += 9;
  return tmstr_act_len;
}  /* tmstr_set_fmt */


const char *tmstr_r(char *buf, size_t buf_len) {
  struct timespec cur_timespec;
  struct tm cur_timestruct;

  if (buf == NULL || buf_len == 0) { return (const char *)tmstr_error_msg; }

  if (tmstr_format == NULL) {
    /* First-time in; user hasn't set format. Set to default.
     * Note that this causes a recursive call to tmstr_r, but only
     * one level of recursion. Also not thread-safe. */
    (void)tmstr_set_fmt(tmstr_default_format);
  }

  int status = clock_gettime(CLOCK_REALTIME, &cur_timespec);
  if (status != 0) {
    return (const char *)tmstr_error_msg;
  }

  if (tmstr_format[0] == 'L') {
    (void)localtime_r(&cur_timespec.tv_sec, &cur_timestruct);
  }
  else if (tmstr_format[0] == 'U') {
    (void)gmtime_r(&cur_timespec.tv_sec, &cur_timestruct);
  }
  else return (const char *)tmstr_error_msg;

  buf[0] = '\0';  /* Just in case strftime doesn't touch the buffer. */
  size_t out_len = strftime(buf, buf_len, &tmstr_format[1], &cur_timestruct);
  if (out_len == 0) {
    return (const char *)tmstr_error_msg;
  }

  /* Add decimal part (if requested). No need to worry about buffer length since
   * we're only overwriting existing '.' characters. */

  char ns_str[16];
  snprintf(ns_str, sizeof(ns_str), "%09lu", cur_timespec.tv_nsec);  /* Leading zeros. */

  char *dot = buf;
  while (*dot != '\0') {  /* Search buffer for decimal point. */
    if (*dot == '.') {  /* Found decimal point. */
      int i = 0;  /* Index into ns_str[]. */
      dot++;  /* Step past the decimal point. */
      while (*dot == '.') {  /* Overwrite rest of '.' with ns_str. */
        if (ns_str[i] != '\0') {
          *dot = ns_str[i];
          i++;
        } else {  /* Past end of ns_str; pad with '0'. */
          *dot = '0';
        }
        dot++;
      }  /* while '.' */
      break;  /* Ignore rest of format string. */
    }
    else {  /* dot not pointing at '.' */
      dot++;
    }
  }  /* while not '\0'. */

  return buf;
}  /* tmstr_r */


const char *tmstr() {
  static char *buf = NULL;
  static size_t buf_len = 0;

  if (tmstr_format == NULL) {
    /* First time initialization. */
    tmstr_set_fmt(tmstr_default_format);
  }
  if (tmstr_act_len == 0) {
    /* Something went wrong with format string. */
    return (const char *)tmstr_error_msg;
  }

  if (buf == NULL || tmstr_act_len != buf_len) {
    if (buf != NULL) {
      free(buf);
    }
    buf = malloc(tmstr_act_len);
    if (buf == NULL) { return tmstr_error_msg; }
    buf_len = tmstr_act_len;
  }

  return tmstr_r(buf, buf_len);
}  /* tmstr */
