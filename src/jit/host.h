// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*****************************************************************************/

#ifdef DEBUG
#ifndef printf
#define printf logf
#endif

#ifndef fprintf
#define fprintf flogf
#endif

class Compiler;
class LogEnv
{
public:
    LogEnv(ICorJitInfo* aCompHnd);
    void setCompiler(Compiler* val)
    {
        const_cast<Compiler*&>(compiler) = val;
    }

    ICorJitInfo* const compHnd;
    Compiler* const    compiler;
};

BOOL vlogf(unsigned level, const char* fmt, va_list args);
int vflogf(FILE* file, const char* fmt, va_list args);

int logf(const char* fmt, ...);
int flogf(FILE* file, const char* fmt, ...);
void gcDump_logf(const char* fmt, ...);

void logf(unsigned level, const char* fmt, ...);

extern "C" void __cdecl assertAbort(const char* why, const char* file, unsigned line);

#undef assert
#define assert(p) (void)((p) || (assertAbort(#p, __FILE__, __LINE__), 0))

#else // DEBUG

#undef assert
#define assert(p) (void)0
#endif // DEBUG

/*****************************************************************************/
#ifndef _HOST_H_
#define _HOST_H_
/*****************************************************************************/

const size_t OS_page_size = (4 * 1024);

extern FILE* jitstdout;

inline FILE* procstdout()
{
    return stdout;
}
#undef stdout
#define stdout use_jitstdout

/*****************************************************************************/
#endif
/*****************************************************************************/
