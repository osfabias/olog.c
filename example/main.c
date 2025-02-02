#include <olog.h>

int main(void)
{
  const olog_config_t config = {
    .file_path = "logs.txt",
    .level = OLOG_LEVEL_TRACE,
  };

  olog_init(&config);

  olog_trace("trace message");
  olog_debug("debug message");
  olog_info("info message");
  olog_warn("warn message");
  olog_error("error message");
  olog_fatal("fatal message");

  olog_deinit();

  return 0;
}
