#ifndef RAMULATOR_WRAPPER_H
#define RAMULATOR_WRAPPER_H

#include <string>
#include "base/config.h"
#include "frontend/frontend.h"
#include "memory_system/memory_system.h"

class RamulatorWrapper {
public:
    RamulatorWrapper(const std::string& config_path);
    ~RamulatorWrapper();

    // Save memory system state to file
    void saveData(const std::string& filename);

    // Send a memory request to Ramulator
    void sendRequest(uint64_t address, bool is_read);

    // Tick front and backend
    void tick();

private:
    std::string config_path;
    Ramulator::IFrontEnd* frontend;
    Ramulator::IMemorySystem* memory_system;
};

#endif
