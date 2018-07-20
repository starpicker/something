#ifndef MLALLOC_H__
#define MLALLOC_H__

extern const char* log_file_name = "mlog.txt";

#include "mlmc.h"

#define malloc(dwMemSize)   MLAlloc((dwMemSize), __FILE__, FUNC_NAME, __LINE__)
#define free(pvMemBuf)      MLFree((pvMemBuf), __FILE__, FUNC_NAME, __LINE__)

#endif // MLALLOC_H__
