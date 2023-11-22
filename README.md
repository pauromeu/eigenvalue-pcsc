# Eigenvalue problems solver

## Building the Project

### Prerequisites
Before building, ensure you have the following installed:
- CMake (version 3.10 or higher)
- A C++ compiler compatible with CMake (GCC, Clang, MSVC, etc.)

### Building Instructions


   Run the following command in the root directory of the project to create a build directory and generate the necessary build files:
   ```bash
   cmake -S . -B build
   ```
   Build the files in the build folder:
   ```bash
   cmake --build build
   ```

### Running the project

   After a successful build, you can find the executable in the build directory. Navigate there and run it:
   
   ```bash
   cd build
    ./Eigenvalue-PCSC
   ```

   In the same directory, run the executable generated to run the tets:
   ```bash
    ./Eigenvalue-PCSC-tests
   ```
