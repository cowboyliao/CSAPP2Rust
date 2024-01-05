#include <stdio.h>

void printError(const char* message) {
  fprintf(stderr, "Error: %s\n", message);
}
