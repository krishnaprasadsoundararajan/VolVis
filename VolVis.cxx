#include "VolVis.h"
#include <FeatureVector.h>
#include <vtkCubeSource.h>
#include <vtkVolumeProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageMapper3D.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageMapper.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageSlice.h>
#include <vtkImageViewer2.h>
#include <vtkXMLImageDataReader.h>


#include <vtkActor2D.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkImageProperty.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkMath.h>
#include <vtkCallbackCommand.h>
#include <qtimer.h>
#include <math.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkLookupTable.h>

#include <fstream>
#include <math.h>
#include <sstream>
#include <iostream>
#include <ctime>
#include<vtkDataArray.h>
#include <vtkPointData.h> // Added to access the instensity colorImage1->GetPointData()-> ....
#include <QtGui>


#define PTR vtkSmartPointer

  
 int Color[3];
 int key_value[5] = { 0,1,2,3,4};
 int left_sliceNumber=30,center_sliceNumber=40,right_sliceNumber=60;
 int *rasterize_array,*rasterize_array_checkcenter,*rasterize_array_checkleft,*rasterize_array_checkright;
 int x_dim,y_dim,z_dim;
 
 
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper2 = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper3 = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapperMain = vtkSmartPointer<vtkImageSliceMapper>::New();
 //vtkSmartPointer<vtkImageSliceMapper> imageSliceMapperMainLeft = vtkSmartPointer<vtkImageSliceMapper>::New();
 //vtkSmartPointer<vtkImageData> colorImage;

//************************************************************************//
//A Class to check for mouse interatcions in the Left Side Renderer Window//
//************************************************************************//

class MouseInteractorStyle4 : public vtkInteractorStyleImage
{
  public:
    static MouseInteractorStyle4* New();
	vtkTypeMacro(MouseInteractorStyle4, vtkInteractorStyleImage);
    int check,right_button_down,left_button_down;
	//On Clicking the right mouse button activates the WindowLevel changes on mouse move
	virtual void OnRightButtonDown()
		{
			right_button_down = 1;
			this->StartWindowLevel();
		}
	//On Releasing the right mouse button deactivates the WindowLevel changes on mouse move
	virtual void OnRightButtonUp()
		{
			right_button_down =0;
			this->EndWindowLevel();
	}

	virtual void OnLeftButtonUp()
	{ 
		left_button_down = 0;
		this->EndWindowLevel();
	}

    virtual void OnLeftButtonDown() 
    {	
		
		left_button_down = 1;
		vtkInteractorStyleImage::OnLeftButtonDown();
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		int z= this->Interactor->GetEventPosition()[2];
	 }
     
	 virtual void OnMouseMove()
	 {
		
		 vtkInteractorStyleImage::OnMouseMove();
		 int x = this->Interactor->GetEventPosition()[0];
         int y = this->Interactor->GetEventPosition()[1];
		 int z= this->Interactor->GetEventPosition()[2];
		 int x_cor,y_cor,z_cor;
		 
		if(left_button_down==1)
			{
				this->EndWindowLevel();
				if(x_dim == y_dim)
				{
					      x_cor = (x_dim*x)/200;
				          y_cor = (y_dim*y)/200;
						  z_cor = left_sliceNumber;
				}	     
				else if(x_dim < y_dim)
				{
					//Needs to be implemented..  Works fine for this Example
				}
				else if(y_dim < x_dim)
				{
	                 //Needs to be implemented..  Works fine for this Example
				}
				if(Color[0] == 255)//Red Color button pressed
					rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 1;//x+(y*x_dim)+((z)*x_dim*y_dim)
				else if(Color[1] == 255)// Green Color button pressed
					rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 2;
				else if(Color[2] == 255)// Blue Color button pressed
					rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 3;
				//Eraser button pressed and checks if the voxel is already colored. Need not erase when not drawn before.
				else if((Color[0] == 333) && (rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] ==1 || rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]==2 || rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]==3))
					rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 4;
             }
		 
		 
    	 }
     	
};


//************************************************************************//
//A Class to check for mouse interatcions in the  Center Renderer Window  //
//************************************************************************//

class MouseInteractorStyleCenter4 : public vtkInteractorStyleImage
{
  public:
	  int right_button_down, left_button_down;
    static MouseInteractorStyleCenter4* New();
    vtkTypeMacro(MouseInteractorStyleCenter4, vtkInteractorStyleImage);
	//On Clicking the right mouse button activates the WindowLevel changes on mouse move
    virtual void OnRightButtonDown()
	{
		right_button_down =1;
		this->StartWindowLevel();
	}
	//On releasing the right mouse button deactivates the WindowLevel changes on mouse move
	virtual void OnRightButtonUp()
	{
		right_button_down =0;
		this->EndWindowLevel();
	}
	//On Clicking the left mouse button activates the WindowLevel changes on mouse move
	virtual void OnLeftButtonUp()
	{ 
		left_button_down = 0;
		this->EndWindowLevel();
	}
    virtual void OnLeftButtonDown() 
    {
		left_button_down = 1;
		this->EndWindowLevel();
		// Forward events
		vtkInteractorStyleImage::OnLeftButtonDown();
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		int z = this->Interactor->GetEventPosition()[1];
	 }
     
	 virtual void OnMouseMove()
	 {
		int x_cor,y_cor,z_cor;
		 vtkInteractorStyleImage::OnMouseMove();
		 int x = this->Interactor->GetEventPosition()[0];
         int y = this->Interactor->GetEventPosition()[1];
		 int z = this->Interactor->GetEventPosition()[2];
		 
		  if( left_button_down == 1 )
			{
			//cout<<"x : "<<x <<" Y :"<<y<<" Z :"<<z<<endl;			  
			   this->EndWindowLevel();			
				if(z_dim == y_dim)
				{
					      z_cor = (z_dim*x)/200;
				          y_cor = (y_dim*y)/200;
						  x_cor = center_sliceNumber;
				}	     
				else if(z_dim < y_dim)
				{
					
					int split = (200-((z_dim*200)/y_dim))/2;
					//cout<<"SPLIT " <<split<<endl;
					       if((x>split)&&(x<(200-split)))
						   {
							   //z_cor = (x * z_dim)/(200-(2*split));
							   //z_cor = (x - split) * z_dim / ( 200 - (2*split));
							   z_cor = (x-split)*(z_dim/((z_dim*200)/y_dim));
							  float temp = float(y_dim)/200;
							  z_cor = (x-split) * temp;
							   y_cor = (y_dim*y)/200;
					           x_cor = center_sliceNumber;
					           
							   if(Color[0] == 255)
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 1;
							   else if(Color[1] == 255)
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 2;
							   else if(Color[2] == 255)
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 3;
							   else if((Color[0] == 333) && (rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]==1 || rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]==2 || rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]==3))
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 4;
							  // cout<<"CENTER Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]<<endl;
						   }
				}
				else if(y_dim < z_dim)
				{
				     //Needs to be implemented.. Works fine with this example
				}
				
				
             }
    	 }
};


//*************************************************************************//
//A Class to check for mouse interatcions in the Right Side Renderer Window//
//*************************************************************************//

 class MouseInteractorStyleRight4 : public vtkInteractorStyleImage
{
  public:

    static MouseInteractorStyleRight4* New();
    vtkTypeMacro(MouseInteractorStyleRight4, vtkInteractorStyleImage);
    int right_button_down, left_button_down;
	//On Clicking the right mouse button activates the WindowLevel changes on mouse move
	virtual void OnRightButtonDown()
	{
		right_button_down =1;
		//mousedown=0;
		this->StartWindowLevel();
	}
	//On releasing the right mouse button deactivates the WindowLevel changes on mouse move
	virtual void OnRightButtonUp()
	{
		right_button_down =0;
		this->EndWindowLevel();
	}

