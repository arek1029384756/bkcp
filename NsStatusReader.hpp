#ifndef NS_STATUS_READER
#define NS_STATUS_READER

#include "StatusReader.hpp"

namespace status_reader {

    class NsStatusReader : public StatusReader {

        FileList& parse(const LineList& lines) {
            std::string pfx("");
            m_files.clear();

            for(auto x : lines) {
                std::size_t pos = x.find(":");
                if(pos != std::string::npos) {
                    pfx = x.substr(0, pos);
                    continue;
                }

                pos = x.find("M ");
                if(pos != std::string::npos) {
                    std::string s = x.substr(pos + 2, x.length() - pos - 2);
                    m_files.emplace_back(pfx + "/" + s);
                    continue;
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

