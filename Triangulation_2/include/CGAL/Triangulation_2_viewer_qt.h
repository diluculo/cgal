// Copyright (c) 2017 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>

#ifndef CGAL_T2_VIEWER_QT_H
#define CGAL_T2_VIEWER_QT_H

#ifdef CGAL_USE_BASIC_VIEWER

#include <CGAL/Qt/Basic_viewer_qt.h>
#include <CGAL/Random.h>

namespace CGAL
{
  
// Default color functor; user can change it to have its own face color
struct DefaultColorFunctorT2
{
  template<typename T2>
  static CGAL::Color run(const T2& at2,
                         const typename T2::Finite_faces_iterator fh)
  {
    // Here dh is the smaller dart of its face
    CGAL::Random random((unsigned long int)(&*fh));
    return get_random_color(random);
  }
};

// Viewer class for T2 
template<class T2, class ColorFunctor>
class SimpleTriangulation2ViewerQt : public Basic_viewer_qt
{
  typedef Basic_viewer_qt                     Base;
  typedef typename T2::Vertex_handle          Vertex_const_handle;
  typedef typename T2::Finite_edges_iterator Edge_const_handle;
  typedef typename T2::Finite_faces_iterator Facet_const_handle;
  typedef typename T2::Point                  Point;
 
public:
  /// Construct the viewer.
  /// @param at2 the t2 to view
  /// @param title the title of the window
  /// @param anofaces if true, do not draw faces (faces are not computed; this can be
  ///        usefull for very big object where this time could be long)
  SimpleTriangulation2ViewerQt(const T2& at2, const char* title="",
                               bool anofaces=false,
                               const ColorFunctor& fcolor=ColorFunctor()) :
    Base(title),
    t2(at2),
    m_nofaces(anofaces),
    m_fcolor(fcolor)
  {
    compute_elements();
  }

protected:
  void compute_face(Facet_const_handle fh)
  {
    CGAL::Color c=m_fcolor.run(t2, fh);
    face_begin(c);

    add_point_in_face(fh->vertex(0)->point());
    add_point_in_face(fh->vertex(1)->point());
    add_point_in_face(fh->vertex(2)->point());
    
    face_end();
  }

  void compute_edge(Edge_const_handle eh)
  {
    add_segment(eh->first->vertex(eh->first->cw(eh->second))->point(),
                eh->first->vertex(eh->first->ccw(eh->second))->point());
  }

  void compute_vertex(Vertex_const_handle vh)
  { add_point(vh->point()); }

  void compute_elements()
  {
    clear();

    for (typename T2::Finite_faces_iterator it=t2.finite_faces_begin();
         it!=t2.finite_faces_end(); ++it)
    { compute_face(it); } 

    for (typename T2::Finite_edges_iterator it=t2.finite_edges_begin();
         it!=t2.finite_edges_end(); ++it)
    { compute_edge(it); } 

    for (typename T2::Finite_vertices_iterator it=t2.finite_vertices_begin();
         it!=t2.finite_vertices_end(); ++it)
    { compute_vertex(it); } 
  }

  virtual void keyPressEvent(QKeyEvent *e)
  {
    // Test key pressed:
    //    const ::Qt::KeyboardModifiers modifiers = e->modifiers();
    //    if ((e->key()==Qt::Key_PageUp) && (modifiers==Qt::NoButton)) { ... }
    
    // Call: * compute_elements() if the model changed, followed by
    //       * redraw() if some viewing parameters changed that implies some
    //                  modifications of the buffers
    //                  (eg. type of normal, color/mono)
    //       * update() just to update the drawing

    // Call the base method to process others/classicals key
    Base::keyPressEvent(e);
  }

protected:
  const T2& t2;
  bool m_nofaces;
  const ColorFunctor& m_fcolor;
};
  
template<class T2, class ColorFunctor>
void draw(const T2& at2,
          const char* title="T2 Viewer",
          bool nofill=false,
          const ColorFunctor& fcolor=ColorFunctor())
{
  int argc=1;

  const char* argv[2]={"t2_viewer","\0"};
  QApplication app(argc,const_cast<char**>(argv));

  SimpleTriangulation2ViewerQt<T2, ColorFunctor> mainwindow(at2, title,
                                                            nofill, fcolor);

#if !defined(CGAL_TEST_SUITE)
  mainwindow.show();
  app.exec();
#endif
}

template<class T2>
void draw(const T2& at2,
          const char* title="T2 Viewer",
          bool nofill=false)
{ return display<T2, DefaultColorFunctorT2>(at2, title, nofill); }

} // End namespace CGAL

#else // CGAL_USE_BASIC_VIEWER

namespace CGAL 
{

template<class T2, class ColorFunctor>
void draw(const T2&,
          const char* ="T2 Viewer",
          bool=false,
          const ColorFunctor& =ColorFunctor())
{
  std::cerr<<"Impossible to draw a Triangulation_2 because CGAL_USE_BASIC_VIEWER is not defined."
           <<std::endl;
}

template<class T2>
void draw(const T2&,
          const char* ="T2 Viewer",
          bool=false)
{
  std::cerr<<"Impossible to draw a Triangulation_2 because CGAL_USE_BASIC_VIEWER is not defined."
           <<std::endl;
}
  
} // End namespace CGAL

#endif // CGAL_USE_BASIC_VIEWER

#endif // CGAL_T2_VIEWER_QT_H
