//
// Created by marco on 22.11.15.
//

#ifndef SFM_PNPSOLVER_H
#define SFM_PNPSOLVER_H

#include "image/pair/imagepair.h"

class PnPSolver {
public:
  virtual void solve(shared_ptr<ImagePair> &image_pair, Mat &intristic_camera_paramaters) = 0;
};

#endif //SFM_PNPSOLVER_H