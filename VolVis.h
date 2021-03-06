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
#include <vtkCubeSource.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>



class VolVis : public QMainWindow, private Ui::VolVis
{
  Q_OBJECT
public:
	int xlook;
	vtkSmartPointer<vtkCubeSource> CubeSource;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkImageSlice> imageSlice;
	vtkSmartPointer<vtkImageSlice> imageSliceColor;
	vtkSmartPointer<vtkImageSlice> imageSlice2;
	vtkSmartPointer<vtkImageSlice> imageSlice2Color;
	vtkSmartPointer<vtkImageSlice> imageSlice3;
	vtkSmartPointer<vtkImageSlice> imageSlice3Color;
	//vtkSmartPointer<vtkImageSlice> imageSliceMain;
	//vtkSmartPointer<vtkImageSlice> imageSliceMainLeft;
	//vtkSmartPointer<vtkImageSlice> imageSliceMainCenter;
	vtkSmartPointer<vtkLookupTable> lut123 ;
	vtkSmartPointer<vtkLookupTable> lut;
	//vtkSmartPointer<vtkLookupTable> lutSupervising;
	vtkSmartPointer<vtkImageData> colorImagePrediction;
	vtkSmartPointer<vtkImageData> colorImagePrediction_randomForest;
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
	vtkSmartPointer<vtkPolyDataMapper> mapper_main_window;
    vtkSmartPointer<vtkActor> actor_main_window;

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
	vtkSmartPointer<vtkRenderer> mainRenderer;
	vtkSmartPointer<vtkRenderer> mainRenderer_randomForest;
	vtkSmartPointer<vtkRenderer> mainRendererLeft;
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
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	//vtkSmartPointer<vtkProperty> actorprop_main_window;
	std::string loadFileName, predictionFileName,predictionFile_randomForest, indexFileName;
	double opacityRed,opacityGreen,opacityBlue,opacityPurple,opacityOrange,opacityYellow;
	int extent[6];

  // Constructor/Destructor
  VolVis(int, char**); 
  void Render();
  void RenderPrediction();
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
  void LoadTrainFile();
  void setSliceNumberLeft(int);
  void setSliceNumberCenter(int);
  void setSliceNumberRight(int);
  //void setSliceNumberMainLeft(int);
  void renderLeft();
  void renderCenter();
  void renderRight();
  void renderMain();
  void renderMainRandomForest();
  //void clearleft();
  //void clearcenter();
  //void clearright();
  void renderGreenColor();
  void renderBlueColor();
  void renderRedColor();
  void renderOrangeColor();
  void renderYellowColor();
  void renderPurpleColor();
  void renderEraser();
  void trainSVM();
  void updateImageArrayafterTraining();
  void loadFromFile();
  void setPredictFile();
  void setPredictFileRandomForest();
  void setIndexFile();
  void updateOpacity();
  
  //void renderMainLeft();
  //void renderMainCenter();
    
};

#endif
