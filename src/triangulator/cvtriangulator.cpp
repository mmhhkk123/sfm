//
// Created by marco on 18.11.15.
//

#include "cvtriangulator.h"
#include <opencv2/calib3d.hpp>
#include <iostream>
#include "math.h"

using namespace std;
using namespace cv;

void CVTriangulator::findPoints3D(shared_ptr<ImagePair> &image_pair, Mat &intrinsic_camera_parameters, map<int, Point3f> &map_points3D) {
  Mat mat_points3d;
  vector<Point3f> points3D;
  triangulatePoints(image_pair->projection_img1, image_pair->projection_img2,
                    image_pair->triangulation_points1, image_pair->triangulation_points2, mat_points3d);

  // Since it's homogenous (x, y, z, w) coord, divide by w to get (x, y, z, 1)
  mat_points3d.row(0) = (mat_points3d.row(0) / mat_points3d.row(3)) + 0;
  mat_points3d.row(1) = (mat_points3d.row(1) / mat_points3d.row(3)) + 0;
  mat_points3d.row(2) = (mat_points3d.row(2) / mat_points3d.row(3)) + 0;

  // convert mat_points3d into vector<Point3f> for projectPoints
  for (int i = 0; i < mat_points3d.cols; i++)
    points3D.push_back(
        Point3f(mat_points3d.at<float>(0, i), mat_points3d.at<float>(1, i), mat_points3d.at<float>(2, i)));

  // Reproject points to 2D, so we can afterwards calculate the reprojection error
  vector<Point2f> reprojected_points;
  projectPoints(points3D, image_pair->image2->camera()->extrinsic_rotation(),
                image_pair->image2->camera()->extrinsic_translation(), intrinsic_camera_parameters, noArray(),
                reprojected_points);

  // Clear points3D and only add points with acceptable reproj. error
  points3D.clear();
  for (int i = 0; i < mat_points3d.cols; i++) {
    float repojection_error = sqrt(pow(reprojected_points.at(i).x - image_pair->triangulation_points2.at(i).x, 2) +
                                   pow(reprojected_points.at(i).y - image_pair->triangulation_points2.at(i).y, 2));

    cout<<repojection_error<<endl;
    if (repojection_error < 1)
      map_points3D[i] = Point3f(mat_points3d.at<float>(0, i), mat_points3d.at<float>(1, i), mat_points3d.at<float>(2, i));
  }

}