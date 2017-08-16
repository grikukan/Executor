#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "executor.h"
int main(int argc, char **argv) {
  int no_patch = 0;
  uint64_t TL = 0, ML = 0;
  char *path = NULL;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--no-patch") == 0) {
      no_patch = 1;
    } else if (strcmp(argv[i], "-TL") == 0) {
      TL = atoll(argv[i + 1]);
      i++;
    } else if (strcmp(argv[i], "-ML") == 0) {
      ML = atoll(argv[i + 1]);
      i++;
    } else {
      path = argv[i];
    }
  }

  char *args[] = { NULL };
  char *env_args[] = { NULL };
  int32_t code = execute(path, args, env_args, TL, ML, no_patch);
  switch (code) {
    case 0:
      printf("OK\n");
      break;
    case 1:
      printf("SV\n");
      break;
    case 2:
      printf("ML\n");
      break;
    case 3:
      printf("TL\n");
      break;
    case 4:
      printf("RE\n");
      break;
    case 239:
      printf("Execve fail\n");
      break;
    case 179:
      printf("You have no patch installed. Probably you want to run --no-patch\n");
      break;
    default:
      printf("WTF\n");
      break;
  }
  return 0;
}