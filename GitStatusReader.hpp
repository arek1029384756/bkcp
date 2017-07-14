#ifndef GIT_STATUS_READER
#define GIT_STATUS_READER

#include <set>
#include <algorithm>
#include "StatusReader.hpp"

namespace status_reader {

    class GitStatusReader : public StatusReader {

        static const std::set<std::string> marks;

        std::list<std::string> m_tokens;

        std::string formatLine(std::string line) {
            line.erase(std::remove_if(line.begin(), 
                        line.end(),
                        [](char x){return std::isspace(x);}),
                        line.end());
            return line;
        }

        void findMarkedVal(const std::string& line) {
            for(const auto& mark : marks) {
                std::size_t pos = line.find(mark);
                if(pos != std::string::npos) {
                    std::string s = line.substr(pos + mark.length(), line.length() - pos - mark.length());
                    m_files.emplace_back(s);
                }
            }
        }

        FileList& parse(const LineList& lines) {
            m_files.clear();

            for(auto line : lines) {
                auto line_f = formatLine(line);
                findMarkedVal(line_f);
           }

            return m_files;
        }

        public:
        static StatusReader* getInstance() {
            static GitStatusReader n;
            return &n;
        }
    };
    const std::set<std::string> GitStatusReader::marks = {"modified:", "newfile:"};
}

#endif

