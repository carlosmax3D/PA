#include "KeyboardInput.h"

struct Input input;
bool KEYS[256];

void Init() {
	for (int i = 0; i < 256; i++) {
		KEYS[i] = false;
	}
}//Initizalizes keys
