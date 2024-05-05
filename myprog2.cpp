#include <iostream>
#include <unistd.h>  // For sleep()

// Function declarations
void C() {
    //std::cout << "Entering C" << std::endl;
    sleep(7);  // Sleep for 2 seconds
    //std::cout << "Exiting C" << std::endl;
}

void B() {
    //std::cout << "Entering B" << std::endl;
    sleep(5);  // Sleep for 2 seconds
    C();       // Call function C
    //std::cout << "Exiting B" << std::endl;
}

void A() {
    //std::cout << "Entering A" << std::endl;
    sleep(2);  // Sleep for 2 seconds
    B();       // Call function B
    //std::cout << "Exiting A" << std::endl;
}

int main() {
    //std::cout << "Program started in main" << std::endl;
    A();  // Start the function stack by calling A
    //std::cout << "Program finished in main" << std::endl;
    return 0;
}
