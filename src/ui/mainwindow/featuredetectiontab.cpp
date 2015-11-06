#include <src/image/image.h>
#include <sfmapp.h>
#include "featuredetectiontab.h"
#include "ui_featuredetectiontab.h"

FeatureDetectionTab::FeatureDetectionTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FeatureDetectionTab),
    sfmapp(SFMApp::getInstance()) {

  ui->setupUi(this);
}

FeatureDetectionTab::~FeatureDetectionTab() {
  delete ui;
}

void FeatureDetectionTab::updateImages() {
  ui->imagesListWidget->clear();
  for (auto &image : sfmapp->images)
    ui->imagesListWidget->addItem(QString(image->get_file_name().c_str()));
}

void FeatureDetectionTab::on_detectFeatures_clicked() {
  sfmapp->detectFeatures();
  currentImageIndex = 0;
  ui->imagesListWidget->setCurrentRow(currentImageIndex);
  updateImage();
}

void FeatureDetectionTab::updateImage() {

  if (currentImageIndex >= 0 && sfmapp->images.size() > 0) {
    shared_ptr<Image> image = sfmapp->images.at(currentImageIndex);

    if (image->get_keypoints()->size() > 0) {
      Mat picture;
      drawKeypoints(*image->get_mat_color(), *image->get_keypoints(), picture);
      ui->cvImageWidget->showImage(picture);
    } else
      ui->cvImageWidget->showImage(*image->get_mat_color());
  }
}

void FeatureDetectionTab::on_imagesListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
  currentImageIndex = ui->imagesListWidget->row(current);
  updateImage();
}
