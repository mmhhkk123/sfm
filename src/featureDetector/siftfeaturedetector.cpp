//
// Created by joschi on 07.11.15.
//

#include "siftfeaturedetector.h"



SIFTFeatureDetector::SIFTFeatureDetector() : sift(SIFT::create(FeatureDetecter::max_features,3,0.04,10)) {
}

void SIFTFeatureDetector::detectFeatures(vector<shared_ptr<Image>> &images, bool useProvidedKeypoints) {
  for (auto image: images) {
    sift->detectAndCompute(*image->mat_grey(), noArray(), *image->get_keypoints(), *image->get_descriptors(), useProvidedKeypoints);
  }
}
