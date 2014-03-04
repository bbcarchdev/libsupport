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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_libsupport.h"

static int log_open(void);
static int log_parse_level(const char *level);
static int log_parse_facility(const char *facility);

static int log_is_open, log_use_config, log_stderr = 0, log_level = LOG_NOTICE, log_facility = LOG_DAEMON, log_syslog = 1;
const char *log_ident = "(unknown)";

int
log_set_ident(const char *ident)
{
	log_use_config = 0;
	log_ident = ident;
	log_reset();
	return 0;
}

int
log_set_level(int level)
{
	log_use_config = 0;
	log_level = level;
	log_reset();
	return 0;
}

int
log_set_facility(int facility)
{
	log_use_config = 0;
	log_facility = facility;
	log_reset();
	return 0;
}

int
log_set_stderr(int val)
{
	log_use_config = 0;
	log_stderr = val;
	log_reset();
	return 0;	
}

int
log_set_syslog(int val)
{
	log_use_config = 0;
	log_syslog = val;
	log_reset();
	return 0;	
}

int
log_set_use_config(int val)
{
	if(log_use_config == val)
	{
		return 0;
	}
	log_use_config = val;
	log_reset();
	return 0;
}

int
log_reset(void)
{
	if(log_is_open && log_syslog)
	{
		closelog();
	}
	log_is_open = 0;
	return 0;
}

void
log_vprintf(int level, const char *fmt, va_list ap)
{
	if(!log_is_open)
	{
		log_open();
	}
	if(level > log_level)
	{
		return;
	}
	if(log_syslog)
	{
		vsyslog(level, fmt, ap);
	}
	else
	{
		fprintf(stderr, "%s: ", log_ident);
		vfprintf(stderr, fmt, ap);
	}
}

void
log_printf(int level, const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	log_vprintf(level, fmt, ap);
	va_end(ap);
}

static int
log_open(void)
{
	int logopt;
	char buf[32];
	const char *ident;
	
	if(log_is_open)
	{
		log_reset();
	}
	logopt = LOG_NDELAY|LOG_PID;
	if(log_use_config)
	{
		log_stderr = config_get_bool("log:stderr", 0);
		log_syslog = config_get_bool("log:syslog", 1);
		config_get("log:level", "notice", buf, sizeof(buf));
		log_level = log_parse_level(buf);
		config_get("log:facility", "user", buf, sizeof(buf));
		log_facility = log_parse_facility(buf);
		config_get("log:ident", "(none)", buf, sizeof(buf));
		ident = buf;
	}
	else
	{
		ident = log_ident;
	}
	if(log_stderr)
	{
		logopt |= LOG_PERROR;
	}
	if(log_syslog)
	{
		openlog(ident, logopt, log_facility);
	}
	log_is_open = 1;
	return 0;
}

static int
log_parse_level(const char *level)
{
	if(!strcasecmp(level, "emerg") || !strcasecmp(level, "emergency")) return LOG_EMERG;
	if(!strcasecmp(level, "alert")) return LOG_ALERT;
	if(!strcasecmp(level, "crit") || !strcasecmp(level, "critical")) return LOG_CRIT;
	if(!strcasecmp(level, "err") || !strcasecmp(level, "error")) return LOG_ERR;
	if(!strcasecmp(level, "warn") || !strcasecmp(level, "warning")) return LOG_WARNING;
	if(!strcasecmp(level, "notice")) return LOG_NOTICE;
	if(!strcasecmp(level, "info")) return LOG_INFO;
	if(!strcasecmp(level, "debug")) return LOG_DEBUG;
	return atoi(level);
}

static int
log_parse_facility(const char *facility)
{
#ifdef LOG_AUTH
	if(!strcasecmp(facility, "auth")) return LOG_AUTH;
#endif
#ifdef LOG_AUTHPRIV
	if(!strcasecmp(facility, "authpriv")) return LOG_AUTHPRIV;
#endif
#ifdef LOG_CRON
	if(!strcasecmp(facility, "cron")) return LOG_CRON;
#endif
#ifdef LOG_DAEMON
	if(!strcasecmp(facility, "daemon")) return LOG_DAEMON;
#endif
#ifdef LOG_FTP
	if(!strcasecmp(facility, "ftp")) return LOG_FTP;
#endif
#ifdef LOG_KERN
	if(!strcasecmp(facility, "kern")) return LOG_KERN;
#endif
#ifdef LOG_LPR
	if(!strcasecmp(facility, "lpr")) return LOG_LPR;
#endif
#ifdef LOG_MAIL
	if(!strcasecmp(facility, "mail")) return LOG_MAIL;
#endif
#ifdef LOG_NEWS
	if(!strcasecmp(facility, "news")) return LOG_NEWS;
#endif
#ifdef LOG_SECURITY
	if(!strcasecmp(facility, "security")) return LOG_SECURITY;
#endif
#ifdef LOG_SYSLOG
	if(!strcasecmp(facility, "syslog")) return LOG_SYSLOG;
#endif
#ifdef LOG_REMOTAUTH
	if(!strcasecmp(facility, "remoteauth")) return LOG_REMOTEAUTH;
#endif
#ifdef LOG_UUCP
	if(!strcasecmp(facility, "uucp")) return LOG_UUCP;
#endif
	if(!strcasecmp(facility, "user")) return LOG_USER;
	if(!strcasecmp(facility, "local0")) return LOG_LOCAL0;
	if(!strcasecmp(facility, "local1")) return LOG_LOCAL1;
	if(!strcasecmp(facility, "local2")) return LOG_LOCAL2;
	if(!strcasecmp(facility, "local3")) return LOG_LOCAL3;
	if(!strcasecmp(facility, "local4")) return LOG_LOCAL4;
	if(!strcasecmp(facility, "local5")) return LOG_LOCAL5;
	if(!strcasecmp(facility, "local6")) return LOG_LOCAL6;
	if(!strcasecmp(facility, "local7")) return LOG_LOCAL7;
	return LOG_USER;
}
