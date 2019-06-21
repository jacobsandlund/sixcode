#include "Engine/Manager.h"

int main(int argc, const char *argv[]) {
	(void)argc;
	(void)argv;

    EngineManagerInit(&gEngineManagerCallbacks, &gEngineManagerConfig);
    EngineManagerRun();
}
