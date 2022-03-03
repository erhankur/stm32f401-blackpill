#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    LOG_LEVEL_OFF,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level_t;

/* ! Do not call LOG functions from ISR context! */
#define LOG_O(format,...)       log_write_format(LOG_LEVEL_OFF, format, ##__VA_ARGS__)
#define LOG_E(format,...)       log_write_format(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define LOG_W(format,...)       log_write_format(LOG_LEVEL_WARN, format, ##__VA_ARGS__)
#define LOG_I(format,...)       log_write_format(LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOG_D(format,...)       log_write_format(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)

void log_level_init(const char *tag, log_level_t level);
void log_hex_dump(log_level_t level, const char *caption, void *buf, size_t buf_size);
void log_write_format(log_level_t level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
