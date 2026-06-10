#ifndef XRSLAM_DEPTH_FACTOR_H
#define XRSLAM_DEPTH_FACTOR_H

#include <xrslam/common.h>

namespace xrslam {

// Unary prior tying a landmark's inverse depth to a measured (LiDAR) value.
class DepthPriorFactor {
  public:
    virtual ~DepthPriorFactor() = default;

  protected:
    DepthPriorFactor() = default;
};

} // namespace xrslam

#endif // XRSLAM_DEPTH_FACTOR_H
