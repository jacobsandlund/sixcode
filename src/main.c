#include "engine/manager.h"

int main(int argc, const char *argv[]) {
	(void)argc;
	(void)argv;

    engine_manager_init(&gEngineConfig);
    engine_manager_run();
}
