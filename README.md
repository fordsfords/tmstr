# tmstr
Simple C functions returning timestamp strings for prints.


## Table of contents

<!-- mdtoc-start -->
&bull; [tmstr](#tmstr)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [API](#api)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Functions](#functions)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`const char *tmstr(void)`](#const-char-tmstrvoid)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [`const char *tmstr_r(char *buf, size_t buflen)`](#const-char-tmstr_rchar-buf-size_t-buflen)  
<!-- TOC created by '../mdtoc/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->


## Introduction

For programs that log events, a timestamp is important.
The tmstr module provides several choices for functions
that return timestamp strings, suitable for inclusion in print statements.

For example:
```c
printf("%s myprogram starting up.\n", tmstr());
```

## API

### Functions

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

Example:
```c
char tm_buf[26];
printf("%s myprogram starting up.\n", tmstr_r(tm_buf, sizeof(tm_buf)));
```
