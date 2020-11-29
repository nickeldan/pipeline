#ifndef __PIPELINE_ERROR_H__
#define __PIPELINE_ERROR_H__

enum plErrorValue {
    PL_ERROR_OK = 0,
    PL_ERROR_NOT_FOUND,
    PL_ERROR_OUT_OF_MEMORY,
    PL_ERROR_INTEGER_OVERFLOW,
    PL_ERROR_NAME_TOO_LONG,
    PL_ERROR_BAD_INPUT,
    PL_ERROR_MODULE_LOAD,

    PL_ERROR_INVALID_ERRNO,
};

const char*
plErrorString(int code);

#endif
