# Eigenvalue Computation Project

## Introduction
This project is dedicated to the implementation of numerical methods for eigenvalue computation. It targets matrices `A` and focuses on finding all scalars `λ` (eigenvalues) that satisfy the equation:

$$ Ax = \lambda x $$

## Features
The project includes the following key features:

1. **Power Method**: 
   - This method is used to find the dominant eigenvalue of a matrix.

2. **Inverse Power Method**:
   - A technique for finding the smallest eigenvalue of a matrix.

3. **Power and Inverse Power Methods with Shift**:
   - An enhancement of the basic methods, incorporating a shift to accelerate convergence.

4. **QR Method**:
   - An algorithm to compute all eigenvalues of a matrix by decomposing it into a product of an orthogonal matrix `Q` and an upper triangular matrix `R`.

## Method Capabilities

## Method Capabilities

| Method                        | # Eigenvalues | # Eigenvectors | Real Matrix Support | Complex Matrix Support | Example Usage                            |
| ----------------------------- | ------------- | --------------- | ------------------- | ----------------------- | ----------------------------------------- |
| Power Method                  | 1             | 1               | ✅                  | ✅                      | [Example](#power-method)              |
| Inverse Power Method          | 1             | 1               | ✅                  | ✅                      | [Example](#inverse-power-method)      |
| Shifted Power Method          | Multiple*      | Multiple*        | ✅                  | ✅                      | [Example](#shifted-power-method)      |
| Shifted Inverse Power Method  | Multiple*      | Multiple*        | ✅                  | ✅                      | [Example](#shifted-inverse-power-method) |
| QR Method                     | All      | ❌        | ✅                  | ✅                      | [Example](#qr-method)                 |



## Cloning the repository

When cloning the repository to a new location, you should initialize and fetch the submodules:

```bash
git clone --recursive https://github.com/pauromeu/eigenvalue-pcsc.git
```

Or, if the repository is already cloned:

```bash
git submodule update --init --recursive
```

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
## Generating Documentation

To generate the project's documentation, follow these steps:

### Prerequisites
Ensure you have Doxygen installed:
- For installation instructions, visit [Doxygen's official website](http://www.doxygen.nl/download.html).

### Generating the Docs
1. **Navigate to the Documentation Directory**:
   If your Doxygen configuration file (`Doxyfile`) is located in a specific directory (e.g., `docs`), navigate to that directory:
   ```bash
   cd docs
   ```
2. **Run Doxygen**:
   Execute the following command to generate the documentation:
   ```bash
   doxygen Doxyfile
   ```

3. **Accessing the Documentation**:
   - After successful generation, the documentation will be available in the directory specified in the `OUTPUT_DIRECTORY` of your Doxyfile.
   - If no output directory is specified, it will be in the same directory as your Doxyfile.
   - Open the `index.html` file in a web browser to view the documentation.