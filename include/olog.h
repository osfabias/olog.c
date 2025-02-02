#pragma once

#include <stdarg.h>

#ifdef _MSC_VER
#ifdef OLOG_EXPORT
#define OLOG_EXPORT declspec(dllexport)
#else
#define OLOG_EXPORT declspec(dllimport)
#endif
#else
#define OLOG_EXPORT
#endif

#ifdef __cplusplus
#define OLOG_EXTERN extern "C"
#else
#define OLOG_EXTERN extern
#endif

#define OLOG_API OLOG_EXTERN OLOG_EXPORT

typedef enum {
  OLOG_LEVEL_TRACE,
  OLOG_LEVEL_DEBUG,
  OLOG_LEVEL_INFO,
  OLOG_LEVEL_WARN,
  OLOG_LEVEL_ERROR,
  OLOG_LEVEL_FATAL,

  OLOG_LEVEL_COUNT
} olog_level_t;

typedef struct {
  const char *file_path;
  olog_level_t level;
} olog_config_t;

typedef struct {
  const char *file_path;
  const char *function_name;
  const char *message_format;
  unsigned line_number;
  olog_level_t level;
} olog_entry_t;

OLOG_API int olog_init(const olog_config_t *config);

OLOG_API void olog_deinit(void);

OLOG_API void olog_mute(void);

OLOG_API void olog_unmute(void);

OLOG_API void olog_set_level(olog_level_t level);

OLOG_API void olog_log(const olog_entry_t *entry, ...);

OLOG_API void olog_vlog(const olog_entry_t *entry, va_list args);

#define OLOG_MAKE_LOG_ENTRY(lvl, fmt) \
  {                                   \
    .file_path = __FILE__,            \
    .function_name = __func__,        \
    .message_format = fmt,            \
    .line_number = __LINE__,          \
    .level = lvl,                     \
  }

#define olog_trace(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_TRACE, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }

#define olog_debug(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_DEBUG, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }

#define olog_info(format, ...)                      \
  {                                                 \
    const olog_entry_t entry =                      \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_INFO, format); \
    olog_log(&entry, ##__VA_ARGS__);                \
  }

#define olog_warn(format, ...)                      \
  {                                                 \
    const olog_entry_t entry =                      \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_WARN, format); \
    olog_log(&entry, ##__VA_ARGS__);                \
  }

#define olog_error(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_ERROR, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }

#define olog_fatal(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_FATAL, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }
