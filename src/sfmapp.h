//
// Created by joschi on 19.10.15.
//

#ifndef SFM_SFMAPP_H
#define SFM_SFMAPP_H

#include "image/image.h"
#include <memory>
#include <cameraMatrixFinder/cameramatrixfinder.h>
#include <cameraMatrixFinder/RANSACCameraMatrixFinder.h>
#include "image/pair/imagepair.h"
#include "featureDetector/featuredetector.h"
#include "featureMatcher/featurematcher.h"
#include "featureDetector/orbfeaturedetector.h"
#include "featureMatcher/flannfeaturematcher.h"

using namespace std;

class SFMApp {
private:
  static SFMApp* instance;

  FeatureDetecter*feature_detector = new ORBFeatureDetector();
  FeatureMatcher*feature_matcher = new FlannFeatureMatcher();
  CameraMatrixFinder *cameraMatrixFinder = new RANSACCameraMatrixFinder();

  shared_ptr<ImagePair> initial_image_pair; // starting image pair

  Mat intrinsic_camera_parameters;

  SFMApp() {};

  SFMApp ( const SFMApp& ){};

  ~SFMApp () { }

public:
  static SFMApp* getInstance();

  vector<shared_ptr<Image>> images;

  vector<shared_ptr<ImagePair>> image_pairs;

  void detectFeatures();

  void matchFeatures();

  void findInitialMatrices(shared_ptr<ImagePair> &initial_image_pair, Mat& intristic_camera_paramaters);

  void loadImages(string const &images_dir);

  void unloadImages();

  void set_intrinsic_camera_parameters(Mat &intrinsic_camera_parameters){
    this->intrinsic_camera_parameters = intrinsic_camera_parameters;
  }
};



#endif //SFM_SFMAPP_H
