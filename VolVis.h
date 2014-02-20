#ifndef VolVis_H
#define VolVis_H

#include <vtkSmartPointer.h>
#include <vtkImageSliceMapper.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkImageProperty.h>
#include <vtkProp3D.h>
#include <QMainWindow>
#include <vtkImageData.h>
//#include <QPushButton>


#include "ui_VolVis.h"

class VolVis : public QMainWindow, private Ui::VolVis
{
  Q_OBJECT
public:


	
	vtkSmartPointer<vtkPolyData> linesPolyData_left_red;
	vtkSmartPointer<vtkPolyData> linesPolyData_left_green;
	vtkSmartPointer<vtkPolyData> linesPolyData_left_blue;
	vtkSmartPointer<vtkPolyData> linesPolyData_center_red;
	vtkSmartPointer<vtkPolyData> linesPolyData_center_green;
	vtkSmartPointer<vtkPolyData> linesPolyData_center_blue;
    vtkSmartPointer<vtkPolyData> linesPolyData_right_red;
	vtkSmartPointer<vtkPolyData> linesPolyData_right_green;
	vtkSmartPointer<vtkPolyData> linesPolyData_right_blue;
	
	vtkSmartPointer<vtkPolyDataMapper> mapper_left_red;
    vtkSmartPointer<vtkActor> actor_left_red;
	vtkSmartPointer<vtkPolyDataMapper> mapper_left_green;
    vtkSmartPointer<vtkActor> actor_left_green;
	vtkSmartPointer<vtkPolyDataMapper> mapper_left_blue;
    vtkSmartPointer<vtkActor> actor_left_blue;

	vtkSmartPointer<vtkPolyDataMapper> mapper_center_red;
    vtkSmartPointer<vtkActor> actor_center_red;
	vtkSmartPointer<vtkPolyDataMapper> mapper_center_green;
    vtkSmartPointer<vtkActor> actor_center_green;
	vtkSmartPointer<vtkPolyDataMapper> mapper_center_blue;
    vtkSmartPointer<vtkActor> actor_center_blue;
  
    vtkSmartPointer<vtkPolyDataMapper> mapper_right_red;
    vtkSmartPointer<vtkActor> actor_right_red;
	vtkSmartPointer<vtkPolyDataMapper> mapper_right_green;
    vtkSmartPointer<vtkActor> actor_right_green;
	vtkSmartPointer<vtkPolyDataMapper> mapper_right_blue;
    vtkSmartPointer<vtkActor> actor_right_blue;
  

	vtkSmartPointer<vtkRenderer> leftRenderer;
	vtkSmartPointer<vtkRenderer> centerRenderer;
	vtkSmartPointer<vtkRenderer> rightRenderer;
	vtkSmartPointer<vtkImageProperty> imageSlice2prop ;
	vtkSmartPointer<vtkProperty> actorprop_left_red ;
	vtkSmartPointer<vtkProperty> actorprop_left_green ;
	vtkSmartPointer<vtkProperty> actorprop_left_blue ;
	vtkSmartPointer<vtkProperty> actorprop_center_red;
	vtkSmartPointer<vtkProperty> actorprop_center_green;
	vtkSmartPointer<vtkProperty> actorprop_center_blue;
	vtkSmartPointer<vtkProperty> actorprop_right_red;
	vtkSmartPointer<vtkProperty> actorprop_right_green;
	vtkSmartPointer<vtkProperty> actorprop_right_blue;

  // Constructor/Destructor
  VolVis(); 
  ~VolVis();
protected:
	//void mouseMoveEvent(QMouseEvent *event);
  //vtkSmartPointer<vtkEventQtSlotConnect> Connections;
signals:
     void valueChanged(int value);
	 void clicked();
	 void mouseMoveEvent(QMouseEvent *event);
	// void updateValue(int value);
public slots:
 
  virtual void slotExit();
  //virtual void Slotupdate(int);

  void setSliceNumberLeft(int);
  void setSliceNumberCenter(int);
  void setSliceNumberRight(int);
  void renderLeft();
  void renderCenter();
  void renderRight();
  void clearleft();
  void clearcenter();
  void clearright();
  void renderGreenColor();
  void renderBlueColor();
  void renderRedColor();
  void renderEraser();
  void GetPointGradient(int,int,int,vtkImageData*);







};

#endif