	virtual void OnLeftButtonUp()
	{ 
		left_button_down = 0;
    }
    virtual void OnLeftButtonDown() 
    {

		left_button_down = 1;
		this->EndWindowLevel();
		// Forward events
		vtkInteractorStyleImage::OnLeftButtonDown();
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		int z= this->Interactor->GetEventPosition()[2];
	 }
     
	 virtual void OnMouseMove()
	 {
		 int x_cor,y_cor,z_cor;
		 vtkInteractorStyleImage::OnMouseMove();
		 int x = this->Interactor->GetEventPosition()[0];
         int y = this->Interactor->GetEventPosition()[1];
		 int z = this->Interactor->GetEventPosition()[2];
			  
		  if(left_button_down==1)
			{
				this->EndWindowLevel();
				if(z_dim == y_dim)
				{
					      z_cor = (z_dim*x)/200;
				          y_cor = (y_dim*y)/200;
						  x_cor = right_sliceNumber;
						  
				}	     
				else if(z_dim < x_dim)
				{
					
					
					
					       
						  
							   //z_cor = (x-split)*(z_dim/((z_dim*200)/y_dim));
							  //float temp = float(y_dim)/200;
							  //z_cor = (x-split) * temp;
							  // y_cor = (y_dim*y)/200;
					          // x_cor = center_sliceNumber;
							  // ==============================
					int split = (200-((z_dim*200)/x_dim))/2;
					//cout<<"SPLIT " <<split<<endl;
					       if((y>split)&&(y<(200-split)))
						   {
							   //z_cor = (x * z_dim)/(200-(2*split));
							   //z_cor = (y - split) *(z_dim/((z_dim*200)/y_dim));
							    float temp = float(x_dim)/200;
							  z_cor = (y-split) * temp;
							   x_cor = (x_dim*x)/200;
					           y_cor = right_sliceNumber;
					            
							   if(Color[0] == 255)
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 1;
							   else if(Color[1] == 255)
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 2;
							   else if(Color[2] == 255)
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 3;
							   else if((Color[0] == 333))
									rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 4;
							   //cout<<"RIGHT Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]<<endl;
						   }
					
				}
				else if(x_dim < z_dim)
				{
					//Needs to be checked and implemented.. Works fine for this example.
					int split = (200-((x_dim*200)/(2*x_dim)));
					if(split < x < (200-split))
							   x_cor = (x_dim*(x-split))/(200 - (2*split));
					z_cor = (z_dim*x)/200;
					y_cor = right_sliceNumber;
					rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)] = 1;
				}
				//cout<<"Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor+(y_cor*x_dim)+(z_cor*x_dim*y_dim)]<<endl;
             }
		 
		 
    	 }
 
};

 
vtkStandardNewMacro(MouseInteractorStyle4);
vtkStandardNewMacro(MouseInteractorStyleCenter4);
vtkStandardNewMacro(MouseInteractorStyleRight4);

// Constructor
void VolVis::Render()
{
	PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	source->SetFileName(loadFileName.c_str());
	source->Update();
	
	
	vtkSmartPointer<vtkImageData> colorImage;
	colorImage = source->GetOutput();
	colorImage->UpdateInformation();
	

	
	colorImage->GetExtent(extent);
 
    double origin[3];
    colorImage->GetOrigin(origin);
 
    double spacing[3];
    colorImage->GetSpacing(spacing);
	//colorImage->SetOrigin(64,64,47);
	
	//Setting the Dimensions from the image read
	x_dim = colorImage->GetDimensions()[0], y_dim = colorImage->GetDimensions()[1], z_dim = colorImage->GetDimensions()[2];
	rasterize_array = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	rasterize_array_checkcenter = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	rasterize_array_checkleft = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	rasterize_array_checkright = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	
	
	for(int i = 0; i < x_dim; i++)
		for(int j = 0; j < y_dim; j++)
			for(int k = 0; k < z_dim; k++)
			{
				rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)]=0;
				rasterize_array_checkcenter[i+(j*x_dim)+(k*x_dim*y_dim)]=0;
				rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)]=0;
				rasterize_array_checkright[i+(j*x_dim)+(k*x_dim*y_dim)]=0;
			}

  //**********************************************************************************//
  //***********************************Left Renderer**********************************//
  //Set Connection to the mapper                                                      //
  //Set Slicer                                                                        //
  //Add Actor                                                                         //
  //Set if there is any rotation                                                      //
  //Set Camera                                                                        //
  //Add to Renderer                                                                   //
  //**********************************************************************************//
  imageSliceMapper->SetInputConnection(colorImage->GetProducerPort());
  imageSlice->SetMapper(imageSliceMapper);
  
    
  imageSliceMapper->SliceFacesCameraOn();
  imageSliceMapper->SetSliceNumber(30);
  leftRenderer->AddActor(imageSlice);
  leftRenderer->ResetCamera();//----------------------
  renderWindow->AddRenderer(leftRenderer);
  leftRenderer->SetBackground(.6, .5, .4);  
  vtkCamera* leftCamera = leftRenderer->GetActiveCamera();
  leftCamera->ParallelProjectionOn();
 
  float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
  float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
