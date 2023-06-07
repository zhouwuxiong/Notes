#include <iostream>
#include <math.h>
#include <Eigen/Dense>
using namespace std;

Eigen::MatrixXd CoordinateTransform(Eigen::Vector3d globalPos, double heading, Eigen::MatrixXd localPos)
{
    cout << localPos << endl;

    double rotation = -(heading * M_PI / 180);

    Eigen::MatrixXd R(3, 3);
    R << cos(rotation), 0, sin(rotation),
        0, 0, 0,
        -sin(rotation), 0, cos(rotation);
    cout << (R * localPos) << endl;
    cout << globalPos << endl;
    return (R * localPos).colwise() + globalPos;
}

int main()
{
    Eigen::Vector3d globalPos{1, 2, 3};
    double heading{180};

    Eigen::MatrixXd localPos(4, 3);
    localPos << 1, 0, 1,
        1, 0, 2,
        2, 0, 2,
        2, 0, 1;
    Eigen::MatrixXd localPos2(3, 4);

    localPos2 = localPos.transpose();
    Eigen::MatrixXd res = CoordinateTransform(globalPos, heading, localPos2);

    cout << res << endl;
    return 1;
}