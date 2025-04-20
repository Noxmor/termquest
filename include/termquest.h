#ifndef TQ_TERMQUEST_H
#define TQ_TERMQUEST_H

#include "interface.h"

void termquest_run(void);

void termquest_push_interface(Interface* inf);

Interface* termquest_pop_interface(void);

void termquest_create_save_file(const char* name);

void termquest_quit(void);

#endif
