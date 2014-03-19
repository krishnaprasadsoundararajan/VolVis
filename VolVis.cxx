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

#define PTR vtkSmartPointer

  
 int Color[3];
 int left_sliceNumber=30,center_sliceNumber=40,right_sliceNumber=60;
 int rasterize_array[128][128][94],rasterize_array_checkcenter[128][128][94],rasterize_array_checkleft[128][128][94],rasterize_array_checkright[128][128][94];
 int x_dim,y_dim,z_dim;
 
 
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper2 = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper3 = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapperMain = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageData> colorImage;

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
					rasterize_array[x_cor][y_cor][z_cor] = 1;//x+(y*x_dim)+((z)*x_dim*y_dim)
				else if(Color[1] == 255)// Green Color button pressed
					rasterize_array[x_cor][y_cor][z_cor] = 2;
				else if(Color[2] == 255)// Blue Color button pressed
					rasterize_array[x_cor][y_cor][z_cor] = 3;
				//Eraser button pressed and checks if the voxel is already colored. Need not erase when not drawn before.
				else if((Color[0] == 333) && (rasterize_array[x_cor][y_cor][z_cor] ==1 || rasterize_array[x_cor][y_cor][z_cor]==2 || rasterize_array[x_cor][y_cor][z_cor]==3))
					rasterize_array[x_cor][y_cor][z_cor] = 4;
					
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
							   z_cor = (x - split) * z_dim / ( 200 - (2*split));
							   y_cor = (y_dim*y)/200;
					           x_cor = center_sliceNumber;
					           
							   if(Color[0] == 255)
									rasterize_array[x_cor][y_cor][z_cor] = 1;
							   else if(Color[1] == 255)
									rasterize_array[x_cor][y_cor][z_cor] = 2;
							   else if(Color[2] == 255)
									rasterize_array[x_cor][y_cor][z_cor] = 3;
							   else if((Color[0] == 333) && (rasterize_array[x_cor][y_cor][z_cor]==1 || rasterize_array[x_cor][y_cor][z_cor]==2 || rasterize_array[x_cor][y_cor][z_cor]==3))
									rasterize_array[x_cor][y_cor][z_cor] = 4;
							  // cout<<"CENTER Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor][y_cor][z_cor]<<endl;
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
					
					int split = (200-((z_dim*200)/x_dim))/2;
					//cout<<"SPLIT " <<split<<endl;
					       if((y>split)&&(y<(200-split)))
						   {
							   //z_cor = (x * z_dim)/(200-(2*split));
							   z_cor = (y - split) * z_dim / ( 200 - (2*split));
							   x_cor = (x_dim*x)/200;
					           y_cor = right_sliceNumber;
					            
							   if(Color[0] == 255)
									rasterize_array[x_cor][y_cor][z_cor] = 1;
							   else if(Color[1] == 255)
									rasterize_array[x_cor][y_cor][z_cor] = 2;
							   else if(Color[2] == 255)
									rasterize_array[x_cor][y_cor][z_cor] = 3;
							   else if((Color[0] == 333))
									rasterize_array[x_cor][y_cor][z_cor] = 4;
							   //cout<<"RIGHT Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor][y_cor][z_cor]<<endl;
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
					rasterize_array[x_cor][y_cor][z_cor] = 1;
				}
				//cout<<"Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor][y_cor][z_cor]<<endl;
             }
		 
		 
    	 }
 
};

 
vtkStandardNewMacro(MouseInteractorStyle4);
vtkStandardNewMacro(MouseInteractorStyleCenter4);
vtkStandardNewMacro(MouseInteractorStyleRight4);

// Constructor