//  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
  float yd = (extent[3] - extent[2] + 1)*spacing[1];
 
  float d = leftCamera->GetDistance();
 
  leftCamera->SetParallelScale(0.5f*static_cast<float>(yd));
  
  leftCamera->SetFocalPoint(xc,yc,0.0);
  leftCamera->SetPosition(xc,yc,+d);
  //**********************************************************************************//
  //************************Center Renderer*******************************************//
  //Set Connection to the mapper                                                      //
  //Set Slicer                                                                        //
  //Add Actor                                                                         //
  //Set if there is any rotation                                                      //
  //Set Camera                                                                        //
  //Add to Renderer                                                                   //
  //**********************************************************************************//
  imageSliceMapper2->SetInputConnection(colorImage->GetProducerPort());
  
  
  imageSlice2->SetMapper(imageSliceMapper2);

  imageSliceMapper2->SliceFacesCameraOn();
  imageSliceMapper2->SetSliceNumber(30);
  imageSlice2->SetOrigin(x_dim/2,y_dim/2,z_dim/2);
  imageSlice2->RotateY(90);
  //imageSlice2->RotateWXYZ(0,0,90,0);
  imageSliceMapper2->SetOrientationToX();

  
  centerRenderer->AddActor(imageSlice2); 
  centerRenderer->ResetCamera();
  
  renderWindow->AddRenderer(centerRenderer);
  centerRenderer->SetBackground(.4, .2, .6);
 
  vtkCamera* centerCamera = centerRenderer->GetActiveCamera();
  centerCamera->ParallelProjectionOn();
  float zcC = origin[2] + 0.5*(extent[4] + extent[5])*spacing[2];
  float ycC = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
  float ydC = (extent[1] - extent[0] + 1)*spacing[0];
 
  float dC = centerCamera->GetDistance();
  centerCamera->SetParallelScale(0.5f*static_cast<float>(yd));
  //centerCamera->SetFocalPoint(zcC,ycC,0.0);
  //centerCamera->SetPosition(zcC,ycC,+dC);
  centerCamera->SetFocalPoint(xc,yc,0.0);
  centerCamera->SetPosition(xc,yc,+d);
 //centerCamera->SetFocalPoint(0,ycC,zcC);
 //centerCamera->SetPosition(+dC,ycC,zcC); 
  //==========================================================================================================================



  //**********************************************************************************//
  //******************************Right Renderer**************************************//
  //Set Connection to the mapper                                                      //
  //Set Slicer                                                                        //
  //Add Actor                                                                         //
  //Set if there is any rotation                                                      //
  //Set Camera                                                                        //
  //Add to Renderer                                                                   //
  //**********************************************************************************//
 
  imageSliceMapper3->SetInputConnection(colorImage->GetProducerPort());
  
  
  imageSlice3->SetMapper(imageSliceMapper3);
	
  imageSliceMapper3->SliceFacesCameraOn();
  imageSliceMapper3->SetSliceNumber(30);

  imageSlice3->SetOrigin(x_dim/2,y_dim/2,z_dim/2);
  imageSlice3->RotateX(90);
  //imageSliceMapper3->SetOrientationToX();
  
  rightRenderer->AddActor(imageSlice3);
  rightRenderer->ResetCamera();
  renderWindow->AddRenderer(rightRenderer);
  rightRenderer->SetBackground(.4, .5, .6);
  vtkCamera* rightCamera = rightRenderer->GetActiveCamera();
  rightCamera->ParallelProjectionOn();
  float zcR = origin[2] + 0.5*(extent[4] + extent[5])*spacing[2];
  float xcR = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
  float ydR = (extent[3] - extent[2] + 1)*spacing[0];
  float dR = rightCamera->GetDistance();
  cout<<endl<<"zcr" <<zcR<<endl;
  rightCamera->SetParallelScale(0.5f*static_cast<float>(yd));
  rightCamera->SetFocalPoint(xc,yc,0.0);
  rightCamera->SetPosition(xc,yc,+d);
  //rightCamera->SetFocalPoint(xcR,0,zcR);
  //rightCamera->SetPosition(xcR,+dR,zcR+10);
  vtkSmartPointer<MouseInteractorStyle4> style1 = vtkSmartPointer<MouseInteractorStyle4>::New();
   vtkSmartPointer<MouseInteractorStyleCenter4> style2 = vtkSmartPointer<MouseInteractorStyleCenter4>::New();
   vtkSmartPointer<MouseInteractorStyleRight4> style3 = vtkSmartPointer<MouseInteractorStyleRight4>::New();
  //**********************************************************************************//
  //******************************QT RELATED STUFF*************************************//
  //Set Background color for the buttons                                               //
  //Set the maximum and minimum Value for the Sliders                                  //
  //Add Interactors                                                                    //
  //Add Renderer to the QVTKWidget                                                     //
  //Set Signals and Slots to call methods on interactions                              //
  //Set Timers so that the functions are called repeatedly based on interval           //
  //***********************************************************************************//
    this->horizontalSliderLeft->setMinimum(extent[4]);
	this->horizontalSliderLeft->setMaximum(extent[5]);
   
	this->horizontalSliderCenter->setMinimum(extent[0]);
	this->horizontalSliderCenter->setMaximum(extent[1]);

	this->horizontalSliderRight->setMinimum(extent[2]);
	this->horizontalSliderRight->setMaximum(extent[3]);
  
    
	
	this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style1);
	this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style2);
	this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
	
	this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style3);
	this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
  
  	 
    // Set up action signals and slots
	//this->horizontalSliderMainCenter->connect(this->horizontalSliderMainCenter,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberMainLeft(int)));
	this->horizontalSliderLeft->connect(this->horizontalSliderLeft,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberLeft(int))); 
	this->horizontalSliderCenter->connect(this->horizontalSliderCenter,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberCenter(int))); 
	this->horizontalSliderRight->connect(this->horizontalSliderRight,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberRight(int))); 
  
  
    this->pushButtonBlue->connect(this->pushButtonBlue,SIGNAL(clicked()),this,SLOT(renderBlueColor()));
	this->pushButtonGreen->connect(this->pushButtonGreen,SIGNAL(clicked()),this,SLOT(renderGreenColor()));
	this->pushButtonRed->connect(this->pushButtonRed,SIGNAL(clicked()),this,SLOT(renderRedColor()));
	this->pushButtonEraser->connect(this->pushButtonEraser,SIGNAL(clicked()),this,SLOT(renderEraser()));
	this->TrainSVM->connect(this->TrainSVM,SIGNAL(clicked()),this,SLOT(trainSVM()));
	
    QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(renderLeft()));
	connect(timer, SIGNAL(timeout()), this, SLOT(renderCenter()));
	connect(timer, SIGNAL(timeout()), this, SLOT(renderRight()));
    
	timer->start(20);
}	
VolVis::VolVis() 
{
   this->setupUi(this);
   //***********************************************************************//
   //Initializations                                                        //
   //***********************************************************************//

    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(800, 600);
	
	renderWindow->FullScreenOn();
	renderWindow->Render();
	//renderWindow->SetWindowName("Transfer Function based Direct Volume Rendering - Dr.Thomas Schultz, Krishna Prasad Soundararajan");

	//loadFileName = "I:\assignment-06-octree-solution (1)\assignment-06-octree-solution\headsq-half.vti";
	//indexFileName = "E:\SemesterIII\Thesis\libsvm-3.17\windows\Index.dat";
	//predictionFileName = "E:\SemesterIII\Thesis\libsvm-3.17\windows\PredictedOutput";
	
	QMenu *fileMenu; 
	
	QAction *loadFile;
	fileMenu = menuBar()->addMenu(tr("&File"));
    loadFile = new QAction(tr("&Load New File"), this);
    loadFile->setShortcuts(QKeySequence::New);
    loadFile->setStatusTip(tr("Load a new file"));
    connect(loadFile, SIGNAL(triggered()), this, SLOT(loadFromFile()));
    fileMenu->addAction(loadFile);
	
	QAction *predictFile;
	predictFile = new QAction(tr("&Load Prediction File"), this);
    predictFile->setShortcuts(QKeySequence::New);
    predictFile->setStatusTip(tr("Load the file for prediction"));
    connect(predictFile, SIGNAL(triggered()), this, SLOT(setPredictFile()));
    fileMenu->addAction(predictFile);

	QAction *indexFile;
	indexFile = new QAction(tr("&Load Index File"), this);
    indexFile->setShortcuts(QKeySequence::New);
    indexFile->setStatusTip(tr("Load the file Index for prediction"));
    connect(indexFile, SIGNAL(triggered()), this, SLOT(setIndexFile()));
    fileMenu->addAction(indexFile);

    
	//Initalize arrays to check for rendering
	

  
  
  imageSlice = vtkSmartPointer<vtkImageSlice>::New();
  //imageSliceMainLeft = vtkSmartPointer<vtkImageSlice>::New();
  
  

  leftRenderer = vtkSmartPointer<vtkRenderer>::New();
  //mainRendererLeft = vtkSmartPointer<vtkRenderer>::New();
  //double leftViewport[4] = {0.0, 0.0, 0.33, 0.4};
  //double centerViewport[4] ={0.33, 0.0, 0.66, 0.4};
  //double rightViewport[4] = {0.66, 0.0, 1.0, 0.4};
  
  imageSlice2 = vtkSmartPointer<vtkImageSlice>::New();
  centerRenderer = vtkSmartPointer<vtkRenderer>::New();
  
  imageSlice3 = vtkSmartPointer<vtkImageSlice>::New();
  rightRenderer = vtkSmartPointer<vtkRenderer>::New();
 
  //==========================================================================================================================


   
 //==========================================================================================================================


   mainRenderer = vtkSmartPointer<vtkRenderer>::New();
   mainRenderer->SetBackground(.6, .5, .4);    
 
   
   this->pushButtonBlue->setAutoFillBackground(true);	
	this->pushButtonRed->setAutoFillBackground(true);
	this->pushButtonGreen->setAutoFillBackground(true);
	
	this->pushButtonBlue->setStyleSheet("background-color: rgb(0, 0, 255); color: rgb(255, 255, 255)");
	this->pushButtonGreen->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(255, 255, 255)");
	this->pushButtonRed->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
	
	
	
	
	
	this->PredictSVM->connect(this->PredictSVM,SIGNAL(clicked()),this,SLOT(updateImageArrayafterTraining()));
	
	this->pushButtonOpacity->connect(this->pushButtonOpacity,SIGNAL(clicked()),this,SLOT(updateOpacity()));
	//connect(timer, SIGNAL(timeout()), this, SLOT(renderMain()));
	opacityRed = opacityBlue = opacityGreen = 0.5;
}


