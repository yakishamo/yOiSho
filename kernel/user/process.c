#include "process.h"
#include "../../common/types64.h"

#define PROCESS_CHILDREN_MAX 128;

struct process {
	uint64_t pid;
	void *text;
	void *stack;
	Process parent;
	Process children[PROCESS_CHILDREN_MAX];
};
