#ifndef XRSLAM_QUATERNION_PARAMETERIZATION_H
#define XRSLAM_QUATERNION_PARAMETERIZATION_H

#include <ceres/ceres.h>
#include <xrslam/common.h>
#include <xrslam/geometry/lie_algebra.h>

namespace xrslam {

struct QuaternionParameterization : public ceres::Manifold {
    bool Plus(const double *q, const double *dq,
              double *q_plus_dq) const override {
        map<quaternion> result(q_plus_dq);
        result = (const_map<quaternion>(q) * expmap(const_map<vector<3>>(dq)))
                     .normalized();
        return true;
    }
    bool PlusJacobian(const double *, double *jacobian) const override {
        map<matrix<4, 3, true>> J(jacobian);
        J.setIdentity(); // the composited jacobian is computed in
                         // PreIntegrationError::Evaluate(), we simply forward
                         // it.
        return true;
    }
    bool Minus(const double *y, const double *x,
               double *y_minus_x) const override {
        map<vector<3>> result(y_minus_x);
        result = logmap(const_map<quaternion>(x).conjugate() *
                        const_map<quaternion>(y));
        return true;
    }
    bool MinusJacobian(const double *, double *jacobian) const override {
        map<matrix<3, 4, true>> J(jacobian);
        J.setZero();
        J.template block<3, 3>(0, 0).setIdentity();
        return true;
    }
    int AmbientSize() const override { return 4; }
    int TangentSize() const override { return 3; }
};

} // namespace xrslam

#endif // XRSLAM_QUATERNION_PARAMETERIZATION_H
