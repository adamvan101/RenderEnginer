#include "DisplayManager.h"

int main(int argc, char** argv) {

	DisplayManager dm = DisplayManager(512, 512, "Scene Demo");

	dm.Run();

	return 0;
}