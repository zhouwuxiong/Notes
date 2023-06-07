#include <iostream>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Core>
using namespace std;
using namespace Eigen;

/**
 * @brief convert position from car Coordinate to world Coordinate
 * @param globalPos[in] car position in world Coordinate
 * @param heading[in] car's orientations reference to Z
 * @param localPos[in] the points in car Coordinate ,the shape is 3*N
 * @return the points in world Coordinate
 */
Eigen::MatrixXd CoordinateTransform(Eigen::Vector3d globalPos, double heading, Eigen::MatrixXd localPos)
{
    cout << localPos << endl;
    // if (localPos.rows() != 3)
    // {
    //     cout << " Illeage input localPos,expect shape 3*N" << endl;
    //     return Eigen::MatrixXd::Zero();
    // }

    double rotation = -(heading * M_PI / 180);

    Eigen::MatrixXd R(3, 3);
    R << cos(rotation), 0, sin(rotation),
        0, 0, 0,
        -sin(rotation), 0, cos(rotation);
    cout << (R * localPos) << endl;
    cout << globalPos << endl;
    return (R * localPos).colwise() + globalPos;
}

void test()
{
    Eigen::Vector3d globalPos{1, 2, 3};
    double heading{180};

    Eigen::MatrixXd localPos(4, 3);
    localPos << 1, 0, 1,
        1, 0, 2,
        2, 0, 2,
        2, 0, 1;

    localPos.transposeInPlace();

    Eigen::MatrixXd res = CoordinateTransform(globalPos, heading, localPos);

    cout << res << endl;
}

using Eigen::MatrixXd;
int main()
{
    test();

    MatrixXd A = MatrixXd::Random(100, 100);
    MatrixXd b = MatrixXd::Random(100, 50);
    MatrixXd x = A.fullPivLu().solve(b);
    double relative_error = (A * x - b).norm() / b.norm(); // norm() is L2 norm
    std::cout << "The relative error is:\n"
              << relative_error << std::endl;

    return 1;
}