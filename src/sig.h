#pragma once

#include "msc.h"
#include <signal.h>
#include <unistd.h>

int sig_handle_reset (void);
int sig_handle_alarm (void);
int sig_set (void);