#include "engine/manager.h"

int main(int argc, const char *argv[]) {
	(void)argc;
	(void)argv;

    engine_manager_init();
    engine_manager_run();
    engine_manager_destroy();
}
