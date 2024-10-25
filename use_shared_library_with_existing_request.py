import ctypes

# Define a Python structure for Ramulator::Request based on the existing C++ definition
class Request(ctypes.Structure):
    _fields_ = [("addr", ctypes.c_uint64),
                ("type_id", ctypes.c_char_p)]

# Load the shared library
lib = ctypes.CDLL("./build/libRamulatorWrapper.so")  # Change to .dll on Windows

# Define the function signatures
lib.RamulatorWrapper_new.argtypes = [ctypes.c_char_p]
lib.RamulatorWrapper_new.restype = ctypes.c_void_p

lib.RamulatorWrapper_tick.argtypes = [ctypes.c_void_p]
lib.RamulatorWrapper_saveData.argtypes = [ctypes.c_void_p, ctypes.c_char_p]

# Define the callback function type
CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(Request))
lib.RamulatorWrapper_sendRequest.argtypes = [ctypes.c_void_p, ctypes.c_uint64, ctypes.c_bool, CALLBACK]

lib.RamulatorWrapper_delete.argtypes = [ctypes.c_void_p]

# Define a Python callback function
def python_callback(req_ptr):
    req = req_ptr.contents  # Access the request structure
    print(f"Python callback called: Address={req.addr}, Command={req.type_id.decode()}")

# Convert Python callback to ctypes-compatible callback
callback = CALLBACK(python_callback)

# Create an instance of RamulatorWrapper
wrapper = lib.RamulatorWrapper_new(b"/home/ozen/test/src/example_config.yaml")

# class component1
    # matrix1 = x
    # value = matrix1[0][0] x * length(y) + y
    
    # valuecanbeused[0] = False
    # setValueCanBeUsedToTrue = 
    # Cycle() = 


# Send a request with the callback
lib.RamulatorWrapper_sendRequest(wrapper, 0x1000, True, callback)

# Call the tick method
for x in range(50):
    lib.RamulatorWrapper_tick(wrapper)


# Save data to a file
lib.RamulatorWrapper_saveData(wrapper, b"output.txt")

# Clean up and delete the wrapper
lib.RamulatorWrapper_delete(wrapper)
