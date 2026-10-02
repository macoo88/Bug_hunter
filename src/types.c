#include "types.h"

static State global_state = {0};

State* GetState(void) {
    return &global_state;
}