VolVis::VolVis() 
{
   this->setupUi(this);
   //***********************************************************************//
   //Initializations                                                        //
   //***********************************************************************//

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(600, 600);
	
	
	PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	
	//Need to make this hard coded to runtime input
	source->SetFileName("I:/assignment-06-octree-solution (1)/assignment-06-octree-solution/headsq-half.vti");
	source->Update();
	
	
	vtkSmartPointer<vtkImageData> colorImage;
	colorImage = source->GetOutput();
	colorImage->UpdateInformation();

	int extent[6];
	colorImage->GetExtent(extent);
 
    double origin[3];
    colorImage->GetOrigin(origin);
 
    double spacing[3];
    colorImage->GetSpacing(spacing);
	
	//Setting the Dimensions from the image read
	x_dim = colorImage->GetDimensions()[0], y_dim = colorImage->GetDimensions()[1], z_dim = colorImage->GetDimensions()[2];
	
	//Initalize arrays to check for rendering
	//rasterize_array = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	//rasterize_array_checkcenter = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	//rasterize_array_checkleft = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	//rasterize_array_checkright = new int[colorImage->GetDimensions()[0]*colorImage->GetDimensions()[1]*colorImage->GetDimensions()[2]];
	
	



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

  
  vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
  imageSlice->SetMapper(imageSliceMapper);

  imageSliceMapper->SliceFacesCameraOn();
  imageSliceMapper->SetSliceNumber(30);

  leftRenderer = vtkSmartPointer<vtkRenderer>::New();
  double leftViewport[4] = {0.0, 0.0, 0.33, 0.4};
  double centerViewport[4] ={0.33, 0.0, 0.66, 0.4};
  double rightViewport[4] = {0.66, 0.0, 1.0, 0.4};
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
  //==========================================================================================================================


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
  
  vtkSmartPointer<vtkImageSlice> imageSlice2 = vtkSmartPointer<vtkImageSlice>::New();
  imageSlice2->SetMapper(imageSliceMapper2);

  imageSliceMapper2->SliceFacesCameraOn();
  imageSliceMapper2->SetSliceNumber(30);
  
  imageSlice2->SetOrigin(x_dim/2,y_dim/2,z_dim/2);
  imageSlice2->RotateY(90);
  imageSliceMapper2->SetOrientationToX();

  centerRenderer = vtkSmartPointer<vtkRenderer>::New();
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
  
  vtkSmartPointer<vtkImageSlice> imageSlice3 = vtkSmartPointer<vtkImageSlice>::New();
  imageSlice3->SetMapper(imageSliceMapper3);
	
  imageSliceMapper3->SliceFacesCameraOn();
  imageSliceMapper3->SetSliceNumber(30);

  imageSlice3->SetOrigin(x_dim/2,y_dim/2,z_dim/2);
  imageSlice3->RotateX(-90);

  rightRenderer = vtkSmartPointer<vtkRenderer>::New();
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
 //==========================================================================================================================


   mainRenderer = vtkSmartPointer<vtkRenderer>::New();
   mainRenderer->SetBackground(.6, .5, .4);    
 
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
   
  
 
  
  
    this->pushButtonBlue->setAutoFillBackground(true);	
	this->pushButtonRed->setAutoFillBackground(true);
	this->pushButtonGreen->setAutoFillBackground(true);
	
	this->pushButtonBlue->setStyleSheet("background-color: rgb(0, 0, 255); color: rgb(255, 255, 255)");
	this->pushButtonGreen->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(255, 255, 255)");
	this->pushButtonRed->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
	
	this->horizontalSliderLeft->setMinimum(0);
	this->horizontalSliderLeft->setMaximum(z_dim - 1);
   
	this->horizontalSliderCenter->setMinimum(0);
	this->horizontalSliderCenter->setMaximum(x_dim - 1);

	this->horizontalSliderRight->setMinimum(0);
	this->horizontalSliderRight->setMaximum(y_dim - 1);
	
	this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style1);
	this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style2);
	this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
	
	this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style3);
	this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
  
  	 
    // Set up action signals and slots
	this->horizontalSliderLeft->connect(this->horizontalSliderLeft,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberLeft(int))); 
	this->horizontalSliderCenter->connect(this->horizontalSliderCenter,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberCenter(int))); 
	this->horizontalSliderRight->connect(this->horizontalSliderRight,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberRight(int))); 
  
  
    this->pushButtonBlue->connect(this->pushButtonBlue,SIGNAL(clicked()),this,SLOT(renderBlueColor()));
	this->pushButtonGreen->connect(this->pushButtonGreen,SIGNAL(clicked()),this,SLOT(renderGreenColor()));
	this->pushButtonRed->connect(this->pushButtonRed,SIGNAL(clicked()),this,SLOT(renderRedColor()));
	this->pushButtonEraser->connect(this->pushButtonEraser,SIGNAL(clicked()),this,SLOT(renderEraser()));
	this->TrainSVM->connect(this->TrainSVM,SIGNAL(clicked()),this,SLOT(trainSVM()));
	this->PredictSVM->connect(this->PredictSVM,SIGNAL(clicked()),this,SLOT(updateImageArrayafterTraining()));
	
	//connect(timer, SIGNAL(timeout()), this, SLOT(renderMain()));
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(renderLeft()));
	connect(timer, SIGNAL(timeout()), this, SLOT(renderCenter()));
	connect(timer, SIGNAL(timeout()), this, SLOT(renderRight()));
    
	timer->start(20);
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
void VolVis::trainSVM()
{
	FeatureVector obj;
	obj.CreateTestTrainData(128,128,94,&rasterize_array[0][0][0]);
	
}


