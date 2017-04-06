//! \file Viewer.h

#ifndef VIEWER_H
#define VIEWER_H

#include <CGAL/Three/Viewer_config.h>
#include <CGAL/Three/Scene_interface.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <CGAL/Three/Viewer_interface.h>

#include <QGLViewer/qglviewer.h>
#include <QPoint>
#include <QFont>
#include <QOpenGLFramebufferObject>
#include <CGAL/Three/TextRenderer.h>
// forward declarations
class QWidget;
namespace CGAL{
namespace Three{
class Scene_draw_interface;
}
}
class QMouseEvent;
class QKeyEvent;
class QContextMenuEvent;

class Viewer_impl;
//! The viewer class. Deals with all the openGL rendering and the mouse/keyboard events.
//! It should not be needed in the plugin.
class VIEWER_EXPORT Viewer : public CGAL::Three::Viewer_interface {

  Q_OBJECT

public:
  Viewer(QWidget * parent, bool antialiasing = false);
  ~Viewer();
  bool testDisplayId(double, double, double)Q_DECL_OVERRIDE;
  void updateIds(CGAL::Three::Scene_item *)Q_DECL_OVERRIDE;
  //! overload several QGLViewer virtual functions
  //! Draws the scene.
  void draw();
  //!This step happens after draw(). It is here that all the useful information is displayed, like the axis system or the informative text.
  void drawVisualHints();
  //! Deprecated. Does the same as draw().
  void fastDraw();
  bool isExtensionFound();
  //! Initializes the OpenGL functions and sets the backGround color.
  void initializeGL();
  //! Draws the scene "with names" to allow picking.
  void drawWithNames();
  /*! Uses the parameter pixel's coordinates to get the corresponding point
   * in the World frame. If this point is found, emits selectedPoint, selected,
   * and selectionRay signals.
   */
  void postSelection(const QPoint&);
  //! Sets the picking matrix to allow the picking.
  void beginSelection(const QPoint &point);
  //! Sets the pick matrix to Identity once the picking is done.
  void endSelection(const QPoint &point);
  //! Sets the scene for the viewer.
  void setScene(CGAL::Three::Scene_draw_interface* scene);
  //! @returns the antialiasing state.
  bool antiAliasing() const;
  //! @returns the fastDrawing state.
  bool inFastDrawing() const;
  //! Implementation of `Viewer_interface::inDrawWithNames()`
  bool inDrawWithNames() const;
  //! Implementation of `Viewer_interface::attribBuffers()`
  void attribBuffers(int program_name) const;
  //! Implementation of `Viewer_interface::getShaderProgram()`
  QOpenGLShaderProgram* getShaderProgram(int name) const Q_DECL_OVERRIDE;
  //!Declares a program names `name`, using `v_shader` as vertex shader and `f_shader` as fragment shader.
  QOpenGLShaderProgram* declare_program(int name,
                                        const char* v_shader,
                                        const char* f_shader)const;
  QPainter* getPainter()Q_DECL_OVERRIDE;
  void saveSnapshot(bool , bool overwrite = false);
  void setOffset(qglviewer::Vec offset);
  qglviewer::Vec offset()const Q_DECL_OVERRIDE;
  void setSceneBoundingBox(const qglviewer::Vec &min, const qglviewer::Vec &max);

  TextRenderer* textRenderer() Q_DECL_OVERRIDE;

Q_SIGNALS:
  void sendMessage(QString);
public Q_SLOTS:
  //! Sets the antialiasing to true or false.
  void setAntiAliasing(bool b) Q_DECL_OVERRIDE;
  //! If b is true, facets will be ligted from both internal and external sides.
  //! If b is false, only the side that is exposed to the light source will be lighted.
  void setTwoSides(bool b) Q_DECL_OVERRIDE;
  void SetOrthoProjection( bool b);
  //! If b is true, some items are displayed in a simplified version when moving the camera.
  //! If b is false, items display is never altered, even when moving.
  void setFastDrawing(bool b) Q_DECL_OVERRIDE;
  //! Makes the camera turn around.
  void turnCameraBy180Degres() Q_DECL_OVERRIDE;
  //! @returns a QString containing the position and orientation of the camera.
  QString dumpCameraCoordinates() Q_DECL_OVERRIDE;
  //!Moves the camera to the new coordinates (position and orientation) through an animation.
  bool moveCameraToCoordinates(QString, 
                               float animation_duration = 0.5f) Q_DECL_OVERRIDE;
  //!Makes the Viewer display a message
  void printMessage(QString message, int ms_delay );
  void displayMessage(const QString &_message, int delay);
  void displayMessage(const QString &_message){displayMessage(_message, 2000);}
  void hideMessage();
  void setBindingSelect() Q_DECL_OVERRIDE
  {
#if QGLVIEWER_VERSION >= 0x020501
    setMouseBinding(::Qt::ShiftModifier, ::Qt::LeftButton, SELECT);
#else
    setMouseBinding(::Qt::SHIFT + ::Qt::LeftButton, SELECT);
#endif
  }

  virtual void setNoBinding() Q_DECL_OVERRIDE
  {
#if QGLVIEWER_VERSION >= 0x020501
    setMouseBinding(::Qt::ShiftModifier, ::Qt::LeftButton, NO_CLICK_ACTION);
#else
    setMouseBinding(::Qt::SHIFT + ::Qt::LeftButton, NO_CLICK_ACTION);
#endif
  }

protected:
  void postDraw();
  void paintEvent(QPaintEvent *);
  void paintGL();

  //!Defines the behaviour for the mouse press events
  void mousePressEvent(QMouseEvent*);
  void wheelEvent(QWheelEvent *);
  //!Defines the behaviour for the key press events
  void keyPressEvent(QKeyEvent*);
  //!Deal with context menu events
  void contextMenuEvent(QContextMenuEvent*);
  //!Defines the behaviour for the key release events
  void keyReleaseEvent(QKeyEvent *);


  void resizeGL(int w, int h);

protected:
  friend class Viewer_impl;
  Viewer_impl* d;
  double prev_radius;

}; // end class Viewer


#endif // VIEWER_H
