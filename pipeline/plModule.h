#include "dll.h"

typedef struct plModule {

} plModule;

void moduleInit(plModule *module);

int ciplLoad(int fd, plModule *module);
