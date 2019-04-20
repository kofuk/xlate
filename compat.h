#ifndef COMPAT_H
#define COMPAT_H

#ifdef _WIN32
#define stat _stat
#define strdup _strdup
#endif

#endif
