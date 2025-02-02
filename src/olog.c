#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "olog.h"

static void write_timestamp(const char *format, FILE *file);
static inline void write_entry_to_console(
  const olog_entry_t *entry, va_list args);
static inline void write_entry_to_file(
  const olog_entry_t *entry, va_list args);

static struct {
  FILE *log_file;
  olog_level_t level;
  bool muted;
} s_olog_state = {
  .muted = false,
};

int olog_init(const olog_config_t *config)
{
  s_olog_state.log_file = fopen(config->file_path, "a");
  if (s_olog_state.log_file == NULL) { return 1; }

  s_olog_state.level = config->level;

  return 0;
}

void olog_deinit(void) { fclose(s_olog_state.log_file); }

void olog_mute(void) { s_olog_state.muted = true; }

void olog_unmute(void) { s_olog_state.muted = false; }

void olog_set_level(olog_level_t level) { s_olog_state.level = level; }

void olog_log(const olog_entry_t *entry, ...)
{
  va_list args;
  va_start(args, entry);
  olog_vlog(entry, args);
  va_end(args);
}

void olog_vlog(const olog_entry_t *entry, va_list args)
{
  if (s_olog_state.muted || entry->level < s_olog_state.level) { return; }

  write_entry_to_console(entry, args);

  if (s_olog_state.log_file) { write_entry_to_file(entry, args); }
}

void write_entry_to_console(const olog_entry_t *entry, va_list args)
{
  static const char *info_format = "\033[2;3;37m%s(%s:%d)\033[0m\n";

  static const char *level_prefixes[OLOG_LEVEL_COUNT] = {
    "\033[1;37mtrace\033[0m: \033[3;37m",
    "\033[1;34mdebug\033[0m: \033[3;37m",
    "\033[1;32minfo\033[0m: ",
    "\007\033[1;5;33mwarn\033[0m: ",
    "\007\033[1;31merror\033[0m: ",
    "\007\033[1;41;97m fatal \033[0m: ",
  };

  printf(info_format,
    entry->file_path,
    entry->function_name,
    entry->line_number);

  write_timestamp("\033[37;2m[%H:%M:%S]\033[0m ", stdout);
  fputs(level_prefixes[entry->level], stdout);
  vprintf(entry->message_format, args);
  fputs("\n\n", stdout);
}

void write_entry_to_file(const olog_entry_t *entry, va_list args)
{
  static const char *info_format = "%s(%s:%d)\n";

  static const char *level_prefixes[OLOG_LEVEL_COUNT] = {
    "trace: ",
    "debug: ",
    "info:  ",
    "warn:  ",
    "error: ",
    "fatal: ",
  };

  fprintf(s_olog_state.log_file,
    info_format,
    entry->file_path,
    entry->function_name,
    entry->line_number);

  write_timestamp("[%d.%m.%Y %H:%M:%S] ", s_olog_state.log_file);
  fputs(level_prefixes[entry->level], s_olog_state.log_file);
  vfprintf(s_olog_state.log_file, entry->message_format, args);
  fputs("\n", s_olog_state.log_file);
}

void write_timestamp(const char *format, FILE *file)
{
  static char timebuf[64];
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(timebuf, sizeof(timebuf) / sizeof(timebuf[0]), format, tm);
  fputs(timebuf, file);
}
