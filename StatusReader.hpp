#ifndef STATUS_READER
#define STATUS_READER

#include <fstream>
#include <list>
#include "FileOps.hpp"

namespace status_reader {
    typedef std::list<std::string> LineList;
    typedef std::list<std::string> FileList;

    class StatusReader {
        LineList m_lines;

        LineList& getStatusNative(const std::string& filename, const std::string& cmd) {
            std::ifstream ifs(filename);
            if(ifs.is_open()) {
                m_lines.clear();
                std::string line;
                while(std::getline(ifs, line)) {
                    m_lines.emplace_back(line);
                }
            } else {
                throw std::runtime_error(std::string("No output file for command '") + cmd + std::string("'"));
            }

            return m_lines;
        }

        protected:
        FileList m_files;
        virtual FileList& parse(const LineList& lines) = 0;

        public:
        static const std::string filename;

        virtual FileList& getFileList(const std::string& cmd) {
            std::string command = cmd + std::string(" > ") + StatusReader::filename;
            std::cout << "Running '" << cmd << "'" << std::endl;
            file_ops::FileOps::execute(command);

            auto l = getStatusNative(StatusReader::filename, cmd);
            std::string rm_output = std::string("rm -f ") + StatusReader::filename;
            file_ops::FileOps::execute(rm_output);

            return parse(l);
        }

    };
    const std::string StatusReader::filename = "status.output";
}

#endif

