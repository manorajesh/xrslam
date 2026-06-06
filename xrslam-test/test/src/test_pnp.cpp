#include <gtest/gtest.h>
#include <xrslam/common.h>
#include <xrslam/geometry/pnp.h>

using namespace xrslam;

namespace {

std::array<vector<3>, 6> make_points() {
    return {vector<3>(-0.8, -0.3, 4.0), vector<3>(0.7, -0.2, 4.5),
            vector<3>(-0.4, 0.6, 5.0),  vector<3>(0.9, 0.5, 5.4),
            vector<3>(0.1, -0.8, 4.8),  vector<3>(-0.9, 0.7, 5.8)};
}

std::array<vector<2>, 6> project(const std::array<vector<3>, 6> &points,
                                 const matrix<3> &R, const vector<3> &t) {
    std::array<vector<2>, 6> image_points;
    for (size_t i = 0; i < points.size(); ++i) {
        image_points[i] = (R * points[i] + t).hnormalized();
    }
    return image_points;
}

} // namespace

TEST(test_pnp, fixed_rotation_translation_recovers_exact_pose) {
    const matrix<3> R =
        (Eigen::AngleAxisd(0.15, Eigen::Vector3d::UnitX()) *
         Eigen::AngleAxisd(-0.08, Eigen::Vector3d::UnitY()) *
         Eigen::AngleAxisd(0.05, Eigen::Vector3d::UnitZ()))
            .toRotationMatrix();
    const vector<3> expected_t(0.25, -0.12, 0.4);
    const auto points = make_points();
    const auto image_points = project(points, R, expected_t);

    vector<3> actual_t;
    ASSERT_TRUE(solve_translation_fixed_rotation(points, image_points, R,
                                                 actual_t));
    EXPECT_LT((actual_t - expected_t).norm(), 1.0e-9);
}

TEST(test_pnp, fixed_rotation_translation_handles_small_image_noise) {
    const matrix<3> R =
        (Eigen::AngleAxisd(-0.04, Eigen::Vector3d::UnitX()) *
         Eigen::AngleAxisd(0.11, Eigen::Vector3d::UnitY()) *
         Eigen::AngleAxisd(-0.07, Eigen::Vector3d::UnitZ()))
            .toRotationMatrix();
    const vector<3> expected_t(-0.18, 0.09, 0.35);
    const auto points = make_points();
    auto image_points = project(points, R, expected_t);
    for (size_t i = 0; i < image_points.size(); ++i) {
        image_points[i][0] += (i % 2 == 0 ? 1.0 : -1.0) * 1.0e-4;
        image_points[i][1] += (i % 3 == 0 ? -1.0 : 1.0) * 1.0e-4;
    }

    vector<3> actual_t;
    ASSERT_TRUE(solve_translation_fixed_rotation(points, image_points, R,
                                                 actual_t));
    EXPECT_LT((actual_t - expected_t).norm(), 5.0e-3);
}
