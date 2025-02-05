#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "olog.h"

static void __olog_write_current_time(const char *format, FILE *file);

static inline void __olog_write_entry_to_console(
  const olog_entry_t *entry, va_list args);

static inline void __olog_write_entry_to_log_file(
  const olog_entry_t *entry, va_list args);

static struct {
  FILE *log_file;
  olog_level_t level;
  bool muted;
} s_olog_state = {
  .muted = false,
};

int olog_init(const olog_conf_t *conf)
{
  s_olog_state.log_file = fopen(conf->log_file_path, "a");
  if (s_olog_state.log_file == NULL) { return 1; }

  s_olog_state.level = conf->level;

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

  __olog_write_entry_to_console(entry, args);

  if (s_olog_state.log_file)
  {
    __olog_write_entry_to_log_file(entry, args);
  }
}

void __olog_write_entry_to_console(const olog_entry_t *entry, va_list args)
{
  static const char *s_sender_info_format =
    "\033[2;3;37m%s(%s:%d)\033[0m\n";

  static const char *s_level_prefixes[OLOG_LEVEL_COUNT] = {
    "\033[1;37mtrace\033[0m: \033[3;37m",
    "\033[1;34mdebug\033[0m: \033[3;37m",
    "\033[1;32minfo\033[0m: ",
    "\007\033[1;5;33mwarn\033[0m: ",
    "\007\033[1;31merror\033[0m: ",
    "\007\033[1;41;97m fatal \033[0m: ",
  };

  static const char *s_timestamp_format = "\033[37;2m[%H:%M:%S]\033[0m ";

  printf(s_sender_info_format,
    entry->sender_file_path,
    entry->sender_function_name,
    entry->sender_line_number);

  __olog_write_current_time(s_timestamp_format, stdout);
  fputs(s_level_prefixes[entry->level], stdout);
  vprintf(entry->message_format, args);
  fputs("\n\n", stdout);
}

void __olog_write_entry_to_log_file(
  const olog_entry_t *entry, va_list args)
{
  static const char *s_sender_info_format = "%s(%s:%d)\n";

  static const char *s_level_prefixes[OLOG_LEVEL_COUNT] = {
    "trace: ",
    "debug: ",
    "info:  ",
    "warn:  ",
    "error: ",
    "fatal: ",
  };

  static const char *s_timestamp_format = "[%d.%m.%Y %H:%M:%S] ";

  fprintf(s_olog_state.log_file,
    s_sender_info_format,
    entry->sender_file_path,
    entry->sender_function_name,
    entry->sender_line_number);

  __olog_write_current_time(s_timestamp_format, s_olog_state.log_file);
  fputs(s_level_prefixes[entry->level], s_olog_state.log_file);
  vfprintf(s_olog_state.log_file, entry->message_format, args);
  fputs("\n", s_olog_state.log_file);
}

void __olog_write_current_time(const char *format, FILE *file)
{
  static char strbuf[64];
  time_t current_time = time(NULL);
  struct tm *timestamp = localtime(&current_time);
  strftime(strbuf, sizeof(strbuf) / sizeof(strbuf[0]), format, timestamp);
  fputs(strbuf, file);
}
