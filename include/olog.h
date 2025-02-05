/**
 * @file olog.h
 * @brief Main functions of olog.
 */
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

/**
 * @enum olog_level_t
 * @brief Logging level.
 */
typedef enum {
  /**
   * Messages with very low-level implementation details. Messages using
   * this log level would normally only be temporary and would expect to
   * be removed before releasing the project.
   */
  OLOG_LEVEL_TRACE,

  /**
   * Informational messages intended for developers working on the
   * project itself. These messages will not typically be of interest
   * to other users building the project and will often be closely
   * related to internal implementation details.
   */
  OLOG_LEVEL_DEBUG,

  /**
   * The main interesting messages that project users might be
   * interested in. Ideally these should be concise, no more than a
   * single line, but still informative.
   */
  OLOG_LEVEL_INFO,

  /**
   * Messages that intented to warn project users about some
   * wrong or unexpected behaviour, that could lead to unwanted
   * consequences.
   */
  OLOG_LEVEL_WARN,

  /**
   * Messages that indicates a significant problem that requires
   * attention but does not necessarily halts the application.
   * These errors usually mean that some functionality has failed
   * and may require corrective action.
   */
  OLOG_LEVEL_ERROR,

  /**
   * Represents a critical issue that causes the application to
   * terminate. Should be used when the system cannot recover from the
   * problem, and immediate shutdown or emergency handling is required.
   */
  OLOG_LEVEL_FATAL,

  /* Number of olog levels. */
  OLOG_LEVEL_COUNT
} olog_level_t;

/**
 * @struct olog_conf_t
 * @brief Olog configuration.
 */
typedef struct {
  /**
   * File path to the log file. If file at path already exists, log
   * new messages will be appended at the end.
   */
  const char *log_file_path;

  /* Minimum level of the messages to be logged. */
  olog_level_t level;
} olog_conf_t;

/**
 * @struct olog_entry_t
 * @brief Log entry.
 */
typedef struct {
  /* Path of the file from which the entry is sent. */
  const char *sender_file_path;

  /* Name of the function from which the entry is sent. */
  const char *sender_function_name;

  /**
   * Message format. Formatting options corresponds to those that are
   * used for printf
   */
  const char *message_format;

  /* Number of line from which the entry is sent. */
  unsigned sender_line_number;

  /* Level of the entry. */
  olog_level_t level;
} olog_entry_t;

/**
 * @brief Initializes olog.
 *
 * If olog is already initialized, the behaviour is undefined.
 *
 * @param conf Pointer to the configutaion struct.
 *
 * @return Returns 0 on success, otherwise returns an error code.
 */
OLOG_API int olog_init(const olog_conf_t *conf);

/**
 * @brief Deinitializes olog.
 *
 * If olog is already deinitialized, the behaviour is undefined.
 */
OLOG_API void olog_deinit(void);

/**
 * @brief Stops olog logging.
 *
 * Messages sent while the olog was muted will be lost.
 */
OLOG_API void olog_mute(void);

/**
 * @brief Resumes olog logging.
 */
OLOG_API void olog_unmute(void);

/* @brief Sets minimum logging level. */
OLOG_API void olog_set_level(olog_level_t level);

/**
 * @brief Logs message.
 *
 * @param entry Pointer to an entry structure.
 * @param ... Variadic arguments.
 */
OLOG_API void olog_log(const olog_entry_t *entry, ...);

/**
 * @brief Logs message.
 *
 * @param entry Pointer to an entry structure.
 * @param args Variadic arguments list.
 */
OLOG_API void olog_vlog(const olog_entry_t *entry, va_list args);

/* @brief Handy macro for easily creating log entry. */
#define OLOG_MAKE_LOG_ENTRY(lvl, fmt) \
  {                                   \
    .sender_file_path = __FILE__,     \
    .sender_function_name = __func__, \
    .sender_line_number = __LINE__,   \
    .message_format = fmt,            \
    .level = lvl,                     \
  }

/* @brief Logs trace message. */
#define olog_trace(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_TRACE, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }

/* @brief Logs debug message. */
#define olog_debug(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_DEBUG, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }

/* @brief Logs info message. */
#define olog_info(format, ...)                      \
  {                                                 \
    const olog_entry_t entry =                      \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_INFO, format); \
    olog_log(&entry, ##__VA_ARGS__);                \
  }

/* @brief Logs warn message. */
#define olog_warn(format, ...)                      \
  {                                                 \
    const olog_entry_t entry =                      \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_WARN, format); \
    olog_log(&entry, ##__VA_ARGS__);                \
  }

/* @brief Logs error message. */
#define olog_error(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_ERROR, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }

/* @brief Logs fatal message. */
#define olog_fatal(format, ...)                      \
  {                                                  \
    const olog_entry_t entry =                       \
      OLOG_MAKE_LOG_ENTRY(OLOG_LEVEL_FATAL, format); \
    olog_log(&entry, ##__VA_ARGS__);                 \
  }
