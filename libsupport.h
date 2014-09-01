/*
 * Copyright 2013 Mo McRoberts.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef LIBSUPPORT_H_
# define LIBSUPPORT_H_                 1

# include <stdarg.h>
# include <syslog.h>
# include <errno.h>

int config_init(int (*defaults_cb)(void));
int config_load(const char *default_path);
int config_set(const char *key, const char *value);
int config_set_default(const char *key, const char *value);
size_t config_get(const char *key, const char *defval, char *buf, size_t bufsize);
const char *config_getptr_unlocked(const char *key, const char *defval);
char *config_geta(const char *key, const char *defval);
int config_get_int(const char *key, int defval);
int config_get_bool(const char *key, int defval);
int config_get_all(const char *section, const char *key, int (*fn)(const char *key, const char *value, void *data), void *data);

void log_vprintf(int level, const char *fmt, va_list ap);
void log_printf(int level, const char *fmt, ...);
int log_reset(void);
int log_set_level(int level);
int log_set_ident(const char *ident);
int log_set_facility(int facility);
int log_set_syslog(int val);
int log_set_stderr(int val);
int log_set_use_config(int val);

#endif /*!LIBSUPPORT_H_*/
