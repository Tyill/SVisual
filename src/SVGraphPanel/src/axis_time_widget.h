//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#pragma once

#include <QWidget>

class GraphWidget;

class AxisTimeAdapter: public QObject{
  Q_OBJECT
  public:
  using setTimeIntervalCBack = std::function<void(GraphWidget*, qint64, qint64)>;
  using getTimeIntervalCBack = std::function<QPair<qint64, qint64>(GraphWidget*)>;
  using getTimeScaleCBack = std::function<double(GraphWidget*)>;
  using getAxisMarkCBack = std::function<QVector<int>(GraphWidget*)>;
  using scaleCBack = std::function<void(GraphWidget*, int delta, int mpos)>;
  using mouseMoveEventCBack = std::function<void(GraphWidget*, QMouseEvent*)>;
  using mousePressEventCBack = std::function<void(GraphWidget*, QMouseEvent*)>;
  using wheelEventCBack = std::function<void(GraphWidget*, QWheelEvent*)>;
  using updateCBack = std::function<void(GraphWidget*)>;
  using widthCBack = std::function<int(GraphWidget*)>;

  AxisTimeAdapter(GraphWidget* graph, QWidget *parent = 0)
      :QObject(parent), graph_(graph){
  }
  void setTimeInterval(qint64 l, qint64 r){
    if (pfSetTimeIntervalCBack){
      pfSetTimeIntervalCBack(graph_, l, r);
    }
  };
  QPair<qint64, qint64> getTimeInterval(){
    return pfGetTimeIntervalCBack ? pfGetTimeIntervalCBack(graph_) : QPair<qint64, qint64>{};
  };
  double getTimeScale(){
    return pfGetTimeScaleCBack ? pfGetTimeScaleCBack(graph_) : 1.0;
  };
  QVector<int> getAxisMark(){
    return pfGetAxisMarkCBack ? pfGetAxisMarkCBack(graph_) : QVector<int>{};
  };
  void scale(int delta, int mpos){
    if (pfScaleCBack){
      pfScaleCBack(graph_, delta, mpos);
    }
  }
  int width(){
    return pfWidthCBack ? pfWidthCBack(graph_) : 1;
  }
  void mouseMoveEvent(QMouseEvent * event){
    if (pfMouseMoveEvent){
        pfMouseMoveEvent(graph_, event);
    }
  }
  void mousePressEvent(QMouseEvent * event){
    if (pfMousePressEvent){
      pfMousePressEvent(graph_, event);
    }
  }
  void wheelEvent(QWheelEvent * event){
    if (pfWheelEvent){
      pfWheelEvent(graph_, event);
    }
  }
  void update(){
    if (pfUpdate){
      pfUpdate(graph_);
    }
  }

  mouseMoveEventCBack pfMouseMoveEvent{};
  mousePressEventCBack pfMousePressEvent{};
  wheelEventCBack pfWheelEvent{};
  setTimeIntervalCBack pfSetTimeIntervalCBack{};
  getTimeIntervalCBack pfGetTimeIntervalCBack{};
  getTimeScaleCBack pfGetTimeScaleCBack{};
  getAxisMarkCBack pfGetAxisMarkCBack{};
  scaleCBack pfScaleCBack{};
  updateCBack pfUpdate{};
  widthCBack pfWidthCBack{};
  GraphWidget* graph_{};
};


class AxisTimeWidget : public QWidget
{
  Q_OBJECT
private:
    
  int cng_dashHeight_ = 3;
  
  int curDashStep_ = 100;
  int curOffsPos_ = 0;
  int fontMetr_ = 0;
  int curIntervSec_ = 0;

  QPair<qint64, qint64> tmInterval_;
    
  double tmScale_ = 1.0;

  int mousePrevPosX_ = 0;
  
  void resizeEvent(QResizeEvent * event)override;

  void drawDashLines(QPainter& painter);
  void drawTimeMark(QPainter& painter);

  QString getTimeMark(int offs);


public:
  AxisTimeWidget(QWidget *parent = 0);
  ~AxisTimeWidget();

  void setTimeInterval(qint64, qint64);

  QPair<qint64, qint64> getTimeInterval() const {
    return tmInterval_;
  }

  double getTimeScale() const {
    return tmScale_;
  }

  QVector<int> getAxisMark();

  void mouseMoveEvent(QMouseEvent * event)override;
  void mousePressEvent(QMouseEvent * event)override;
  void wheelEvent(QWheelEvent * event)override;
  void scale(int delta, int mpos);

protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
  void req_axisChange();

};
