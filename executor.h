//
// Created by gritukan on 8/16/17.
//

#include <sys/ptrace.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdlib.h>

#ifndef EXECUTOR_EXECUTOR_H
#define EXECUTOR_EXECUTOR_H
int32_t init_safe_mode();
void set_time_limit(uint64_t time);
void set_memory_limit(uint64_t memory);
int32_t execute(char *path, char **args, char **env_args, uint64_t TL, uint64_t ML, int32_t force_patch);


#endif //EXECUTOR_EXECUTOR_H
