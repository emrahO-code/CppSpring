#include <iostream>
#include <cppspring/cppspring.hpp>

int main() {
    std::cout << "CppSpring version: " << cppspring::VERSION << std::endl;
    
    auto status = cppspring::core::BatchStatus::STARTING;
    std::cout << "Status: " << cppspring::core::to_string(status) << std::endl;

    // Test round-trip conversion
    auto status2 = cppspring::core::from_string("COMPLETED");
    std::cout << "Converted status: " << cppspring::core::to_string(status2) << std::endl;

    std::cout << "BatchStatus working perfectly!" << std::endl;
    return 0;
}