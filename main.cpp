#include <iostream>
#include "NsStatusReader.hpp"
#include "GitStatusReader.hpp"

namespace {
    class App {
        int m_argc;
        char** m_argv;

        void showUsage() const {
            std::cout << "BacKup CoPy ver " << getVersion() << std::endl;
            std::cout << "Local safety copy utility, (C) A.Antoniak" << std::endl;
            std::cout << "    " << std::endl;
            std::cout << "    1) copy" << std::endl;
            std::cout << "        $ cd PROJECT_DIRECTORY" << std::endl;
            std::cout << "        $ " << m_argv[0] << " COMMAND [BACKUP_DIRECTORY]" << std::endl;
            std::cout << "        " << std::endl;
            std::cout << "    2) reverse copy" << std::endl;
            std::cout << "        $ cd BACKUP_DIRECTORY" << std::endl;
            std::cout << "        $ find . -type f | xargs -I {} cp --parents {} PROJECT_DIRECTORY" << std::endl;
            std::cout << "        " << std::endl;
            std::cout << "    options:" << std::endl;
            std::cout << "        COMMAND            - { \"nosilo status\", \"git status\" }" << std::endl;
            std::cout << "        BACKUP_DIRECTORY   - backup copy destination (leave empty for current directory)" << std::endl;
            std::cout << "        PROJECT_DIRECTORY  - working project directory" << std::endl;
        }

        void checkOptions() const {
            if(m_argc < 2 || std::string(m_argv[1]) == "--help") {
                showUsage();
                throw std::runtime_error("");
            }
        }

        status_reader::StatusReader* checkVersionControl() const {
            checkOptions();

            auto pos = std::string(m_argv[1]).find("silo");
            if(pos != std::string::npos) {
                return status_reader::NsStatusReader::getInstance();
            } else {
                return status_reader::GitStatusReader::getInstance();
            }
        }

        public:
        App(int argc, char** argv)
            : m_argc(argc), m_argv(argv) {}

        static const std::string& getVersion() {
            static const std::string ver = "1.5";
            return ver;
        }

        int run() {
            try {
                auto ns = checkVersionControl();

                auto lf = ns->getFileList((m_argc > 1) ? m_argv[1] : "<empty>");
                if(lf.empty()) {
                    throw std::runtime_error("Nothing to copy");
                }

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

