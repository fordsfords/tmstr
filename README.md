# tmstr
Simple C functions returning timestamp strings for prints.


## Table of contents

<!-- mdtoc-start -->
&bull; [tmstr](#tmstr)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Formatting](#formatting)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Multi-Threading](#multi-threading)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [API](#api)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Functions](#functions)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`size_t tmstr_set_fmt(const char *fmt_str)`](#size_t-tmstr_set_fmtconst-char-fmt_str)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`const char *tmstr(void)`](#const-char-tmstrvoid)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`const char *tmstr_r(char *buf, size_t buflen)`](#const-char-tmstr_rchar-buf-size_t-buflen)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Extern Globals](#extern-globals)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`const char tmstr_error_msg[]`](#const-char-tmstr_error_msg)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`const char tmstr_default_format[]`](#const-char-tmstr_default_format)  
<!-- TOC created by '../mdtoc/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->


## Introduction

For programs that log events, a timestamp is important.
The tmstr module provides several choices for functions
that return timestamp strings, suitable for inclusion in print statements.

Simple example (assumes single-threaded use):
```c
printf("%s myprogram starting up.\n", tmstr());
```

### Formatting

tnstr also allows flexible timestamp formatting using an
enhanced formating string derived from
[strftime()](https://man7.org/linux/man-pages/man3/strftime.3.html).
The enhancement consists of:
* The first character of the format string **MUST** be either 'L' for local time
or 'U' for UTC time.
* The inclusion of multiple periods in a row inserts the decimal part of seconds.
The first period is printed as-is, and subsequent periods are replaced by digits.
The number of periods after the first one indicates the desired precision.

If a format string is desired, it should be set during initialization.
For example:
```c
/* Use 7 periods to get microsecond precision. */
tmstr_set_fmt("L%F %T.......");
printf("timestamp: %s\n", tmstr());
```
will print a local system time in the form:
```
2024-05-11 14:33:34.492125
```

### Multi-Threading

The `tmstr_r()` function is thread-safe.
Multi-threaded programs should first call `tmstr_set_fmt()`
(which is not thread-safe) to set an appropriate format.

For the default format, use:
```c
size_t tm_len = tmstr_set_fmt(tmstr_default_format);
char buf[32];
int buf_len = sizeof(buf);
printf("timestamp: %s\n", tmstr_r(buf, buf_len));
```
Note that while 32 chars is OK for the default format,
using a custom format may require a larger buffer for
`tmstr_r()`.
You should use the return value from `tmstr_set_fmt()`
as the `buf` size (perhaps malloc the buffer).

For proper multi-threaded use, the passed-in `buf` must
not be accessed concurrently by other threads during the
`tmrstr_r()` and `printf()`.

## API

### Functions

#### `size_t tmstr_set_fmt(const char *fmt_str)`

Set the format string. Optional for non-multithreaded use.
Should be called by multi-threaded programs prior to first
call to `tmstr_r()`.
See extern global `tmstr_default_format` (below).

NOTE: not thread-safe.

#### `const char *tmstr(void)`

Returns a pointer to a time string in the same format that the unix `date`
command prints.
Note that in the unlikely event of an internal error, the string returned
will be an error message.

NOTE: not thread-safe.
See `tmstr_r()`.

Example:
```c
printf("%s myprogram starting up.\n", tmstr());
```

#### `const char *tmstr_r(char *buf, size_t buflen)`

Thread-safe (reentrant) version.
Requires the caller provide a character buffer of
at least 26 chars.
On success, tmstr_r() returns a pointer to that
same buffer.
On failure, returns a pointer to an error
message.

The passed-in `buf` should be thread-local.
For example, could be a dynamic local to the
current function.

Example:
```c
char tm_buf[32];
printf("%s myprogram starting up.\n", tmstr_r(tm_buf, sizeof(tm_buf)));
```

### Extern Globals

#### `const char tmstr_error_msg[]`

Static char buffer containing error message.
If a tmstr function returns a pointer to this on error.
The caller can compare the returned pointer to tmstr_error_msg
to detect the error algorithmically.

#### `const char tmstr_default_format[]`

Static char buffer containing the default format string.
Multithreaded programs should call `tmstr_set_fmt()`
prior to calling `tmstr_r()`. You can pass
tmstr_default_format to get the default behavior.
