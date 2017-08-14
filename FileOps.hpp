#ifndef FILEOPS_H
#define FILEOPS_H

#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <exception>
#include <stdexcept>    //covering g++ version differences

namespace file_ops {

    class FileOps {
        static std::string format0(int val, std::size_t digits = 2) {
            auto s = std::to_string(val);
            auto len = s.length();
            if(len < digits) {
                s = std::string(digits - len, '0') + s;
            }
            return s;
        }

        public:
        static void execute(const std::string& cmd) {
            int rs = system(cmd.c_str());
            if(rs != 0) {
                throw std::runtime_error(std::string("Command '") + cmd + std::string("' failed: ") + std::to_string(rs));
            }
        }

        static const std::string makeDir(const std::string& dir) {
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            auto pt = std::localtime(&now_c);
            auto s = dir + std::string("bkp_") + std::to_string(pt->tm_year + 1900) + std::string("_") + 
                format0(pt->tm_mon + 1) + std::string("_") +
                format0(pt->tm_mday) + std::string("_") +
                format0(pt->tm_hour) + std::string("_") +
                format0(pt->tm_min) + std::string("_") +
                format0(pt->tm_sec);
            auto scmd = std::string("mkdir ") + s;
            std::cout << "Running '" << scmd << "'" << std::endl;

            execute(scmd);

            return s;
        }

        static void copyFile(const std::string& filename, const std::string& destination) {
            auto cp_cmd = std::string("cp --parents ") + filename + std::string(" ") + destination;
            std::cout << cp_cmd << std::endl;
            execute(cp_cmd);
        }
    };
}

#endif

