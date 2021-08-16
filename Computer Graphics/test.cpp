#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <math.h>
#define PI M_PI
using namespace std;
using namespace Eigen;
int main(){
    Vector4d p(1,0,0,0);
    Isometry3d T=Isometry3d::Identity();
    AngleAxisd R(PI,Vector3d(0,0,1));
    T.rotate(R);
    T.pretranslate(Vector3d(0,0,0));
    cout<<T.matrix()*p<<endl;
    cout<<p<<endl;
    cout<<T.matrix();
   

}