//**************************************************************************************************************************************//
// This function is called by the a function when Predict SVM button is pressed.                                                        //
// This function visualizes the Main renderer window reading the values from the Predicted output of the SVM                            //
//**************************************************************************************************************************************//

void VolVis::renderMain()
{
	PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	source->SetFileName("I:/assignment-06-octree-solution (1)/assignment-06-octree-solution/headsq-half.vti");
	source->Update();
	//PTR<vtkImageData>
	vtkSmartPointer<vtkImageData> colorImage;
		colorImage = source->GetOutput();
	colorImage->UpdateInformation();
	int one=0,two=0,three =0;
	 vtkSmartPointer<vtkLookupTable> lut =
    vtkSmartPointer<vtkLookupTable>::New();
  //int tableSize = std::max(resolution*resolution + 1, 10);
  lut->SetNumberOfTableValues(128*128*94+1);
  //lut->SetNumberOfColors(4);
  lut->Build();
	//============================================
	cout<<"called============";
 	int *v = new int[128*128*94];
	int *pv = new int[128*128*94];
    int num = 0,prediction = 0;
	 ifstream fin("E:/SemesterIII/Thesis/libsvm-3.17/windows/Index.dat");
	 ifstream fin1("E:/SemesterIII/Thesis/libsvm-3.17/windows/PredictedOutput");
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
		int prod =(x_dim*y_dim);
		int x1 = num % x_dim;
		int y1 = (num%prod)/x_dim;
		int z1 = num/prod;
		z1++;
		if(prediction==1)
		{
			colorImage->SetScalarComponentFromDouble(x1,y1,z1,0,150);
			one++;
		}
		else if(prediction==2)
		{
			colorImage->SetScalarComponentFromDouble(x1,y1,z1,0,550);
		    two++;
		}
		else if(prediction==3)
		{
			colorImage->SetScalarComponentFromDouble(x1,y1,z1,0,1200);
		    three++;
		}

		rasterize_array[x1][y1][z1] = prediction+5;
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

    delete [] v;
	cout<<"The values are read successfully predicted by the SVM and the scalar values are set accordinly"<<endl;
	//--------------------------------------------
   vtkSmartPointer<vtkVolumeRayCastCompositeFunction> rayCastFunction =
      vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
  
     vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper =
       vtkSmartPointer<vtkVolumeRayCastMapper>::New();
     volumeMapper->SetInputConnection(colorImage->GetProducerPort());
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
	  volumeColor->AddRGBPoint(100,  1.0, 0, 0);
	  volumeColor->AddRGBPoint(200,  1.0, 0, 0);
      volumeColor->AddRGBPoint(500, 0, 1, 0);
	  volumeColor->AddRGBPoint(600, 0, 1, 0);
	  volumeColor->AddRGBPoint(1000, 0, 0, 1);
	  volumeColor->AddRGBPoint(1400, 0, 0, 1);
	  
    // The opacity transfer function is used to control the opacity
    // of different tissue types.
    vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity =
       vtkSmartPointer<vtkPiecewiseFunction>::New();
     volumeScalarOpacity->AddPoint(0,    0.0);
	 volumeScalarOpacity->AddPoint(99,    0.0);
     volumeScalarOpacity->AddPoint(100,  0.5);
	 volumeScalarOpacity->AddPoint(200,  0.5);
     volumeScalarOpacity->AddPoint(500, 0.7);
	 volumeScalarOpacity->AddPoint(600, 0.7);
     volumeScalarOpacity->AddPoint(1000, 0.2);
	 volumeScalarOpacity->AddPoint(1400, 0.2);
	 volumeScalarOpacity->AddPoint(1401, 0);
   
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
    //volumeProperty->SetInterpolationTypeToLinear();
	//volumeProperty->SetInterpolationTypeToNearest();
    //volumeProperty->ShadeOn();
    //volumeProperty->SetAmbient(0.4);
    //volumeProperty->SetDiffuse(0.6);
    //volumeProperty->SetSpecular(0.2);
    // The vtkVolume is a vtkProp3D (like a vtkActor) and controls the position
    /// and orientation of the volume in world coordinates.
    vtkSmartPointer<vtkVolume> volume =
     vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
 
    // Finally, add the volume to the renderer
    mainRenderer->AddViewProp(volume);
 

	//vtkSmartPointer<vtkImageMapToColors> firstColorMapper = 
      //vtkSmartPointer<vtkImageMapToColors>::New();
	//firstColorMapper->SetInputConnection( colorImage->GetProducerPort() );
	//lut->SetRange( 0.0, 255.0 );
    //lut->SetHueRange( 0.0, 0.1 );
    //lut->SetValueRange( 0.4, 0.8 );
    lut->Build();
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
	
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if(((rasterize_array[i][j][k] == 1 || rasterize_array[i][j][k] == 2 || rasterize_array[i][j][k] == 3) && (rasterize_array_checkleft[i][j][k] != 1)) || (rasterize_array[i][j][k] == 4 && (rasterize_array_checkleft[i][j][k] == 1)))
			  {
				  rasterize_array_checkleft[i][j][k]=1;
				  vtkSmartPointer<vtkCubeSource> CubeSource = vtkSmartPointer<vtkCubeSource>::New();
				  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
				  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();			  
				  CubeSource->SetCenter(i,j,k);
				 
				  CubeSource->SetXLength(5);
				  CubeSource->SetYLength(5);
				  CubeSource->SetZLength(5);
				  CubeSource->Update();
				  mapper->SetInputConnection(CubeSource->GetOutputPort());
				  actor->SetMapper(mapper);
				  actorprop_left_red = vtkSmartPointer<vtkProperty>::New();
				 if(rasterize_array[i][j][k] == 1)
					actorprop_left_red->SetColor(255,0,0);
				 else if(rasterize_array[i][j][k] == 2)
					actorprop_left_red->SetColor(0,255,0);
				 else if(rasterize_array[i][j][k] == 3)
					actorprop_left_red->SetColor(0,0,255);
				 else if(rasterize_array[i][j][k] == 4)
				 {
					actorprop_left_red->SetColor(255,255,255);
					rasterize_array[i][j][k] = 0;
					rasterize_array_checkleft[i][j][k]=0;
				 }
				 actor->SetProperty(actorprop_left_red);
				 leftRenderer->AddActor(actor);
			  }
	this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->Render();

	this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	this->qvtkWidgetLeft->GetRenderWindow()->Render();
	this->qvtkWidgetLeft->update();
			  
}

