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
   - An enhancement of the basic methods, incorporating a shift to accelerate convergence and find eigenvalues that are not in the extreme of the spectrum.

4. **QR Method**:
   - An algorithm to compute all eigenvalues of a matrix by decomposing it into a product of an orthogonal matrix `Q` and an upper triangular matrix `R`.


### Method Capabilities

| Method                        | # Eigenvalues | # Eigenvectors | Real Matrix Support | Complex Matrix Support | Example Usage                            |
| ----------------------------- | ------------- | --------------- | ------------------- | ----------------------- | ----------------------------------------- |
| Power Method                  | 1             | 1               | ✅                  | ✅                      | [Example 1](#1-find-the-largest-eigenvalue-and-its-associate-eigenvector-of-a-matrix)              |
| Inverse Power Method          | 1             | 1               | ✅                  | ✅                      | [Example 1](#1-find-the-largest-eigenvalue-and-its-associate-eigenvector-of-a-matrix)      |
| Shifted Power Method          | Multiple*      | Multiple*        | ✅                  | ✅                      | [Example 3](#3-shifting-to-get-other-eigenvectors)      |
| Shifted Inverse Power Method  | Multiple*      | Multiple*        | ✅                  | ✅                      | [Example 3](#3-shifting-to-get-other-eigenvectors) |
| QR Method                     | All      | ❌        | ✅                  | ✅                      | [Example 2](#2-finding-all-eigenvalues-of-a-matrix)               |

### Available Options:

| Flag              | Description                                                   | Compulsory | Default Value   | Example Usage                     |
| ----------------- | ------------------------------------------------------------- | ---------- | --------------- | ---------------------------------- |
| `--solver`    | Eigenvalue solver method to use.               | Yes        | -               | `--solver=pm`                        |
| `--matrix`     | Name of the matrix file inside `data/matrix` in `mtx` format.         | Yes        | -               | `--matrix=can_24`              |
| `--type`    | Type of solver used. Only two valid types: `real` and `complex`.      | No         | `real`   | `--type=complex`            |
| `--tol`    | Tolerance of the solver for convergence check. | No         | `1e-6`           | `--tol=1e-8`                           |
| `--shift`     | Shift value for Shifted Methods.                | For Shifted Methods         | `0.0`           | `--shift=1.0`                           |
| `--maxIter`      | Number of maximum iterations            | No         | 10000               | `--maxIter=1000`                               |

### Program Workflow

1. **Input Data**:
   - Place the matrix for which you want to compute eigenvalues and eigenvectors inside the folder `data/matrix/`.
   - Currently, the program supports matrices in the `.mtx` format.

2. **Running the Solver**:
   - The solver is executed through the command line interface.
   - You need to specify both the matrix and the solver you want to use.
   - Refer to the examples section below for detailed usage instructions.

3. **Output Results**:
   - The results are stored in the `results/` folder.
   - For each input matrix named `mat`, three main result files are provided:
     - `mat_eigenvalues.txt`: Contains the eigenvalue(s) of the matrix `mat`.
     - `mat_eigenvectors.txt`: Contains the eigenvector(s) of the matrix `mat`.
     - `mat_spectrum.png`: Presents a plot of the spectrum of the eigenvalue(s) of the matrix `mat`.


### Examples

In this section, some usage examples are provided. All examples use demonstration matrices that can be found inside the `data/matrix/` folder.

#### 1. Find the largest eigenvalue and its associate eigenvector of a matrix.

To find the dominant eigenvalue (the one with the maximum absolute value) we would use the **Power Method**. So, this is the command we would run from the root folder:

```bash
build/Eigenvalue-PCSC --matrix=can_24 --solver=pm
```

This is the simplest command we can write. We are specifying the only 2 compulsory commands: the matrix and the solver. After running this, the expected output are the files with the larger eigenvalue and its correspondant eigenvalue.

Similarly, we can find the smallest eigenvalue (in absolute value) and its associated vector. In this casae, we have to use **Inverser Power Method**.

```bash
build/Eigenvalue-PCSC --matrix=can_24 --solver=im
```


Once generated, you should be able to find them in the [results](results/) folder.


#### 2. Finding all eigenvalues of a matrix

The only method in our solver that allows you to obtain all eigenvalues at once is the **QR method**. In this section we present an example of how to use it for both real and complex matrices. However, this method will not provide the eigenvectors.

##### 2.1 Real matrix

Now let's focus on `rand_int` matrix. This is a 20 x 20 matrix that has been generated with random integers. Let's find its eigenvalues:

```bash
build/Eigenvalue-PCSC --matrix=rand_int --solver=qr --maxIter=1000 --tol=1e-5
```

Note that in this case we added two optional arguments. We will get the plots of the eigenvalues in the complex plane and their spectrum.

<table>
  <tr>
    <td>
      <img src="results/rand_int_eigenvalues.png" alt="Image 1" style="width: 100%;" />
      <p align="center">
        <sup>Figure 1. Complex plane</sup>
      </p>
    </td>
    <td>
      <img src="results/rand_int_spectrum.png" alt="Image 2" style="width: 100%;" />
      <p align="center">
        <sup>Figure 2. Spectrum</sup>
      </p>
    </td>
  </tr>
</table>

Again, check the [results](results/) folder to get your solution.

##### 2.2 Complex matrix

For the complex case, we have to use an input complex matrix. Let's take `rand_complex` as example. 

**Important**: The solver type must be fixed to `complex` or an error will be raised.

```bash
build/Eigenvalue-PCSC --matrix=rand_complex --solver=qr --type=complex --maxIter=1000 --tol=1e-5
```

This is the result to obtain:

<table>
  <tr>
    <td>
      <img src="results/rand_complex_eigenvalues.png" alt="Image 1" style="width: 100%;" />
      <p align="center">
        <sup>Figure 3. Complex plane</sup>
      </p>
    </td>
    <td>
      <img src="results/rand_complex_spectrum.png" alt="Image 2" style="width: 100%;" />
      <p align="center">
        <sup>Figure 4. Spectrum</sup>
      </p>
    </td>
  </tr>
</table>

#### 3. Shifting to get other eigenvectors

Now, let's focus on a case were we want to obtain all the eigenvectors. Since we can't use QR method to obtain them. We need to get them one by one with the Shifted method (we are aware it's not ideal, but at least they can be obtained). Let's study the matrix `dum3`. This matrix is as follows:

$$ dum3 = 
\begin{bmatrix}
0 & 11 & -5 \\
-2 & 17 & -7 \\
-4 & 2 & -10.1 \\
\end{bmatrix}
$$

The eigenvalues and eigenvector are as follows:

$$
\begin{align*}
\lambda_1 &\approx \text{4.4082} & \quad v_1 &\approx \begin{bmatrix} 0.3352 \\ -0.4978 \\ -0.7998 \end{bmatrix} \\
\lambda_2 &\approx \text{1.7322} & \quad v_2 &\approx \begin{bmatrix} -0.1329 \\ -0.4274 \\ -0.8942 \end{bmatrix} \\
\lambda_3 &\approx \text{0.7596} & \quad v_3 &\approx \begin{bmatrix} 0.3643 \\ 0.4060 \\ 0.8380 \end{bmatrix}
\end{align*}
$$

So, let's find the three eigenvalues and the corresponding eigenvectors. We can use the **Power Metho**d** to obtain the largest one.

```bash
build/Eigenvalue-PCSC --matrix=dum3 --solver=pm
```

Similarly, we can find the smallest by using the **Inverse Power Method**.

```bash
build/Eigenvalue-PCSC --matrix=dum3 --solver=pm
```

Finally, we can use any of the shifted methods to obtain the non-extreme eigenvalues. The shift must be close to the value of the target eigenvalue (closer than any other). In this case, we know the analytical value of `λ2` so we can use it just like that. We could use **QR method** to obtain this value. Let's use `shift=2.0`. So, the following command using **Inverse Power Method with Shift** (we could also use **Power Method with Shift**), will give us the second eigenvalue and its corresponding eigenvector.

```bash
build/Eigenvalue-PCSC --matrix=dum3 --solver=ims --shift=2.0
```

That's it! Now we have all the eigenvalues and eigenvectors of `dum3`.


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
1. CMake (version 3.10 or higher)
2. A C++ compiler compatible with CMake (GCC, Clang, MSVC, etc.)
3. Gnuplot (version 3.2 or higher)

This project also makes use of depencies included as submodules. No need to manually install them.

1. Eigen: A C++ template library for linear algebra.
2. Google Test: A C++ testing framework for unit testing.

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