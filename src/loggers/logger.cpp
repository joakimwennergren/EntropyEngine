#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "logger.h"

quill::Logger* logger_;

void InitializeQuill()
{
  // Start the backend thread
  quill::Backend::start();

  // Create and store the logger
  logger_ = quill::Frontend::create_or_get_logger(
    "root", quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1"));
}