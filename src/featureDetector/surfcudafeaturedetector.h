// Copyright 2015 Marco Fuellemann & Janosch Rohdewald.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef SFM_SURFCUDAFEATUREDETECTOR_H
#define SFM_SURFCUDAFEATUREDETECTOR_H

#ifdef USE_CUDA
#include "featuredetector.h"
#include "opencv2/xfeatures2d/cuda.hpp"
#include "opencv2/calib3d.hpp"

using namespace cv::cuda;

/*
 * Implementation of the SURF feature detector - CUDA implementation.
 */
class SURFCUDAFeatureDetector : public FeatureDetecter {
private:
  SURF_CUDA surf_cuda;

public:
  SURFCUDAFeatureDetector();

  /**
   * @see FeatureDetecter
   */
  void detectFeatures(vector<shared_ptr<Image>> &images, bool use_provided_keypoints = false);

};

#endif // USE_CUDA

#endif //SFM_SURFCUDAFEATUREDETECTOR_H