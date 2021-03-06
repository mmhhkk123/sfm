// Copyright 2015 Marco Fuellemann & Janosch Rohdewald.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef SFM_IMAGEPAIR_H
#define SFM_IMAGEPAIR_H

#include <memory>
#include <vector>
#include "opencv2/core/cvdef.h"
#include "opencv2/core/core.hpp"
#include <map>
#include "image/image.h"

using namespace cv;
using namespace std;

/*
 * The ImagePair struct contains all variables/matrices that are used to describe
 * the relation between two images.
 */
struct ImagePair {
public:
  // First image, which usually is the more right image of both
  shared_ptr<Image> image1;

  // Second image, which usually is the more left image of both
  shared_ptr<Image> image2;

  // List with all matches between the keypoints of the two images
  vector<DMatch> matches;

  // Rotation between the two image cameras.
  // The rotation relative to (0/0/0), is defined by the camera of an image
  Mat_<double> rotation;

  // Translation between the two image cameras.
  // The translation relative to (0/0/0), is defined by the camera of an image
  Mat_<double> translation;

  // The essential matrix that relates the corresponding points of the two images
  Mat_<double> essential;

  // Points used for triangulation of image1
  vector<Point2f> triangulation_points1;

  // Points used for triangulation of image2
  vector<Point2f> triangulation_points2;

  // For solving the PnP problem, one needs to know which object point ("in space") is related to
  // which keypoint in the image.
  // Thus the ObjectPoints for solving PnP are saved in this vector
  vector<Point3f> pnp_object_points;

  // and the points that refer to the image (image2) are saved in this vector
  vector<Point2f> pnp_image_points;

  /**
   * Get the matches/keypoints in as two vector of Point2f.
   *
   * output: points1    the matched keypoints in image1
   * output: points2    the matched keypoints in image2
   */
  void getMatches(vector<Point2f> &points1, vector<Point2f> &points2);

  /**
   * Clear all the matrices. Is used if one would like to restart the calculation
   * with the same images
   */
  void clearMatrices();
};


#endif //SFM_IMAGEPAIR_H
