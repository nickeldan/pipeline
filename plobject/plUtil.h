#ifndef PIPELINE_UTIL_H
#define PIPELINE_UTIL_H

enum plRetValue {
    PL_RET_OK = 0,
    PL_RET_USAGE,
    PL_RET_OUT_OF_MEMORY,
    PL_RET_BAD_DATA,
    PL_RET_NO_ACCESS,
    PL_RET_IO,
    PL_RET_OVERFLOW,
};

int
plTranslateVasqRet(int value);

#endif  // PIPELINE_UTIL_H
