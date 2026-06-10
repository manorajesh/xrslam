#ifndef XRSLAM_CERES_DEPTH_FACTOR_H
#define XRSLAM_CERES_DEPTH_FACTOR_H

#include <ceres/ceres.h>
#include <cmath>
#include <xrslam/common.h>
#include <xrslam/estimation/depth_factor.h>
#include <xrslam/map/track.h>

namespace xrslam {

// 1-D residual r = sqrt(w) * (inv_depth - measured_inv_depth) on a single landmark's
// inverse depth, softly anchoring it (and thus metric scale) to the LiDAR measurement.
class CeresDepthPriorFactor : public DepthPriorFactor,
                              public ceres::SizedCostFunction<1, 1> {
  public:
    CeresDepthPriorFactor(Track *track, double weight)
        : track(track), measured_inv_depth(track->measured_inv_depth),
          sqrt_weight(std::sqrt(weight > 0.0 ? weight : 0.0)) {}

    bool Evaluate(const double *const *parameters, double *residuals,
                  double **jacobians) const override {
        const double inv_depth = parameters[0][0];
        residuals[0] = sqrt_weight * (inv_depth - measured_inv_depth);
        if (jacobians && jacobians[0]) {
            jacobians[0][0] = sqrt_weight;
        }
        return true;
    }

    Track *track;

  private:
    double measured_inv_depth;
    double sqrt_weight;
};

} // namespace xrslam

#endif // XRSLAM_CERES_DEPTH_FACTOR_H
