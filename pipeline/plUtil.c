#include "vasq/definitions.h"

#include "plUtil.h"

int
plTranslateVasqRet(int value)
{
    switch (value) {
    case VASQ_RET_OK: return PL_RET_OK;
    case VASQ_RET_IMPROPER_USE: return PL_RET_USAGE;
    case VASQ_RET_OUT_OF_MEMORY: return PL_RET_OUT_OF_MEMORY;
    default: return -1;
    }
}
