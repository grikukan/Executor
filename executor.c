//
// Created by gritukan on 8/16/17.
//

#include "executor.h"

#define MEMORY_LIMIT_CODE 0x4280
#define SAFE_MODE_CODE 0x4281
#define TIME_LIMIT_CODE 0x4282

#define MEMORY_LIMIT_STATUS 0x10000
#define SECURITY_VIOLATION_STATUS 0x20000
#define TIME_LIMIT_STATUS 0x40000

#define SECOND 1000 // There are 1000 milliseconds in second

int32_t init_safe_mode() {
  if (ptrace(SAFE_MODE_CODE, 0, 0, 0) == 0) { // enable kernel-based safe mode
    return 0;
  }
  return -1;
}

void set_time_limit(uint64_t time) { // Sets time limit for process (in milliseconds)
  struct rlimit limit;
  uint64_t real_time_limit = (time + SECOND) / SECOND; // Kernel doesn't support millisecond-precision time limit, so we will round it
  limit.rlim_cur = real_time_limit;
  limit.rlim_max = real_time_limit;
  setrlimit(RLIMIT_CPU, &limit); // set time limit
  ptrace(TIME_LIMIT_CODE, 0, 0, 0); // enable kernel-based time limit detection
}

void set_memory_limit(uint64_t memory) { // Sets memory limit for process (in bytes)
  struct rlimit limit;
  limit.rlim_cur = memory;
  limit.rlim_max = memory;
  setrlimit(RLIMIT_DATA, &limit);
  ptrace(MEMORY_LIMIT_CODE, 0, 0, 0); // enable kernel-based memory limit detection
}

int32_t execute(char *path, char **args, char **env_args, uint64_t TL, uint64_t ML, int32_t force_patch) { // 0 - OK, 1 - SV, 2 - ML, 3 - TL, 4 - RE, 239 - execfail, 179 - no patch
  pid_t fork_pid = fork();
  if (fork_pid == 0) { // It's out child, where we will execute program
    if (TL) {
      set_time_limit(TL);
    }
    if (ML) {
      set_memory_limit(ML);
    }
    if (!force_patch && init_safe_mode() != 0) exit(179);
    if (path == NULL) exit(239);
    execve(path, args, env_args);
    exit(239);
  } else {
    int status;
    wait(&status);
    if (status & SECURITY_VIOLATION_STATUS) {
      return 1;
    }
    if (status & MEMORY_LIMIT_STATUS) {
      return 2;
    }
    if (status & TIME_LIMIT_STATUS) {
      return 3;
    }
    status &= (0xffff); // Let's store only lowest bits
    status = WEXITSTATUS(status);
    if (status == 239 || status == 179) return status;
    if (status) {
      return 4;
    }
    return 0;
  }
}