#include <iostream>

#include "App.hpp"

int main() {
    try{
        App app {};
        app.run();
    }
    catch (const std::exception &ex){
        std::cerr << "Error occurred: " << ex.what() << '\n';
    }

    return 0;
}