void VolVis::renderBlueColor()
{
	Color[0] = 0;
	Color[1] = 0;
	Color[2] = 255;
	cout<<"Blue Color " <<endl;

}
void VolVis::renderGreenColor()
{
	Color[0] = 0;
	Color[1] = 255;
	Color[2] = 0;
	cout<<"Green Color " <<endl;
}
void VolVis::renderRedColor()
{
	Color[0] = 255;
	Color[1] = 0;
	Color[2] = 0;
	cout<<"Red Color " <<endl;
}
void VolVis::renderEraser()
{
	Color[0] = 333;
	Color[1] = 999;
	Color[2] = 999;
	cout<<"Eraser " <<endl;
}
/*void VolVis::renderMainLeft()
{    
	double origin[3];
    colorImage->GetOrigin(origin);
 
    double spacing[3];
    colorImage->GetSpacing(spacing);
	vtkSmartPointer<vtkLookupTable> lut123 = vtkSmartPointer<vtkLookupTable>::New();
  //int tableSize = std::max(resolution*resolution + 1, 10);
  lut123->SetNumberOfTableValues(x_dim*y_dim*z_dim);
  //lut->SetNumberOfColors(4);
  lut123->Build();
  imageSliceMapperMainLeft->SetInputConnection(colorImage->GetProducerPort());
  imageSliceMainLeft->SetMapper(imageSliceMapper);
  imageSliceMainLeft->GetProperty()->SetLookupTable(lut);
  imageSliceMainLeft->Update();
  imageSliceMapperMainLeft->SliceFacesCameraOn();
  imageSliceMapperMainLeft->SetSliceNumber(30);
  mainRendererLeft->AddActor(imageSliceMainLeft);
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  lut123->SetTableValue(i+(j*x_dim)+(k*x_dim*y_dim),0,0,0,1);
  lut->Build();
	 for( int i =0 ; i < x_dim; i++)
	  for( int j =0 ; j < y_dim; j++)
		  for( int k = 0 ; k < z_dim; k++)
		  {
			  if((rasterize_array[i+(k*x_dim)+(k*x_dim*y_dim)] == 6) || (rasterize_array[i+(k*x_dim)+(k*x_dim*y_dim)] == 1))
			  lut123->SetTableValue((i+(k*x_dim)+(k*x_dim*y_dim)),1,0,0,1);
			  else if((rasterize_array[i+(k*x_dim)+(k*x_dim*y_dim)] == 7) || (rasterize_array[i+(k*x_dim)+(k*x_dim*y_dim)] == 2))
			  lut123->SetTableValue(i+(k*x_dim)+(k*x_dim*y_dim),0,1,0,1);
			  else if((rasterize_array[i+(k*x_dim)+(k*x_dim*y_dim)] == 8) || (rasterize_array[i+(k*x_dim)+(k*x_dim*y_dim)] == 3))
			  lut123->SetTableValue(i+(k*x_dim)+(k*x_dim*y_dim),0.5,0.6,0.2,1);
			  else
			  {
				  //cout<<i+(j*x_dim)+(k*x_dim*y_dim)<<endl;
				  lut123->SetTableValue(i+(k*x_dim)+(k*x_dim*y_dim),0.6,0.6,0,1);
			  }
		  }
			//  if(((rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3) && (rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)] != 1)) || (rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4 && (rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)))
			 // {
				//  rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)]=1;
				  
			 // }
			  //leftRenderer->AddActor(imageSlice);
		  imageSliceMainLeft->GetProperty()->SetLookupTable(lut);
  imageSliceMainLeft->Update();
  imageSliceMapperMainLeft->SliceFacesCameraOn();
  imageSliceMapperMainLeft->SetSliceNumber(30);
  mainRendererLeft->AddActor(imageSliceMainLeft);
  mainRendererLeft->ResetCamera();//----------------------
  renderWindow->AddRenderer(mainRendererLeft);
  mainRendererLeft->SetBackground(.6, .5, .4);  
  vtkCamera* mainLeftCamera = mainRendererLeft->GetActiveCamera();
  mainLeftCamera->ParallelProjectionOn();
 
  float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
  float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
//  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
  float yd = (extent[3] - extent[2] + 1)*spacing[1];
 
  float d = mainLeftCamera->GetDistance();
 
  mainLeftCamera->SetParallelScale(0.5f*static_cast<float>(yd));
  
  mainLeftCamera->SetFocalPoint(xc,yc,0.0);
  mainLeftCamera->SetPosition(xc,yc,+d);
	this->qvtkWidgetMainCenter->GetRenderWindow()->RemoveRenderer(mainRendererLeft);		  
  this->qvtkWidgetMainCenter->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetMainCenter->GetRenderWindow()->GetInteractor()->Render();
	
	//this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->ReInitialize();
	//this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->Render();
	this->qvtkWidgetMainCenter->GetRenderWindow()->AddRenderer(mainRendererLeft);
	this->qvtkWidgetMainCenter->GetRenderWindow()->Render();
	this->qvtkWidgetMainCenter->update();
			  
}*/
void VolVis::trainSVM()
{
	FeatureVector obj;
	obj.CreateTestTrainData(x_dim,y_dim,z_dim,&rasterize_array[0]);

	/*PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	source->SetFileName("I:/assignment-06-octree-solution (1)/assignment-06-octree-solution/headsq-half.vti");
	source->Update();
	//PTR<vtkImageData>
	vtkSmartPointer<vtkImageData> colorImage1;
		colorImage1 = source->GetOutput();
	colorImage1->UpdateInformation();
	//rasterize_array = new int[x_dim*y_dim*z_dim];
	//ofstream outputFileTest,outputFileTrain,outputFileIndex,outputFileIndex1;
    //outputFileTest.open("E:/SemesterIII/Thesis/libsvm-3.17/windows/Test.dat");
	//outputFileTrain.open("E:/SemesterIII/Thesis/libsvm-3.17/windows/Train.dat");
	//outputFileIndex.open("E:/SemesterIII/Thesis/libsvm-3.17/windows/Index.dat");
	//outputFileIndex1.open("E:/SemesterIII/Thesis/libsvm-3.17/windows/Index1.dat");

	//Instead of using ofstream using FILE* reduces the time for writing the file by almost 50%
	
	FILE *outputFileTest,*outputFileTrain,*outputFileIndex,*outputFileIndex1;
	outputFileTest = fopen("E:/SemesterIII/Thesis/libsvm-3.17/windows/Test.dat","w");
	outputFileTrain = fopen("E:/SemesterIII/Thesis/libsvm-3.17/windows/Train.dat","w");
	outputFileIndex = fopen("E:/SemesterIII/Thesis/libsvm-3.17/windows/Index.dat","w");
	outputFileIndex1 = fopen("E:/SemesterIII/Thesis/libsvm-3.17/windows/Index1.dat","w");

	cout<<"File created";
	//cout<<"inside test train data "<<g[10]<<endl;
	int extent[6];
	unsigned long long t1,t2;
	double *g = new double[3];
	//double *g8 = new double[8];
	//vtkSmartPointer<vtkDataArray> g8 = vtkSmartPointer<vtkDataArray>::New();
	std::stringstream ssTest,ssTrain,ssIndex,ssIndex1;
  extent[0] = extent[1] = extent[2] = extent[3] = extent[4] = extent[5] = 0; 
  colorImage1->GetExtent(extent);
 
  cout<<"Extents "<<extent[0]<<" "<<extent[1]<<" "<<extent[2]<<" "<<extent[3]<<" "<<extent[4]<<" "<<extent[5]<<" "<<endl;
  std::clock_t c_start= std::clock();
	for(int x = extent[0]; x < extent[1]; x++) //extent[1]
	   for(int y = extent[2]; y < extent[3]; y++)//extent[3]
		  for(int z = extent[4]; z < extent[5]; z++)//extent[5]
		   {
			   
			   //cout<<x<<" "<<y<<" "<<z<<" "<<z<<endl;
			  // GetPointGradient(x,y,z,colorImage1,g);
			   colorImage1->GetPointGradient(x,y,z,colorImage1->GetPointData()->GetScalars(),g);
			  // colorImage1->GetVoxelGradient(x,y,z,colorImage1->GetPointData()->GetScalars(),g8);
			  // if(g[0]!=0 && g[1]!=0 && g[2]!=0)
			   {
				   //colorImage1->GetPointGradient(x,y,z,colorImage1,g[3]);
				    double sq_gradientMagnitude = (g[0]*g[0]) +(g[1]*g[1])+(g[2]*g[2]);
				    double gradient_magnitude = sqrt(sq_gradientMagnitude);
					//cout<<rasterize_array[x+(y*x_dim)+((z)*x_dim*y_dim)];
					if((rasterize_array[x+(y*x_dim)+((z)*x_dim*y_dim)]!=0))// || ((x+(y*x_dim)+(z*x_dim*y_dim))%1000 == 0))
						{
							//cout<<"x+(y*x_dim)+(z*x_dim*y_dim)"<< x+(y*x_dim)+(z*x_dim*y_dim);
							//ssTrain<<rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]<<" "<<"1:"<<colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim))<<" 2:"<<g[0]<<" 3:" <<g[1]<<" 4:"<<g[2]<<" 5:"<<gradient_magnitude<<endl;
				        	//ssIndex1<<x+(y*(extent[1] - extent[0]))+(z*(extent[1] - extent[0])*(extent[3] - extent[2]))<<endl;
					        fprintf(outputFileTrain,"%d 1:%lf 2:%lf 3:%lf 4:%lf 5:%lf 6:%d 7:%d 8:%d\n",rasterize_array[x+(y*x_dim)+((z)*x_dim*y_dim)],g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),x,y,z);
					        fprintf(outputFileIndex1,"%d\n",x+(y*(extent[1] - extent[0]))+(z*(extent[1] - extent[0])*(extent[3] - extent[2])));
					   }
					else
					{
						//cout<<"writing test"<<
						
						//cout<<"G: "<<g[0]<<" "<<g[1]<<" "<<g[2];
			            //ssTest<<rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]<<" "<<"1:"<<g[0]<<" 2:"<<g[1]<<" 3:" <<g[2]<<" 4:"<<gradient_magnitude<<" 5:"<<colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim))<<endl;
						//outputFileTest<<rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]<<" "<<"1:"<<g[0]<<" 2:"<<g[1]<<" 3:" <<g[2]<<" 4:"<<gradient_magnitude<<" 5:"<<colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim))<<endl;
					   //ssIndex<<x+(y*(extent[1] - extent[0]))+(z*(extent[1] - extent[0])*(extent[3] - extent[2]))<<endl;
					   fprintf(outputFileTest,"%d 1:%lf 2:%lf 3:%lf 4:%lf 5:%lf 6:%d 7:%d 8:%d\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),x,y,z);
					   fprintf(outputFileIndex,"%d\n",x+(y*(extent[1] - extent[0]))+(z*(extent[1] - extent[0])*(extent[3] - extent[2])));
					}
				
						
			   }}
		  std::clock_t end = std::clock();
		  cout<<"time taken " <<end-c_start<<endl;
		  //outputFileTest<<ssTest;
		  //outputFileTrain<<ssTrain;
		  //outputFileIndex<<ssIndex;
		  //outputFileIndex1<ssIndex1;
		  //outputFileTest.close();
		  //outputFileTrain.close();
		  //outputFileIndex.close();
		  //outputFileIndex1.close();
		  fclose(outputFileTest);
		  fclose(outputFileTrain);
		  fclose(outputFileIndex);
		  fclose(outputFileIndex1);
		*/  cout<<"============================COMPLETED===================================================";
}


