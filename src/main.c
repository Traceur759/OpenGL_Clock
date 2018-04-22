// Include APIs
#include "clock.h"

int
main(int argc, char *argv[]) {

    GLFWwindow *window;
    if (clock_init(&window)) {
        return -1;
    }

	return clock_run(window);
}

