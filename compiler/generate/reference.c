#include "reference.h"

const char *
plRefTypeName(uint32_t flags)
{
    flags &= 0xff000000;
    flags &= ~PL_REF_FLAG_EXPORT;

    switch (flags) {
    case PL_REF_FLAG_OBJECT: return "OBJECT";
    case PL_REF_FLAG_TYPE: return "TYPE";
    case PL_REF_FLAG_SOURCE: return "SOURCE";
    case PL_REF_FLAG_PIPE: return "PIPE";
    case PL_REF_FLAG_SINK: return "SINK";
    default: return "MODULE";
    }
}