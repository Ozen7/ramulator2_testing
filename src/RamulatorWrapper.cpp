#include "RamulatorWrapper.h"
#include <iostream>
#include <fstream>

RamulatorWrapper::RamulatorWrapper(const std::string& path) : config_path(path) {
    YAML::Node config = Ramulator::Config::parse_config_file(config_path, {});
    frontend = Ramulator::Factory::create_frontend(config);
    memory_system = Ramulator::Factory::create_memory_system(config);
    frontend->connect_memory_system(memory_system);
    memory_system->connect_frontend(frontend);
}

RamulatorWrapper::~RamulatorWrapper() {
    frontend->finalize();
    memory_system->finalize();
}

void RamulatorWrapper::tick() {
    // Call tick function on both the frontend and memory system
    frontend->tick();
    memory_system->tick();
}

void RamulatorWrapper::saveData(const std::string& filename) {
    std::ofstream outfile(filename);
    // Save custom data about memory system
    outfile << "Saving data from memory system\n";
    outfile.close();
}

void RamulatorWrapper::sendRequest(uint64_t address, bool is_read) {
    bool success = frontend->receive_external_requests(
        0, address, 0, [this](Ramulator::Request& req) {
            std::cout << "Request completed for address: " << req.addr << " with command: " << req.type_id << std::endl;
        }
    );

    if (success) {
        std::cout << "Request successfully sent to Ramulator!" << std::endl;
    } else {
        std::cout << "Request rejected (queue full)!" << std::endl;
    }
}
