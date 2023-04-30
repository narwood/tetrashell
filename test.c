#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

int main () {

FILE* fp = fopen("recovered/out_1.bin", "rb");
TetrisGameState state;
fread(&state, sizeof(state), 1, fp);
fclose(fp);


}
