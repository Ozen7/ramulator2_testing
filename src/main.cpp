#include "../include/RamulatorWrapper.h"
#include <iostream>

int main() {
    // Path to the Ramulator configuration file
    std::string config_path = "/home/ozen/test/src/example_config.yaml";

    try {
        // Instantiate the Ramulator wrapper
        RamulatorWrapper ramulator(config_path);

        // Send multiple read requests in a loop
        for (uint64_t address = 0x1000; address < 0x2000; address += 0x100) {
            ramulator.sendRequest(address, true); // Read requests
        }

        // Send a write request
        ramulator.sendRequest(0x2000, false); // Write request to address 0x2000

        // Save the current state/data
        ramulator.saveData("memory_state.txt");

        for(uint64_t i = 0; i < 1000; i+=1){
            ramulator.tick();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}
