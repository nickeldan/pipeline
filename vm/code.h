#ifndef PIPELINE_CODE_H
#define PIPELINE_CODE_H

enum plOpCode {
    PL_OP_POP = 0,
    PL_OP_COPY,
    PL_OP_PUSH_LOCAL,
    PL_OP_PUSH_LOCAL_TRANSFER,
    PL_OP_PUSH_CONST,
    PL_OP_PUSH_GLOBAL,
    PL_OP_PUSH_SMALL_INT,
    PL_OP_PUSH_TRUE,
    PL_OP_PUSH_FALSE,
    PL_OP_PUSH_BLANK,

    PL_OP_DROP,
    PL_OP_END,

    PL_DROP_IF_FALSE,
    PL_END_IF_FALSE,

    PL_OP_TEST,
    PL_OP_TEST_TYPE,

    PL_OP_AND,
    PL_OP_OR,
    PL_OP_NOT,

    PL_OP_ARRAY_LOOKUP,
    PL_OP_ARRAY_QUICK_LOOKUP,

    PL_OP_MOVE_VAL_TO_FS,
    PL_OP_PUSH_SOURCE_TO_FS,
    PL_OP_PUSH_PIPE_TO_FS,
    PL_OP_PUSH_SINK_TO_FS,
    PL_OP_PUSH_GROUP_TO_FS,
    PL_OP_PUSH_LOCAL_REF_TO_FS,
    PL_OP_START_FLOW,
    PL_OP_PUSH_FLOW_END_TO_FS,

    PL_OP_ADD,
    PL_OP_IN_PLACE_ADD,
    PL_OP_SUBTRACT,
    PL_OP_IN_PLACE_SUBTRACT,
    PL_OP_MULTIPLY,
    PL_OP_IN_PLACE_MULTIPLY,
    PL_OP_DIVIDE,
    PL_OP_IN_PLACE_DIVIDE,
    PL_OP_MODULO,
    PL_OP_IN_PLACE_MODULO,
    PL_OP_BIT_AND,
    PL_OP_IN_PLACE_BIT_AND,
    PL_OP_BIT_OR,
    PL_OP_IN_PLACE_BIT_OR,
    PL_OP_LSHIFT;
    PL_OP_IN_PLACE_LSHIFT;
    PL_OP_RSHIFT;
    PL_OP_IN_PLACE_RSHIFT;

    PL_OP_ARRAY_ADD,
    PL_OP_ARRAY_IN_PLACE_ADD,
    PL_OP_ARRAY_MULTIPLY,
    PL_OP_ARRAY_IN_PLACE_MULTIPLY,

    PL_OP_EXIT,

    PL_OP_UNUSED,
};

_Static_assert(PL_OP_UNUSED <= 256, "PL_OP_UNUSED must be no bigger than 256.");

#endif  // PIPELINE_CODE_H
