#include <iostream>
#include "IPD_lib/include/Engine.h"

// TODO go through code and remove unnecessary statics, static programs exist for execution of whole program
int main(int argc, char* argv[]) {

        try {
            Engine::Engine e(argc, argv);
            std::cout << "Engine initialized successfully\n";
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            return 1;
        }

}