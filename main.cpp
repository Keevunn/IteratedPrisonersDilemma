#include <iostream>
#include "IPD_lib/include/Engine.h"

int main(int argc, char* argv[]) {

        try {
            Engine::Engine e(argc, argv);
            std::cout << "Engine initialized successfully\n";

            e.runIPD();
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            return 1;
        }

}