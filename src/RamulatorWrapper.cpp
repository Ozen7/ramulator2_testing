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

void RamulatorWrapper::sendRequest(uint64_t address, bool is_read, std::function<void(Ramulator::Request&)> callback ) {
    bool success = frontend->receive_external_requests(
        0, address, 0, callback
    );

    if (success) {
        std::cout << "Request successfully sent to Ramulator!" << std::endl;
    } else {
        std::cout << "Request rejected (queue full)!" << std::endl;
    }
}

// C-style function pointer for Python interop
extern "C" {
    typedef void (*CRequestCallback)(Ramulator::Request*);

    RamulatorWrapper* RamulatorWrapper_new(const char* path) {
        return new RamulatorWrapper(path);
    }

    void RamulatorWrapper_tick(RamulatorWrapper* wrapper) {
        wrapper->tick();
    }

    void RamulatorWrapper_saveData(RamulatorWrapper* wrapper, const char* filename) {
        wrapper->saveData(filename);
    }

    // Wrapper function that converts the C-style callback to std::function
    void RamulatorWrapper_sendRequest(RamulatorWrapper* wrapper, uint64_t address, bool is_read, CRequestCallback c_callback) {
        std::function<void(Ramulator::Request&)> callback = [c_callback](Ramulator::Request& req) {
            c_callback(&req); // Call the C-style callback
        };
        wrapper->sendRequest(address, is_read, callback);
    }

    void RamulatorWrapper_delete(RamulatorWrapper* wrapper) {
        delete wrapper;
    }
}