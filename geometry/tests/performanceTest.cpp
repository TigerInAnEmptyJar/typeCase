#include "geometry.h"
#include "geometry_new.h"
#include <iostream>

#include <gtest/gtest.h>

#include <chrono>
#include <vector>

TEST(PerformanceTests, CreationTest)
{
  const size_t amount = 10000;

  // Create a big amount of 2D vectors as arrays
  {
    auto start = std::chrono::system_clock::now();
    vector2D oldVectors[amount];
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " old 2D-vectors in an array took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << oldVectors[0].getState() << std::endl;
  }

  // Create a big amount of new 2D vectors as arrays
  {
    auto start = std::chrono::system_clock::now();
    geo::Vector2D<double> newVectors[amount];
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " new 2D-vectors in an array took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << newVectors[0].isNull() << std::endl;
  }

  // Create a big amount of 3D vectors as arrays
  {
    auto start = std::chrono::system_clock::now();
    vector3D oldVectors[amount];
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " old 3D-vectors in an array took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << oldVectors[0].getState() << std::endl;
  }

  // Create a big amount of new 3D vectors as arrays
  {
    auto start = std::chrono::system_clock::now();
    geo::Vector3D<double> newVectors[amount];
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " new 3D-vectors in an array took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << newVectors[0].isNull() << std::endl;
  }

  // Create a big amount of 3D vectors in loop
  {
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      vector3D oldVector;
      oldVector.isLine();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " old 3D-vectors in a loop took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create a big amount of new 3D vectors in loop
  {
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      geo::Vector3D<double> newVector;
      newVector.isLine();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " new 3D-vectors in a loop took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create a big amount of 25D vectors in loop
  {
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      Vector oldVector(25);
      oldVector.isLine();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " old 25D-vectors in a loop took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create a big amount of new 25D vectors in loop
  {
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      geo::Vector<double, 25> newVector;
      newVector.isLine();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " new 25D-vectors in a loop took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create a big amount of 24x27 matrices in a loop
  {
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      matrixNxM oldMatrix(24, 27);
      oldMatrix.Lines();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " old 24x27-matrices in a loop took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create a big amount of new 24x27 matrices in a loop
  {
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      geo::Matrix<double, 24, 27> newMatrix;
      newMatrix.lines();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Creating " << amount << " new 24x27-matrices in a loop took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }
}

TEST(PerformanceTests, BasicCalculationsTest)
{
  const size_t amount = 5000;

  // Create and initialize (with random numbers) a big amount of 2D-vectors and do subtraction.
  {
    vector2D left[amount];
    vector2D right[amount];
    for (size_t i = 0; i < amount; i++) {
      left[i].setValues(static_cast<double>(rand()) / INT_FAST64_MAX,
                        static_cast<double>(rand()) / INT_FAST64_MAX);
      right[i].setValues(static_cast<double>(rand()) / INT_FAST64_MAX,
                         static_cast<double>(rand()) / INT_FAST64_MAX);
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      left[i] - right[i];
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Subtracting " << amount << " old 2D-vectors took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << std::endl;
  }

  // Create and initialize (with random numbers) a big amount of new 2D-vectors and do subtraction.
  {
    geo::Vector2D<double> left[amount];
    geo::Vector2D<double> right[amount];
    for (size_t i = 0; i < amount; i++) {
      left[i].setValues(static_cast<double>(rand()) / INT_FAST64_MAX,
                        static_cast<double>(rand()) / INT_FAST64_MAX);
      right[i].setValues(static_cast<double>(rand()) / INT_FAST64_MAX,
                         static_cast<double>(rand()) / INT_FAST64_MAX);
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      left[i] - right[i];
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Subtracting " << amount << " new 2D-vectors took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << std::endl;
  }

  // Create and initialize (with random numbers) a big amount of 3D-vectors and do length
  // calculation.
  {
    vector3D oldVectors[amount];
    for (size_t i = 0; i < amount; i++) {
      oldVectors[i].setValues(static_cast<double>(rand()) / INT_FAST64_MAX,
                              static_cast<double>(rand()) / INT_FAST64_MAX,
                              static_cast<double>(rand()) / INT_FAST64_MAX);
      EXPECT_EQ(_regular_, oldVectors[i].getState());
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      oldVectors[i].length();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Length calculation of " << amount << " old 3D-vectors took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << std::endl;
  }

  // Create and initialize (with random numbers) a big amount of new 3D-vectors and do length
  // calculation.
  {
    geo::Vector3D<double> newVectors[amount];
    for (size_t i = 0; i < amount; i++) {
      newVectors[i].setValues(static_cast<double>(rand()) / INT_FAST64_MAX,
                              static_cast<double>(rand()) / INT_FAST64_MAX,
                              static_cast<double>(rand()) / INT_FAST64_MAX);
      EXPECT_FALSE(newVectors[i].isNull());
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      newVectors[i].length();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Length calculation of " << amount << " new 3D-vectors took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs. " << std::endl;
  }
}

TEST(PerformanceTests, MatrixMultiplication)
{
  const size_t amount = 5000;

  // Create and initialize a big amount of 24x27 matrices and multiply them.
  {
    std::vector<matrixNxM> left;
    std::vector<matrixNxM> right;
    for (size_t i = 0; i < amount; i++) {
      left.push_back(matrixNxM(24, 27));
      right.push_back(matrixNxM(27, 24));
      for (auto j = 0; j < 24; j++) {
        for (auto k = 0; k < 27; k++) {
          left[i].setValue(j, k, static_cast<double>(rand()) / INT_FAST64_MAX);
          right[i].setValue(k, j, static_cast<double>(rand()) / INT_FAST64_MAX);
        }
      }
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      left[i] * right[i];
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Multiplying " << amount << " old 24x27-matrices took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create and initialize a big amount of new 24x27 matrices and multiply them.
  {
    std::vector<geo::Matrix<double, 24, 27>> left(amount);
    std::vector<geo::Matrix<double, 27, 24>> right(amount);
    for (size_t i = 0; i < amount; i++) {
      for (size_t j = 0; j < 24; j++) {
        for (size_t k = 0; k < 27; k++) {
          left[i].value(j, k) = static_cast<double>(rand()) / INT_FAST64_MAX;
          right[i].value(k, j) = static_cast<double>(rand()) / INT_FAST64_MAX;
        }
      }
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      left[i] * right[i];
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Multiplying " << amount << " new 24x27-matrices took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create and initialize a big amount of 24x27 matrices and multiply them with a vector.
  {
    std::vector<matrixNxM> left;
    std::vector<Vector> right;
    for (size_t i = 0; i < amount; i++) {
      left.push_back(matrixNxM(24, 27));
      right.push_back(Vector(27));
      right.back().transpose();
      for (auto j = 0; j < 24; j++) {
        for (auto k = 0; k < 27; k++) {
          left[i].setValue(j, k, static_cast<double>(rand()) / INT_FAST64_MAX);
          right[i].setValue(k, static_cast<double>(rand()) / INT_FAST64_MAX);
        }
      }
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      left[i] * right[i];
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Multiplying " << amount << " old 24x27-matrices to vectors took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }

  // Create and initialize a big amount of new 24x27 matrices and multiply them.
  {
    std::vector<geo::Matrix<double, 24, 27>> left(amount);
    std::vector<geo::Vector<double, 27>> right(amount);
    for (size_t i = 0; i < amount; i++) {
      for (size_t j = 0; j < 24; j++) {
        for (size_t k = 0; k < 27; k++) {
          left[i].value(j, k) = static_cast<double>(rand()) / INT_FAST64_MAX;
          right[i][k] = static_cast<double>(rand()) / INT_FAST64_MAX;
        }
      }
    }
    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < amount; i++) {
      left[i] * right[i];
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Multiplying " << amount << " new 24x27-matrices to vectors took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " µs." << std::endl;
  }
}