//**************************************************************************************************************************************//
// This function is called by the QT Timer. This function checks for the mouse moves and it updates the render with a cube to visualize //
// This function visualizes the Center renderer                                                                              //
//**************************************************************************************************************************************//


void VolVis::renderCenter()
{
	
  int resized_k,resized_j;
  std::vector<vtkSmartPointer<vtkActor> > actors;
  
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if(((rasterize_array[i][j][k] == 1 || rasterize_array[i][j][k] == 2 || rasterize_array[i][j][k] == 3) && (rasterize_array_checkcenter[i][j][k] != 1))|| (rasterize_array[i][j][k] == 4 && (rasterize_array_checkcenter[i][j][k] == 1)))
			  {
				    rasterize_array_checkcenter[i][j][k] = 1;
					vtkSmartPointer<vtkCubeSource> CubeSource = vtkSmartPointer<vtkCubeSource>::New();
					vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
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
						int split = (200-((z_dim*200)/y_dim));
						split/=2;
						
						//split = (split * z_dim)/ y_dim;
						//split = y_dim - (split);
						//x := (c - a) * (z - y) / (b - a) + y
					    // cout <<" K "<<k<<endl;
					      // resized_k = (k*(200-(2*split))/z_dim) + split;
						     resized_k = k+split;
						  // cout<<"===========resized_k---------"<<k;
						   resized_j = (j);

				}
				/*else if(y_dim < z_dim)
				{
					int split = (200-((y_dim*200)/(2*y_dim)));
					resized_j = ((j+split) * (z_dim - split))/(y_dim + split);
					resized_k =k;       
				}*///cout<<"CUBE :"<<resized_k<<" "<<resized_j<<" "<<i<<endl;
					CubeSource->SetCenter(resized_k,resized_j,110-i);
					CubeSource->Update();
					mapper->SetInputConnection(CubeSource->GetOutputPort());
					actor->SetMapper(mapper);
					actorprop_center_red = vtkSmartPointer<vtkProperty>::New();
					if(rasterize_array[i][j][k] == 1)
						actorprop_center_red->SetColor(255,0,0);
					else if(rasterize_array[i][j][k] == 2)
						actorprop_center_red->SetColor(0,255,0);
					else if(rasterize_array[i][j][k] == 3)
						actorprop_center_red->SetColor(0,0,255);
					else if(rasterize_array[i][j][k] == 4)
					{
						actorprop_center_red->SetColor(255,255,255);
					    rasterize_array[i][j][k] = 0;
						rasterize_array_checkcenter[i][j][k] = 0;
					}
					actor->SetProperty(actorprop_center_red);
					centerRenderer->AddActor(actor);
			  
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->Render();
	this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
	this->qvtkWidgetCenter->GetRenderWindow()->Render();
	this->qvtkWidgetCenter->update();
	}

}
//**************************************************************************************************************************************//
// This function is called by the QT Timer. This function checks for the mouse moves and it updates the render with a cube to visualize //
// This function visualizes the right side of the renderer                                                                              //
//**************************************************************************************************************************************//
void VolVis::renderRight()
{
  int resized_i,resized_k;	
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  // rasterize_array is the array to visualize the volume as red, green or blue. Rasterize_array_checkright is the array to check if cube at that voxel is already drawn.                                                                                                              
			  if (((rasterize_array[i][j][k] == 1 || rasterize_array[i][j][k] == 2 || rasterize_array[i][j][k] == 3) && (rasterize_array_checkright[i][j][k] != 1))|| (rasterize_array[i][j][k] == 4 && (rasterize_array_checkright[i][j][k] == 1)))
			  {
				    if(rasterize_array[i][j][k] !=0)
					{
						rasterize_array_checkright[i][j][k]=1;
						vtkSmartPointer<vtkCubeSource> CubeSource = vtkSmartPointer<vtkCubeSource>::New();
					    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
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
								int split = (200-((z_dim*200)/x_dim));
								split/=2;
						
						//split = (split * z_dim)/ y_dim;
						//split = y_dim - (split);
						//x := (c - a) * (z - y) / (b - a) + y
					    // cout <<" K "<<k<<endl;
					      // resized_k = (k*(200-(2*split))/z_dim) + split;
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
				    if(rasterize_array[i][j][k] == 1)
						actorprop_left_red->SetColor(255,0,0);
					else if(rasterize_array[i][j][k] == 2)
						actorprop_left_red->SetColor(0,255,0);
					else if(rasterize_array[i][j][k] == 3)
						actorprop_left_red->SetColor(0,0,255);
					else if(rasterize_array[i][j][k] == 4)
					{
						actorprop_center_red->SetColor(0,0,255);
					    rasterize_array[i][j][k] =0;
						rasterize_array_checkright[i][j][k] =0;
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
 };
