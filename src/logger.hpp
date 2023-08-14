#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>

namespace logger {
    const int LEVEL_DEBUG = 0;
    const int LEVEL_INFO = 1;
    const int LEVEL_WARN = 2;
    const int LEVEL_ERROR = 3;
    const int LEVEL_FATA = 4;
    const std::string LEVELS[5] = {"\x1b[36mDEBUG\x1b[0m", "\x1b[32mINFO\x1b[0m", "\x1b[33mWARN\x1b[0m", "\x1b[31mERROR\x1b[0m", "\x1b[31mFATAL\x1b[0m"};

    std::string align_right(std::string str, int width) {
        int str_len = str.length();
        if (str_len >= width) {
            return str;
        }
        int spaces = width - str_len;
        std::string result = "";
        for (int i = 0; i < spaces; i++) {
            result += " ";
        }
        result += str;
        return result;
    }

    void Log(int level_index, std::string message) {
        std::cout << "\x1b[1m" << align_right(LEVELS[level_index], 21) << "\x1b[0m" << " " << message << std::endl;
    }

    void LogError(std::string message, const char* error) {
        std::cout << "\x1b[1m" << align_right(LEVELS[LEVEL_ERROR], 21) << "\x1b[0m" << " " << message << ": " << error << std::endl;
    }
}

#endif