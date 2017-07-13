#include <iostream>
#include "NsStatusReader.hpp"

namespace {
    class App {
        int m_argc;
        char** m_argv;

        void showUsage() {
            std::cout << "BacKup CoPy - local safety copy utility, (C) A.Antoniak" << std::endl;
            std::cout << "Usage:" << std::endl;
            std::cout << "    " << m_argv[0] << " COMMAND [DIRECTORY]" << std::endl;
            std::cout << "    COMMAND     - { \"nosilo status\", \"git status\" }" << std::endl;
            std::cout << "    DIRECTORY   - destination (leave empty for current directory)" << std::endl;
        }

        void checkOptions() {
            if(m_argc < 2 || std::string(m_argv[1]) == "--help") {
                showUsage();
                throw std::runtime_error("");
            }
        }

        public:
        App(int argc, char** argv)
            : m_argc(argc), m_argv(argv) {}

        int run() {
            try {
                checkOptions();

                auto ns = status_reader::NsStatusReader::getInstance();
                auto lf = ns->getFileList((m_argc > 1) ? m_argv[1] : "<empty>");
                auto dest = file_ops::FileOps::makeDir((m_argc > 2) ? m_argv[2] : "./");
                for(auto x : lf) {
                    file_ops::FileOps::copyFile(x, dest);
                }
            } catch(const std::runtime_error& e) {
                std::cout << e.what() << std::endl;
                return EXIT_FAILURE;
            } catch(...) {
                std::cout << "Unknown exception! Something really bad happened"
                    << std::endl;
                return EXIT_FAILURE;
            }
            return 0;
        }
    };
}

int main(int argc, char** argv) {
    App a(argc, argv);
    return a.run();
}

