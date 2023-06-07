#include <Eigen/Dense>
#include <iostream>

int main() {
    // Create a 3x3 matrix
    Eigen::Matrix3d mat;
    mat << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    // Get the top-right 2x2 submatrix
    Eigen::Matrix2d submat = mat.topRightCorner<2, 2>();

    // Print the submatrix
    std::cout << submat << std::endl;

    return 0;
}
