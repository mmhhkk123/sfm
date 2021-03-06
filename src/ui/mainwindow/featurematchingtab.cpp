// Copyright 2015 Marco Fuellemann & Janosch Rohdewald.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "featurematchingtab.h"
#include "ui_featurematchingtab.h"

FeatureMatchingTab::FeatureMatchingTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FeatureMatchingTab),
    sfmapp(SFMApp::getInstance()) {
  ui->setupUi(this);
}

FeatureMatchingTab::~FeatureMatchingTab() {
  delete ui;
}

void FeatureMatchingTab::updateImagePairs() {
  ui->matchesListWidget->clear();
  for (auto &featureMatch : *sfmapp->image_pairs())
    ui->matchesListWidget->addItem(
        QString((featureMatch->image1->file_name() + " <-> " + featureMatch->image2->file_name()).c_str()));
  emit imagePairsUpdated();
}

void FeatureMatchingTab::updateImage() {
  if (currentMatchIndex >= 0 && sfmapp->images()->size() > 0) {
    shared_ptr<ImagePair> matches = sfmapp->image_pairs()->at(currentMatchIndex);
    shared_ptr<Image> image1 = matches->image1;
    shared_ptr<Image> image2 = matches->image2;

    if (matches->matches.size() > 0) {
      Mat picture;
      drawMatches(*image1->mat_grey(), *image1->keypoints(), *image2->mat_grey(),
                  *image2->keypoints(), matches->matches, picture);
      ui->cvImageWidget->showImage(picture);
    } else
      ui->cvImageWidget->showImage(*image1->mat_grey());
  }
}


void FeatureMatchingTab::on_matchFeatures_clicked() {
  sfmapp->matchFeatures();
  updateImagePairs();
  currentMatchIndex = 0;
  ui->matchesListWidget->setCurrentRow(currentMatchIndex);
  updateImage();
}

void FeatureMatchingTab::on_matchesListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
  currentMatchIndex = ui->matchesListWidget->row(current);
  updateImage();
}
