#include <../../nrnconf.h>
#include "nrnmpiuse.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>

#if NRNMPI_DYNAMICLOAD /* to end of file */

/* temporarily turn off NRNMPI so we get globals but not nrnmpidec.h */
#undef NRNMPI
#include "nrnmpi.h"
#define NRNMPI 1

#include "mpispike.h"
#include "nrnmpi_def_cinc" /* nrnmpi global variables */
#include "nrnmpi_dynam_cinc" /* autogenerated file */

static void* load_mpi(const char* name) {
	int flag = RTLD_NOW | RTLD_GLOBAL;
	void* handle = dlopen(name, flag);
	if (!handle) {
		fprintf(stderr, "load_mpi: %s\n", dlerror());
	}
	return handle;
}

static void load_nrnmpi(const char* name) {
	int i;
	int flag = RTLD_NOW | RTLD_GLOBAL;
	void* handle = dlopen(name, flag);
	if (!handle) {
		fprintf(stderr, "load_nrnmpi: %s\n", dlerror());
		abort();
	}	
	for (i = 0; ftable[i].name; ++i) {
		void* p = dlsym(handle, ftable[i].name);
		if (!handle) {
			fprintf(stderr, "load_nrnmpi: %s\n", dlerror());
			/*printf("could not dynamically load symbol %s\n", ftable[i].name);*/
			abort();
		}	
		*ftable[i].ppf = p;
	}
}

void nrnmpi_load() {
#if DARWIN
	void* handle = load_mpi("libmpi.dylib");
	if (handle) {
		/* see man dyld */
		load_nrnmpi("@loader_path/libnrnmpi.dylib");
	}else{
printf("Is openmpi installed? If not in default location, need a LD_LIBRARY_PATH.\n");
		abort();
	}
#else
	void* handle = load_mpi("libmpi.so");
	if (handle) {
		load_nrnmpi("libnrnmpi.so");
	}else{
		handle = load_mpi("libmpl.so");
		handle = load_mpi("libmpich.so");
		load_nrnmpi("libnrnmpi.so");
	}
#endif
	if (!handle) {
		printf("could not dynamically load libmpi.so or libmpich2.so\n");
		abort();
	}	
}
#endif
