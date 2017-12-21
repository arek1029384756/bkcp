#include <iostream>
#include <algorithm>
#include "NsStatusReader.hpp"
#include "GitStatusReader.hpp"

namespace {
    class App {
        std::vector<std::string> m_argv;
        static const std::set<std::string> commands;

        static const std::string getAvailableCommands() {
            return std::accumulate(commands.begin(),
                    commands.end(),
                    std::string(""),
                    [](const std::string& out, const std::string& cmd) {
                        auto coma = (out == "") ? std::string("") : std::string(", ");
                        return out + coma + std::string("\"") + cmd + std::string("\"");
                    });
        }

        static bool commandAvailable(const std::string& cmd) {
            auto it = commands.find(cmd);
            return it != commands.end();
        }

        void serviceCommand(status_reader::StatusReader* const statusReader) {
            auto lf = statusReader->getFileList(m_argv.at(1));
            if(lf.empty()) {
                throw std::runtime_error("Nothing to copy");
            }

            auto dest = file_ops::FileOps::makeDir(m_argv.at(2));
            for(auto x : lf) {
                file_ops::FileOps::copyFile(x, dest);
            }
        }

        void showUsage() const {
            std::cout << "BacKup CoPy ver " << getVersion() << std::endl;
            std::cout << "Local safety copy utility, (C) A.Antoniak" << std::endl;
            std::cout << "    " << std::endl;
            std::cout << "    1) copy" << std::endl;
            std::cout << "        $ cd PROJECT_DIRECTORY" << std::endl;
            std::cout << "        $ " << m_argv.at(0) << " COMMAND BACKUP_DIRECTORY" << std::endl;
            std::cout << "        " << std::endl;
            std::cout << "    2) reverse copy" << std::endl;
            std::cout << "        $ cd BACKUP_DIRECTORY" << std::endl;
            std::cout << "        $ " << m_argv.at(0) << " PROJECT_DIRECTORY" << std::endl;
            std::cout << "        " << std::endl;
            std::cout << "    options:" << std::endl;
            std::cout << "        COMMAND            - { " << getAvailableCommands() << " }" << std::endl;
            std::cout << "        BACKUP_DIRECTORY   - backup copy destination" << std::endl;
            std::cout << "        PROJECT_DIRECTORY  - working project directory" << std::endl;
        }

        void checkOptions() const {
            auto len = m_argv.size();
            if(len < 2  || len > 3 || m_argv.at(1) == "--help") {
                showUsage();
                throw std::runtime_error("");
            }
        }

        status_reader::StatusReader* getStatusReader() const {
            auto argN1 = m_argv.at(1);
            if(commandAvailable(argN1)) {
                auto pos = argN1.find("silo");
                if(pos != std::string::npos) {
                    return status_reader::NsStatusReader::getInstance();
                } else {
                    return status_reader::GitStatusReader::getInstance();
                }
            }

            return nullptr;
        }

        public:
        App(int argc, char** argv) {
            for(int i = 0; i < argc; ++i) {
                m_argv.emplace_back(argv[i]);
            }
        }

        static const std::string& getVersion() {
            static const std::string ver = "1.6";
            return ver;
        }

        int run() {
            try {
                checkOptions();

                auto srPtr = getStatusReader();
                if(srPtr) {
                    serviceCommand(srPtr);
                } else {
                    file_ops::FileOps::reverseCopyFiles(m_argv.at(1));
                }
            } catch(const std::exception& e) {
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
    const std::set<std::string> App::commands = { "nosilo status", "git status" };
}

int main(int argc, char** argv) {
    App a(argc, argv);
    return a.run();
}