//**************************************************************************************************************************************//
// This function is called by the a function when Predict SVM button is pressed.                                                        //
// This function visualizes the Main renderer window reading the values from the Predicted output of the SVM                            //
//**************************************************************************************************************************************//

void VolVis::renderMain()
{
	if(indexFileName.empty() || predictionFileName.empty() || loadFileName.empty())
	{
		cout<<"inside if"<<endl;
		QMessageBox msgBox;
		msgBox.setText("Please Load Prediction,Index and Training File");
		msgBox.exec();

	}
	else
	{
	cout<<"Button Pressed"<<endl;
	PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	source->SetFileName(loadFileName.c_str());
	
	source->Update();
	//PTR<vtkImageData>
	//vtkSmartPointer<vtkImageData> colorImage;
	colorImagePrediction = source->GetOutput();
	colorImagePrediction->UpdateInformation();
	int one=0,two=0,three =0;
	//lut = vtkSmartPointer<vtkLookupTable>::New();
  //int tableSize = std::max(resolution*resolution + 1, 10);
  //lut->SetNumberOfTableValues(128*128*93);
  //lut->SetNumberOfColors(4);
  //lut->Build();
	//============================================
	cout<<"called============";
 /*	int *v = new int[x_dim*y_dim*z_dim];
	int *pv = new int[x_dim*y_dim*z_dim];
    int num = 0,prediction = 0;
	
		cout<<"inside else"<<endl;
	ifstream fin(indexFileName);
	ifstream fin1(predictionFileName);
    char buffer[512],buffer_prediction[512];
    int i = 0;
    int bytes = 0, bytes_prediction = 0;
    char *p,*predict;
    int hasnum = 0;
    int eof = 0;
    while(!eof)
    {
    fin.read(buffer, sizeof(buffer));
	if(bytes_prediction > 0);
	fin1.read(buffer_prediction, sizeof(buffer_prediction));
    p = buffer;
	predict = buffer_prediction;
	//cout<<"Predict"<< predict;
    bytes = 512;
	bytes_prediction = 512;
   while((bytes > 0) && (bytes_prediction > 0) )
    {
        if (*p == 26)   // End of file marker...
        {
        eof = 1;
        break;
        }
        if (*p == '\n' || *p == ' ')
        {
        //cout<<"Num :"<<num << " : " <<prediction<<endl;
        if (hasnum)
		{
            v[i++] = num;
            //pv[i++] = prediction;
		}
		int prod = (extent[1] - extent[0])*(extent[3] - extent[2]);
		int x1 = num % ((extent[1] - extent[0]));
		int y1 = (num% prod) / x_dim;
		int z1 = num /prod;
		if(prediction==1)
		{
			//colorImagePrediction->SetScalarComponentFromDouble(x1,y1,z1,0,150);
			lut->SetTableValue(num     , 1     , 0     , 0, 1);
			one++;
		}
		else if(prediction==2)
		{
			//colorImagePrediction->SetScalarComponentFromDouble(x1,y1,z1,0,550);
			lut->SetTableValue(num     , 0     , 1     , 0, 1);
		    two++;
		}
		else if(prediction==3)
		{
			//colorImagePrediction->SetScalarComponentFromDouble(x1,y1,z1,0,1200);
			lut->SetTableValue(num     , 0     , 0     , 0, 1);
		    three++;
		}
		else 
			cout << num<<endl;

		rasterize_array[x1+(y1*x_dim)+(z1*x_dim*y_dim)] = prediction+5;
		if(((x1+(y1*x_dim)+(z1*x_dim*y_dim)) ==640) || ((x1+(y1*x_dim)+(z1*x_dim*y_dim)) ==896) || ((x1+(y1*x_dim)+(z1*x_dim*y_dim)) ==1152))
		{
			cout<<"Num :"<<((x1+(y1*x_dim)+(z1*x_dim*y_dim)))<<endl;
		}
		num = 0;
		prediction = 0;
        p++;
		predict++;
		predict++;
        bytes--;
		bytes_prediction--;
        hasnum = 0;
		
        }
        else if (*p >= '0' &&  *p <= '9')
        {
        if(hasnum == 0)
			{
				prediction = *predict-48;
				
		    }
				hasnum = 1;

		//prediction = *predict;
        num *= 10;
        num += *p -'0';
        p++;
        bytes--;
		//cout<<"Num :"<<num << " : " <<prediction<<endl;
        }
        else 
        {
        cout << "Error..." << endl;
        exit(1);
        }
		
		
    }
	//cout<<buffer<<endl;
    memset(buffer, 26, sizeof(buffer));  // To detect end of files. 
    }
    cout << "Number of values read " << i <<" one :"<<one<<"two :"<<two<<" three :"<<three<< endl;

    delete [] v;*/

 //for( int i =0 ;i<x_dim;i++)
	//  for( int j =0 ;j< y_dim;j++)
		//  for( int k =0 ;k< z_dim;k++)
			//  lut->SetTableValue(i+(j*x_dim)+(k*x_dim*y_dim),0,0,0,1);
//lut->Build();
int num1,num2;
std::string line,line1;
ifstream myfile (predictionFileName);
ifstream myfile1(indexFileName);
 if (myfile.is_open())
  {
    while (! myfile.eof() )
    {
      getline(myfile,line);
	  getline(myfile1,line1);
	  num1 = atoi(line1.c_str());
		  num2 = atoi(line.c_str());
		int prod = x_dim*y_dim;
		int x1 = num1 % x_dim;
		int y1 = (num1% prod) / x_dim;
		int z1 = num1 /prod;
	 // cout<<atoi(line.c_str())<<" "<<atoi(line1.c_str())<<endl;
     if (num2==1)
      {
	//	  lut->SetTableValue(num1,1,0,0.7,1);
		  rasterize_array[num1] = num2 +5;
		  colorImagePrediction->SetScalarComponentFromDouble(x1,y1,z1,0,150);
		  one++;
      // read next line and print it... but how?
      }
	 else if (num2==2)
      {
		  rasterize_array[num1] = num2 +5;
		  //lut->SetTableValue(num1,0.7,1,0,1);
		  colorImagePrediction->SetScalarComponentFromDouble(x1,y1,z1,0,550);
		  two++;
      // read next line and print it... but how?
      }
	 else if (num2==3)
      {
		  rasterize_array[num1] = num2 +5;
		  //lut->SetTableValue(num1,0.7,0.3,0,1);
		  colorImagePrediction->SetScalarComponentFromDouble(x1,y1,z1,0,1200);
		  three++;
      // read next line and print it... but how?
      }
	 else
		 cout<< num1 << " " <<num2<<endl;
	}
    
  }
  //else cout << "Unable to open file"; 
    cout<<"ONE :" <<one<<endl;
	cout<<"two :" <<two<<endl;
	cout<<"three :" <<three<<endl;
	//lut->Build();
	cout<<"The values are read successfully predicted by the SVM and the scalar values are set accordinly"<<endl;
	//--------------------------------------------
	//ifstream myfile3("E:\SemesterIII\Thesis\libsvm-3.17\windows\Index1.dat");
	//std::string line3;
	//int num3;
 //if (myfile3.is_open())
  //{
    //while (! myfile.eof() )
    //{
      //getline(myfile3,line3);
	  //num3 = atoi(line3.c_str());
		//  rasterize_array[num3] =  6;
		 // lut->SetTableValue(num1,0,0,1,1);
		  //one++;
      // read next line and print it... but how?
      }
	// 
    
  //}
  //else cout << "Unable to open file"; 
    //cout<<"ONE :" <<one<<endl;
	//cout<<"two :" <<two<<endl;
	//cout<<"three :" <<three<<endl;
	//cout<<"The values are read successfully predicted by the SVM and the scalar values are set accordinly"<<endl;
	//--------------------------------------------
	//VolVis::renderMainLeft();
	VolVis::RenderPrediction();		 
	//myfile.close();
	//myfile1.close();
 }


