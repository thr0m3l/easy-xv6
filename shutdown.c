#include "stat.h"
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  printf(1, "Shutting down....\n");
  shutdown();
  exit();
}