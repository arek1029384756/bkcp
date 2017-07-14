#ifndef NS_STATUS_READER
#define NS_STATUS_READER

#include <set>
#include "StatusReader.hpp"

namespace status_reader {

    const std::set<std::string> marks = {"M ", "A "};

    class NsStatusReader : public StatusReader {

        FileList& parse(const LineList& lines) {
            std::string pfx("");
            m_files.clear();

            for(auto line : lines) {
                std::size_t pos = line.find(":");
                if(pos != std::string::npos) {
                    pfx = line.substr(0, pos);
                    continue;
                }

                for(const auto& mark : marks) {
                    pos = line.find(mark);
                    if(pos != std::string::npos) {
                        std::string s = line.substr(pos + 2, line.length() - pos - 2);
                        m_files.emplace_back(pfx + "/" + s);
                    }
                }
            }

            return m_files;
        }

        public:
        static StatusReader* getInstance() {
            static NsStatusReader n;
            return &n;
        }
    };
}

#endif