void VolVis::RenderPrediction()
{
	  vtkSmartPointer<vtkVolumeRayCastCompositeFunction> rayCastFunction =
      vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
  
     vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper =
       vtkSmartPointer<vtkVolumeRayCastMapper>::New();
     volumeMapper->SetInputConnection(colorImagePrediction->GetProducerPort());
     volumeMapper->SetVolumeRayCastFunction(rayCastFunction);
	 
    // The color transfer function maps voxel intensities to colors.
    // It is modality-specific, and often anatomy-specific as well.
    // The goal is to one color for flesh (between 500 and 1000)
     // and another color for bone (1150 and over).
	//vtkSmartPointer<vtkDiscretizableColorTransferFunction> volumeColor = 
    //vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
    vtkSmartPointer<vtkColorTransferFunction>volumeColor =
      vtkSmartPointer<vtkColorTransferFunction>::New();
	//volumeColor->DiscretizeOn();
	//volumeColor->SetNumberOfValues(3);
	 volumeColor->AddRGBPoint(0,    0.0, 0.0, 0.0);
	  volumeColor->AddRGBPoint(150,  1, 0, 0);
      volumeColor->AddRGBPoint(550, 0, 1, 0);
	  volumeColor->AddRGBPoint(1200, 0, 0, 1);
	 
    // The opacity transfer function is used to control the opacity
    // of different tissue types.
    vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity =
       vtkSmartPointer<vtkPiecewiseFunction>::New();
     volumeScalarOpacity->AddPoint(0,    0);
	 volumeScalarOpacity->AddPoint(99,    0);
     volumeScalarOpacity->AddPoint(100,  opacityRed);
	 volumeScalarOpacity->AddPoint(200,  opacityRed);
     volumeScalarOpacity->AddPoint(500, opacityGreen);
	 volumeScalarOpacity->AddPoint(600, opacityGreen);
	 volumeScalarOpacity->AddPoint(1000, opacityBlue);
	 volumeScalarOpacity->AddPoint(1400, opacityBlue);
	 volumeScalarOpacity->AddPoint(140001, 0);
   
     // The gradient opacity function is used to decrease the opacity
     // in the "flat" regions of the volume while maintaining the opacity
    // at the boundaries between tissue types.  The gradient is measured
    // as the amount by which the intensity changes over unit distance.
    // For most medical data, the unit distance is 1mm.
    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
      vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity->AddPoint(0,   0.0);
    volumeGradientOpacity->AddPoint(90,  0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);
 
   // The VolumeProperty attaches the color and opacity functions to the
   // volume, and sets other volume properties.  The interpolation should
   // be set to linear to do a high-quality rendering.  The ShadeOn option
   // turns on directional lighting, which will usually enhance the
    // appearance of the volume and make it look more "3D".  However,
    // the quality of the shading depends on how accurately the gradient
    // of the volume can be calculated, and for noisy data the gradient
    // estimation will be very poor.  The impact of the shading can be
    // decreased by increasing the Ambient coefficient while decreasing
    // the Diffuse and Specular coefficient.  To increase the impact
    // of shading, decrease the Ambient and increase the Diffuse and Specular.
   vtkSmartPointer<vtkVolumeProperty> volumeProperty =
      vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetColor(volumeColor);
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty->SetInterpolationTypeToLinear();
	//volumeProperty->SetInterpolationTypeToNearest();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);
	
    // The vtkVolume is a vtkProp3D (like a vtkActor) and controls the position
    /// and orientation of the volume in world coordinates.
    vtkSmartPointer<vtkVolume> volume =
     vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
	mainRenderer->RemoveAllViewProps();
	//mainRenderer->Clear();
    // Finally, add the volume to the renderer
    mainRenderer->AddViewProp(volume);

	//vtkSmartPointer<vtkImageMapToColors> firstColorMapper = 
      //vtkSmartPointer<vtkImageMapToColors>::New();
	//firstColorMapper->SetInputConnection( colorImage->GetProducerPort() );
	//lut->SetRange( 0.0, 255.0 );
    //lut->SetHueRange( 0.0, 0.1 );
    //lut->SetValueRange( 0.4, 0.8 );
   // lut->Build();
    //firstColorMapper->SetLookupTable( lut );
	//vtkSmartPointer<vtkImageActor> imgActor = 
      //vtkSmartPointer<vtkImageActor>::New();
	
   //vtkSmartPointer<vtkDataSetMapper> imgDataSetMapper = 
     // vtkSmartPointer<vtkDataSetMapper>::New();
   //imgDataSetMapper->SetInputConnection(firstColorMapper->GetOutputPort());
 // imgActor->SetMapper(imgDataSetMapper);
  //mainRenderer->AddActor(imgActor);


    // Set up an initial view of the volume.  The focal point will be the
    // center of the volume, and the camera position will be 400mm to the
    // patient's left (which is our right).
    vtkCamera *camera = mainRenderer->GetActiveCamera();
    double *c = volume->GetCenter();
    camera->SetFocalPoint(c[0], c[1], c[2]);
    camera->SetPosition(c[0] + 400, c[1], c[2]);
    camera->SetViewUp(0, 0, -1);
  
    //this->qvtkWidgetMain->GetRenderWindow()->RemoveRenderer(mainRenderer);
  	//this->qvtkWidgetMain->GetRenderWindow()->AddRenderer(mainRenderer);
  	//this->qvtkWidgetMain->GetRenderWindow()->Render();
	//this->qvtkWidgetMain->update(); 
   
  		  
	this->qvtkWidgetMain->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetMain->GetRenderWindow()->GetInteractor()->Render();

	this->qvtkWidgetMain->GetRenderWindow()->AddRenderer(mainRenderer);
	this->qvtkWidgetMain->GetRenderWindow()->Render();
	this->qvtkWidgetMain->update();	
 
 cout<<endl<<"The Volume is rendered!!!!"<<endl;
}
//**************************************************************************************************************************************//
// This function is called by the QT Timer. This function checks for the mouse moves and it updates the render with a cube to visualize //
// This function visualizes the left side of the renderer                                                                              //
//**************************************************************************************************************************************//
void VolVis::renderLeft()
{
	CubeSource = vtkSmartPointer<vtkCubeSource>::New();
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if(((rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3) && (rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)] != 1)) || (rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4 && (rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)))
			  {
				  rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)]=1;
				  
				  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();			  
				  CubeSource->SetCenter(i,j,k);
				  
				  CubeSource->SetXLength(3);
				  CubeSource->SetYLength(3);
				  CubeSource->SetZLength(1);
				  CubeSource->Update();
				  mapper->SetInputConnection(CubeSource->GetOutputPort());
				  actor->SetMapper(mapper);
				  actorprop_left_red = vtkSmartPointer<vtkProperty>::New();
				 if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)
					actorprop_left_red->SetColor(255,0,0);
				 else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2)
					actorprop_left_red->SetColor(0,255,0);
				 else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3)
					actorprop_left_red->SetColor(0,0,255);
				 else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4)
				 {
					actorprop_left_red->SetColor(255,255,255);
					rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] = 0;
					rasterize_array_checkleft[i+(j*x_dim)+(k*x_dim*y_dim)]=0;
				 }
				 actor->SetProperty(actorprop_left_red);
				 leftRenderer->AddActor(actor);
			  }
	//this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->ReInitialize();
	//this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->Render();

	//this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	//this->qvtkWidgetLeft->GetRenderWindow()->Render();
	this->qvtkWidgetLeft->update();
	//CubeSource->Delete();
			  
}

