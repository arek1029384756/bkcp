#include <iostream>
#include "NsStatusReader.hpp"

namespace {
    class App {
        int m_argc;
        char** m_argv;

        public:
        App(int argc, char** argv)
            : m_argc(argc), m_argv(argv) {}

        int run() {
            try {
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

