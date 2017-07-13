#include <iostream>
#include "NsStatusReader.hpp"

int main(int argc, char** argv) {
    try {
        auto ns = status_reader::NsStatusReader::getInstance();
        auto lf = ns->getFileList((argc > 1) ? argv[1] : "<empty>");
        auto dest = file_ops::FileOps::makeDir((argc > 2) ? argv[2] : "./");
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