//**************************************************************************************************************************************//
// This function is called by the QT Timer. This function checks for the mouse moves and it updates the render with a cube to visualize //
// This function visualizes the Center renderer                                                                              //
//**************************************************************************************************************************************//


void VolVis::renderCenter()
{
	
  int resized_k,resized_j;
  std::vector<vtkSmartPointer<vtkActor> > actors;
  vtkSmartPointer<vtkCubeSource> CubeSource = vtkSmartPointer<vtkCubeSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if(((rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3) && (rasterize_array_checkcenter[i+(j*x_dim)+(k*x_dim*y_dim)] != 1))|| (rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4 && (rasterize_array_checkcenter[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)))
			  {
				    rasterize_array_checkcenter[i+(j*x_dim)+(k*x_dim*y_dim)] = 1;
					
					vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
					CubeSource->SetXLength(5);
					CubeSource->SetYLength(5);
					CubeSource->SetZLength(5);
					if(z_dim == y_dim)
					{
					      i=i;
					}	     
					else if(z_dim < y_dim)
					{
						//int split = (200-((z_dim*200)/y_dim));
						int split = y_dim - z_dim;
						split/=2;
						
						//split = (split * z_dim)/ y_dim;
						//split = y_dim - (split);
						//x := (c - a) * (z - y) / (b - a) + y
					    // cout <<" K "<<k<<endl;
					      // resized_k = (k*(200-(2*split))/z_dim) + split;
						     //resized_k = (k*200)/y_dim;
							 resized_k = k + split;
						  // cout<<"===========resized_k---------"<<k;
						   resized_j = (j);

				}
				/*else if(y_dim < z_dim)
				{
					int split = (200-((y_dim*200)/(2*y_dim)));
					resized_j = ((j+split) * (z_dim - split))/(y_dim + split);
					resized_k =k;       
				}*///cout<<"CUBE :"<<resized_k<<" "<<resized_j<<" "<<i<<endl;
					int split = (200-((z_dim*200)/y_dim))/2;
					if((resized_k<split) || (resized_k > (200-split)))
						cout<<" K : "<<k<<endl;
					CubeSource->SetCenter(resized_k,resized_j,128-i);
					CubeSource->Update();
					mapper->SetInputConnection(CubeSource->GetOutputPort());
					actor->SetMapper(mapper);
					actorprop_center_red = vtkSmartPointer<vtkProperty>::New();
					if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)
						actorprop_center_red->SetColor(255,0,0);
					else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2)
						actorprop_center_red->SetColor(0,255,0);
					else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3)
						actorprop_center_red->SetColor(0,0,255);
					else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4)
					{
						actorprop_center_red->SetColor(255,255,255);
					    rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] = 0;
						rasterize_array_checkcenter[i+(j*x_dim)+(k*x_dim*y_dim)] = 0;
					}
					actor->SetProperty(actorprop_center_red);
					centerRenderer->AddActor(actor);
			  
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->Render();
	this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
	this->qvtkWidgetCenter->GetRenderWindow()->Render();
	this->qvtkWidgetCenter->update();
	//CubeSource->Delete();
	}

}
//**************************************************************************************************************************************//
// This function is called by the QT Timer. This function checks for the mouse moves and it updates the render with a cube to visualize //
// This function visualizes the right side of the renderer                                                                              //
//**************************************************************************************************************************************//
void VolVis::renderRight()
{
	vtkSmartPointer<vtkCubeSource> CubeSource = vtkSmartPointer<vtkCubeSource>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  int resized_i,resized_k;	
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  // rasterize_array is the array to visualize the volume as red, green or blue. Rasterize_array_checkright is the array to check if cube at that voxel is already drawn.                                                                                                              
			  if (((rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2 || rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3) && (rasterize_array_checkright[i+(j*x_dim)+(k*x_dim*y_dim)] != 1))|| (rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4 && (rasterize_array_checkright[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)))
			  {
				    if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] !=0)
					{
						rasterize_array_checkright[i+(j*x_dim)+(k*x_dim*y_dim)]=1;
						
					    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
						CubeSource->SetXLength(5);
						CubeSource->SetYLength(5);
						CubeSource->SetZLength(5);
						if(z_dim == x_dim)
							{
								  //Do Nothing
						  
							}	     
						else if(z_dim < x_dim)
							{
								//int split = (200-((z_dim*200)/x_dim));
								//split/=2;
						
						//split = (split * z_dim)/ y_dim;
						//split = y_dim - (split);
						//x := (c - a) * (z - y) / (b - a) + y
					    // cout <<" K "<<k<<endl;
					      // resized_k = (k*(200-(2*split))/z_dim) + split;
						     //resized_k = k + split;
						  // cout<<"===========resized_k---------"<<k;
						   //resized_i = (i);

						   //===============
						   //int split = (200-((z_dim*200)/y_dim));
						int split = y_dim - z_dim;
						split/=2;
						
						//split = (split * z_dim)/ y_dim;
						//split = y_dim - (split);
						//x := (c - a) * (z - y) / (b - a) + y
					    // cout <<" K "<<k<<endl;
					      // resized_k = (k*(200-(2*split))/z_dim) + split;
						     //resized_k = (k*200)/y_dim;
							 resized_k = k + split;
						  // cout<<"===========resized_k---------"<<k;
						   resized_i = (i);

				//	=============
					//int split = (200-((z_dim*200)/x_dim));
					
				//	split = (split * z_dim)/ x_dim;
					//split = x_dim - (split);
					//resized_k = (k*split)/z_dim;
					//resized_i =i;
					
				}
				else if(x_dim < z_dim)
				{
					int split = (200-((x_dim*200)/(2*x_dim)));
					resized_i = ((i+split) * (z_dim - split))/(x_dim + split);
					resized_k =k;       
				}
					CubeSource->SetCenter(resized_i,resized_k,j);
					CubeSource->Update();
	 				mapper->SetInputConnection(CubeSource->GetOutputPort());
		     	    actor->SetMapper(mapper);
					
				    actorprop_left_red = vtkSmartPointer<vtkProperty>::New();
				    if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 1)
						actorprop_left_red->SetColor(255,0,0);
					else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 2)
						actorprop_left_red->SetColor(0,255,0);
					else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 3)
						actorprop_left_red->SetColor(0,0,255);
					else if(rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] == 4)
					{
						actorprop_center_red->SetColor(0,0,255);
					    rasterize_array[i+(j*x_dim)+(k*x_dim*y_dim)] =0;
						rasterize_array_checkright[i+(j*x_dim)+(k*x_dim*y_dim)] =0;
					}
					actor->SetProperty(actorprop_left_red);
					
					rightRenderer->AddActor(actor);
			  
	this->qvtkWidgetRight->GetRenderWindow()->RemoveRenderer(rightRenderer);
	this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->Render();
	this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
	this->qvtkWidgetRight->GetRenderWindow()->Render();
	this->qvtkWidgetRight->update();
					}}
}

