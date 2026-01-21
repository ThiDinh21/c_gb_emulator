#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

typedef enum
{
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
} LogLevel;

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"

// Main logging macro
#define LOG(level, fmt, ...)                                                         \
    do                                                                               \
    {                                                                                \
        if (level >= LOG_LEVEL)                                                      \
        {                                                                            \
            const char *color = "";                                                  \
            const char *level_str = "";                                              \
            FILE *stream = stdout;                                                   \
                                                                                     \
            switch (level)                                                           \
            {                                                                        \
            case LOG_DEBUG:                                                          \
                color = COLOR_BLUE;                                                  \
                level_str = "DEBUG";                                                 \
                break;                                                               \
            case LOG_INFO:                                                           \
                color = COLOR_RESET;                                                 \
                level_str = "INFO";                                                  \
                break;                                                               \
            case LOG_WARNING:                                                        \
                color = COLOR_YELLOW;                                                \
                level_str = "WARN";                                                  \
                stream = stderr;                                                     \
                break;                                                               \
            case LOG_ERROR:                                                          \
                color = COLOR_RED;                                                   \
                level_str = "ERROR";                                                 \
                stream = stderr;                                                     \
                break;                                                               \
            }                                                                        \
                                                                                     \
            fprintf(stream, "%s[%s] %s:%d: " fmt "%s\n",                             \
                    color, level_str, __FILE__, __LINE__, __VA_ARGS__, COLOR_RESET); \
        }                                                                            \
    } while (0)

// Helper macros
#define LOG_DEBUG(...) LOG(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LOG(LOG_INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG(LOG_WARNING, __VA_ARGS__)
#define LOG_ERROR(...) LOG(LOG_ERROR, __VA_ARGS__)

#endif

#endif