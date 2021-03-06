// Copyright 2015 Marco Fuellemann & Janosch Rohdewald.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "pointviewer.h"

void PointViewer::update(vector<shared_ptr<ObjectPoint>> &object_points, vector<shared_ptr<ImageCamera>> &cameras) {
  visible_object_points = object_points;
  for (auto object_point : visible_object_points)
    object_point->recalculateColor();

  this->cameras = cameras;
  updateGL();
}

void PointViewer::draw() {
  glDisable(GL_LIGHTING);

  // Draw the points
  if (visible_object_points.size() > 0) {
    glPointSize(3.0);
    glBegin(GL_POINTS);

    for (auto object_point : visible_object_points) {
      glColor3f(object_point->color().x, object_point->color().y, object_point->color().z);
      glVertex3d(object_point->gl_coordinates()->x, object_point->gl_coordinates()->y,
                 object_point->gl_coordinates()->z);
    }
    glEnd();
  }

  // draw the camera frames
  for (auto camera : cameras) {
    GLdouble mat_rot_trans[16];

    // Get rotation of camera
    Mat_<double> position_rotation = camera->gl_rotation();
    for (int col = 0; col < 3; col++) {
      for (int row = 0; row < 3; row++)
        mat_rot_trans[col * 4 + row] = position_rotation(row, col);
      mat_rot_trans[col * 4 + 3] = 0;
    }

    // Get translation of camera
    Mat_<double> position_translation = camera->gl_translation();
    mat_rot_trans[12] = position_translation(0);
    mat_rot_trans[13] = position_translation(1);
    mat_rot_trans[14] = position_translation(2);
    mat_rot_trans[15] = 1;

    // draw camera
    glPushMatrix();
    glMultMatrixd(mat_rot_trans);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(camera_size, -camera_size, 2*camera_size);
    glVertex3f(-camera_size, -camera_size, 2*camera_size);
    glVertex3f(-camera_size, camera_size, 2*camera_size);
    glVertex3f(camera_size, camera_size, 2*camera_size);
    glVertex3f(camera_size, -camera_size, 2*camera_size);
    glEnd();

    // draw line out of camera
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, -.5*camera_size);
    glVertex3f(0, 0, camera_size*5);
    glEnd();

    glPopMatrix();
  }
}

// Constructor must call the base class constructor.
PointViewer::PointViewer(QWidget *parent) : QGLViewer(parent), sfmapp(SFMApp::getInstance()) {
  qglviewer::Vec camera_position(0, 0, 40);
  camera()->setPosition(camera_position);
  camera()->lookAt(sceneCenter());
  setSceneBoundingBox(qglviewer::Vec(-50, -50, -50), qglviewer::Vec(50, 50, 50));
  showEntireScene();
}

QString PointViewer::helpString() const {
  QString text("<h2>S i m p l e V i e w e r</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}

void PointViewer::setObjectCenterPivot() {
  float sumX = 0;
  float sumY = 0;
  float sumZ = 0;
  unsigned long numPoints = visible_object_points.size();
  for (auto object_point : visible_object_points) {
    sumX += object_point->gl_coordinates()->x;
    sumY += object_point->gl_coordinates()->y;
    sumZ += object_point->gl_coordinates()->z;
  }
  if (numPoints > 0) {
    camera()->setPivotPoint(qglviewer::Vec(sumX / numPoints, sumY / numPoints, sumZ / numPoints));
  }
}