/*void VolVis::setSliceNumberMainLeft(int a)
{
	imageSliceMapperMainLeft->SetSliceNumber(a);
	this->qvtkWidgetMainCenter->update();
	//this->qvtkWidgetMain->update();
	//left_sliceNumber = a;
	VolVis::renderMainLeft();
}*/

//*****************************************************************************//
// Function called when the Left Slider is moved. The slice is been updated    //
// with the corresponding slice number. A variable is also set to update array //
//*****************************************************************************//

void VolVis::setSliceNumberLeft(int a)
{
	imageSliceMapper->SetSliceNumber(a);
	this->qvtkWidgetCenter->update();
	left_sliceNumber = a;
}

//*****************************************************************************//
// Function called when the Center Slider is moved. The slice is been updated  //
// with the corresponding slice number. A variable is also set to update array //
//*****************************************************************************//

void VolVis::setSliceNumberCenter(int a)
{
	imageSliceMapper2->SetSliceNumber(a);
	this->qvtkWidgetCenter->update();
	center_sliceNumber = a;
	cout<<a<<endl;

}

//*****************************************************************************//
// Function called when the Right Slider is moved. The slice is been updated   //
// with the corresponding slice number. A variable is also set to update array //
//*****************************************************************************//

void VolVis::setSliceNumberRight(int a)
{
	imageSliceMapper3->SetSliceNumber(a);
	this->qvtkWidgetRight->update();
	right_sliceNumber = a;

}

//*****************************************************************************//
// Function called when Predict SVM button is pressed.                         //
// This function renders the Volume in the main renderer                       //
//*****************************************************************************//

void VolVis::updateImageArrayafterTraining()
{
	cout<<"called============";
	VolVis::renderMain();
}

void VolVis::loadFromFile()
 {
	 cout<<"Clciked";
     QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open the Dataset"), "",
         tr("All Files (*)"));
     if (fileName.isEmpty())
         return;
     else {
		 loadFileName = fileName.toUtf8().constData();
		 cout<<"loadFileName :"<<loadFileName;
		 VolVis::Render();
          }
	 }

void VolVis::setPredictFile()
 {
	 
     QString fileName = QFileDialog::getOpenFileName(this,
         tr("Set the Prediction File"), "",
         tr(";All Files (*)"));
     if (fileName.isEmpty())
         return;
     else {
		 predictionFileName = fileName.toUtf8().constData();
          }
	 }

void VolVis::setIndexFile()
 {
	 cout<<"clocled";
     QString fileName = QFileDialog::getOpenFileName(this,
         tr("Set the Index File for Prediction"), "",
         tr("All Files (*)"));
     if (fileName.isEmpty())
         return;
     else {
		 indexFileName = fileName.toUtf8().constData();
          }
	 }

 
void VolVis::updateOpacity()
{
	opacityBlue=this->doubleSpinBoxBlue->value();
	opacityRed = this->doubleSpinBoxRed->value();
	opacityGreen = this->doubleSpinBoxGreen->value();
	VolVis::RenderPrediction();
}



//*****************************************************************************//
// Function called at the end.                                                 //
//*****************************************************************************//

void VolVis::slotExit() 
{
  qApp->exit();
}


//*****************************************************************************//
// Destructor is called. Mappers are set to null                               //
//*****************************************************************************//

 VolVis::~VolVis() {
 imageSliceMapper= NULL;
 imageSliceMapper2= NULL;
 imageSliceMapper3= NULL;
 imageSliceMapperMain = NULL;
 //imageSliceMapperMainLeft = NULL;
 //imageSliceMapperMainCenter = NULL;

 };
