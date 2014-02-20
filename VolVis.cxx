#include "VolVis.h"
#include <FeatureVector.h>
#include <vtkAssignAttribute.h>
#include <vtkGradientFilter.h>
#include <vtkPropCollection.h>
#include <vtkCubeSource.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCubeSource.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>
#include <vtkVersion.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkXMLImageDataReader.h>
#include <vtkRenderer.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageMapper3D.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkImageSlice.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkLine.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkImageProperty.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkMath.h>
#include <vtkCallbackCommand.h>
#include <qtimer.h>
#include <math.h>
#define PTR vtkSmartPointer

  
 int Color[3];
 int mousedown =0,right_button_down,left_sliceNumber=30,center_sliceNumber=40,right_sliceNumber=60;
 int rasterize_array[128][128][94],rasterize_array_checkcenter[128][128][94],rasterize_array_checkleft[128][128][94],rasterize_array_checkright[128][128][94];
 int x_dim = 128,y_dim=128,z_dim=94;
 
 
  vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper2 = vtkSmartPointer<vtkImageSliceMapper>::New();
 vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper3 = vtkSmartPointer<vtkImageSliceMapper>::New();
 
class MouseInteractorStyle4 : public vtkInteractorStyleImage
{
  public:
    static MouseInteractorStyle4* New();
	vtkTypeMacro(MouseInteractorStyle4, vtkInteractorStyleImage);
    int check;
	virtual void OnRightButtonDown()
		{
			right_button_down = 1;
			this->StartWindowLevel();
		}
	virtual void OnRightButtonUp()
		{
			right_button_down =0;
			this->EndWindowLevel();
	}
	virtual void OnLeftButtonUp()
	{ 
		mousedown = 0;
	}

    virtual void OnLeftButtonDown() 
    {		
		mousedown = 1;
		this->EndWindowLevel();
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
		 //cout<<"X: "<<x << " Y : " << y <<" Z "<<z<<endl;
		if(right_button_down==0)
			this->EndWindowLevel(); 
		if(mousedown==1)
			{
				if(x_dim == y_dim)
				{
					      x_cor = (x_dim*x)/200;
				          y_cor = (y_dim*y)/200;
						  z_cor = left_sliceNumber;
						  //cout<<"LEFT SLICXE NUMBER :"<< z_cor<<endl;
				}	     
				else if(x_dim < y_dim)
				{
					
				}
				else if(y_dim < x_dim)
				{
	
				}
				if(Color[0] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 1;
				else if(Color[1] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 2;
				else if(Color[2] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 3;
				else 
					rasterize_array[x_cor][y_cor][z_cor] = 0;
					
				cout<<"Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor][y_cor][z_cor]<<endl; 
             }
		 
		 
    	 }
     	
};

class MouseInteractorStyleCenter4 : public vtkInteractorStyleImage
{
  public:
    static MouseInteractorStyleCenter4* New();
    vtkTypeMacro(MouseInteractorStyleCenter4, vtkInteractorStyleImage);
    virtual void OnRightButtonDown()
	{
		right_button_down =1;
		this->StartWindowLevel();
	}
	virtual void OnRightButtonUp()
	{
		right_button_down =0;
		this->EndWindowLevel();
	}
	
	virtual void OnLeftButtonUp()
	{ 
		mousedown = 0;
	}
    virtual void OnLeftButtonDown() 
    {
		mousedown = 1;
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
		  if(right_button_down==0)
			this->EndWindowLevel(); 
		  if(mousedown==1 )
			{
			//cout<<"x : "<<x <<" Y :"<<y<<" Z :"<<z<<endl;			  
						
				if(z_dim == y_dim)
				{
					      z_cor = (z_dim*x)/200;
				          y_cor = (y_dim*y)/200;
						  x_cor = center_sliceNumber;
				}	     
				else if(z_dim < y_dim)
				{
					
					int split = (200-((z_dim*200)/y_dim))/2;
					cout<<"SPLIT " <<split<<endl;
					       if(0 < x < (200-(2*split)))
							   //z_cor = (z_dim*(x-split))/(200 - (2*split));
							   z_cor = (x * z_dim)/(200-(2*split));
							   y_cor = (y_dim*y)/200;
					           x_cor = center_sliceNumber;
					           rasterize_array[x_cor][y_cor][z_cor] = 1;
				}
				else if(y_dim < z_dim)
				{
				
				}
				if(Color[0] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 1;
				else if(Color[1] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 2;
				else if(Color[2] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 3;
				else if(rasterize_array[x_cor][y_cor][z_cor]!=0)
					rasterize_array[x_cor][y_cor][z_cor] = 0;
				cout<<"Rasterize_array ["<<x_cor<<"]["<<y_cor<<"]["<<z_cor<<"]"<<rasterize_array[x_cor][y_cor][z_cor]<<endl;
             }
    	 }
};


 class MouseInteractorStyleRight4 : public vtkInteractorStyleImage
{
  public:
    static MouseInteractorStyleRight4* New();
    vtkTypeMacro(MouseInteractorStyleRight4, vtkInteractorStyleImage);
    int check;
	virtual void OnRightButtonDown()
	{
		right_button_down =1;
		//mousedown=0;
		this->StartWindowLevel();
	}
	virtual void OnRightButtonUp()
	{
		right_button_down =0;
		this->EndWindowLevel();
	}
	virtual void OnLeftButtonUp()
	{ 
		mousedown = 0;
    }
    virtual void OnLeftButtonDown() 
    {
		mousedown = 1;
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
		 int z= this->Interactor->GetEventPosition()[2];
	   	cout<<"x : "<<x <<" Y :"<<y<<" Z :"<<z<<endl;
		  if(right_button_down == 0)
			  this->EndWindowLevel();
		  if(mousedown==1)
			{
				 //cout<<"X: "<<x << " Y : " << y <<" Z "<<z<<endl;
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
					       if(split < y < (200-split))
							   z_cor = (z_dim*(y-split))/(200 - (2*split));
					x_cor = (x_dim*x)/200;
					y_cor = right_sliceNumber;
					rasterize_array[x_cor][y_cor][z_cor] = 1;
				}
				else if(x_dim < z_dim)
				{
					int split = (200-((x_dim*200)/(2*x_dim)));
					if(split < x < (200-split))
							   x_cor = (x_dim*(x-split))/(200 - (2*split));
					z_cor = (z_dim*x)/200;
					y_cor = right_sliceNumber;
					rasterize_array[x_cor][y_cor][z_cor] = 1;
				}
				if(Color[0] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 1;
				else if(Color[1] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 2;
				else if(Color[2] == 255)
					rasterize_array[x_cor][y_cor][z_cor] = 3;
				else if(rasterize_array[x_cor][y_cor][z_cor]!=0)
					rasterize_array[x_cor][y_cor][z_cor] = 0;
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
	PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	source->SetFileName("I:/assignment-06-octree-solution (1)/assignment-06-octree-solution/headsq-half.vti");
	source->Update();
	PTR<vtkImageData> colorImage = source->GetOutput();
	colorImage->UpdateInformation();
	

	//Image Slice Mapper. Three mappers for three orthogonal views
	this->pushButtonBlue->setAutoFillBackground(true);	
	this->pushButtonRed->setAutoFillBackground(true);
	this->pushButtonGreen->setAutoFillBackground(true);
	
	this->pushButtonBlue->setStyleSheet("background-color: rgb(0, 0, 255); color: rgb(255, 255, 255)");
	this->pushButtonGreen->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(255, 255, 255)");
	this->pushButtonRed->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
	imageSliceMapper->SetInputConnection(colorImage->GetProducerPort());
    imageSliceMapper2->SetInputConnection(colorImage->GetProducerPort());
	imageSliceMapper3->SetInputConnection(colorImage->GetProducerPort());
	
	

	//Image Slice Actor  
    vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice->SetMapper(imageSliceMapper);
	vtkSmartPointer<vtkImageSlice> imageSlice2 = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice2->SetMapper(imageSliceMapper2);
	vtkSmartPointer<vtkImageSlice> imageSlice3 = vtkSmartPointer<vtkImageSlice>::New();
    imageSlice3->SetMapper(imageSliceMapper3);
	imageSliceMapper->SliceFacesCameraOn();
	imageSliceMapper->SetSliceNumber(30);
	//imageSliceMapper->BorderOff();
	//imageSliceMapper->BackgroundOn();
	imageSliceMapper2->SliceFacesCameraOn();
	
	//imageSliceMapper2->SliceAtFocalPointOn();
	//imageSliceMapper2->BorderOff();

	
	imageSliceMapper3->SliceFacesCameraOn();
	
	//imageSliceMapper3->SliceAtFocalPointOn();
	//imageSliceMapper3->BorderOff();
	imageSlice2->SetOrigin(x_dim/2,y_dim/2,z_dim/2);
	imageSlice2->RotateY(90);
	//imageSlice->RotateY(-90);
	imageSlice3->SetOrigin(x_dim/2,y_dim/2,z_dim/2);
	imageSlice3->RotateX(-90);
	
	imageSliceMapper2->SetSliceNumber(40);
	imageSliceMapper2->SetOrientationToX();
	imageSliceMapper3->SetSliceNumber(60);
	
	cout<<" #1 X :"<<imageSlice->GetCenter()[0]<<" #1 y :"<<imageSlice->GetCenter()[0]<<" #1 z :"<<imageSlice->GetCenter()[2]<<endl;
	cout<<" #2 X :"<<imageSlice2->GetCenter()[0]<<" #2 y :"<<imageSlice2->GetCenter()[0]<<" #2 z :"<<imageSlice2->GetCenter()[2]<<endl;
	cout<<" #3 X :"<<imageSlice3->GetCenter()[0]<<" #3 y :"<<imageSlice3->GetCenter()[0]<<" #3 z :"<<imageSlice3->GetCenter()[2]<<endl;
	
	this->horizontalSliderLeft->setMinimum(0);
	this->horizontalSliderLeft->setMaximum(z_dim - 1);
   
	this->horizontalSliderCenter->setMinimum(0);
	this->horizontalSliderCenter->setMaximum(x_dim - 1);

	this->horizontalSliderRight->setMinimum(0);
	this->horizontalSliderRight->setMaximum(y_dim - 1);
	//Setup render window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(600, 600);
	
 
   vtkSmartPointer<MouseInteractorStyle4> style1 = vtkSmartPointer<MouseInteractorStyle4>::New();
   vtkSmartPointer<MouseInteractorStyleCenter4> style2 = vtkSmartPointer<MouseInteractorStyleCenter4>::New();
   vtkSmartPointer<MouseInteractorStyleRight4> style3 = vtkSmartPointer<MouseInteractorStyleRight4>::New();
   
   //=====================
   int extent[6];
   colorImage->GetExtent(extent);
 
  double origin[3];
  colorImage->GetOrigin(origin);
 
  double spacing[3];
  colorImage->GetSpacing(spacing);
 
  

  // Render and start interaction
 leftRenderer = vtkSmartPointer<vtkRenderer>::New();
  double leftViewport[4] = {0.0, 0.0, 0.33, 0.4};
  double centerViewport[4] ={0.33, 0.0, 0.66, 0.4};
  double rightViewport[4] = {0.66, 0.0, 1.0, 0.4};
  leftRenderer->AddActor(imageSlice);
  leftRenderer->ResetCamera();//----------------------
  
  // Setup both renderers
  
  renderWindow->AddRenderer(leftRenderer);
  leftRenderer->SetBackground(.6, .5, .4);  
  
  centerRenderer = vtkSmartPointer<vtkRenderer>::New();
  centerRenderer->AddActor(imageSlice2); 
  centerRenderer->ResetCamera();
  
  renderWindow->AddRenderer(centerRenderer);
  centerRenderer->SetBackground(.4, .2, .6); /* vtkSmartPointer<vtkGradientFilter> gradients =
    vtkSmartPointer<vtkGradientFilter>::New();
  gradients->SetInputConnection(colorImage->GetProducerPort());

  vtkSmartPointer<vtkAssignAttribute> vectors =
    vtkSmartPointer<vtkAssignAttribute>::New();
  vectors->SetInputConnection(gradients->GetOutputPort());
  vectors->Assign("Gradients", vtkDataSetAttributes::VECTORS,
                  vtkAssignAttribute::POINT_DATA);*/
  double g[3];
  g[0] = g[1] = g[2] =0;
  //vtkSmartPointer<vtkDataArray> inScalars = vtkSmartPointer<vtkDataArray>::New();
  //double *inScalars;
  //inScalars->Initialize();
  int i[3]={10,20,30};
  //vtkIdType ij=0;
  //inScalars = static_cast<double*>(colorImage->GetScalarPointer());
  //cout<<endl<<"INSCALAR"<<inScalars<<endl;
  double sp = colorImage->GetScalarComponentAsFloat(50,20,30,0); // s->GetComponent(i+1 + j*dims[0] + k*ijsize,0);
   double sm = colorImage->GetScalarComponentAsDouble(0,0,1,0);//s->GetComponent(i-1 + j*dims[0] + k*ijsize,0);
   //double space[3];
   //colorImage->GetSpacing(space);
  // cout<<"SP "<<sp<<" SM : "<<sm<<endl;
   //g[0] = 0.5 * (sm - sp) / space[0];
  //for(int x = 1; x < x_dim; x++)
	//   for(int y = 1; y < y_dim; y++)
		//  for(int z = 2; z < z_dim; z++)
		  // {
			//   GetPointGradient(x,y,z,colorImage);
			  // cout<<"GRADIENT"<<g[0]<<" " <<g[1]<<" "<<g[2]<<endl;
		//   }
 
   //colorImage->GetVoxelGradient(10,20,30,inScalars,normal);
  //colorImage->GetPointGradient(1,2,3,colorImage->GetScalarPointer(),g);
  //cout<<"component"<<inScal)ars->GetComponent(ij,0);
  

  //=============================================
  
  rightRenderer = vtkSmartPointer<vtkRenderer>::New();
  rightRenderer->AddActor(imageSlice3);
  rightRenderer->ResetCamera();
  renderWindow->AddRenderer(rightRenderer);
  rightRenderer->SetBackground(.4, .5, .6);  
  //================================================TEST?????

  // Add the sphere to the left and the cube to the right
  
  //===========================================
  vtkCamera* leftCamera = leftRenderer->GetActiveCamera();
  leftCamera->ParallelProjectionOn();
  //leftRenderer->SetBackground(1,0,0);
 
  float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
  float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
//  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
  float yd = (extent[3] - extent[2] + 1)*spacing[1];
 
  float d = leftCamera->GetDistance();
  leftCamera->SetParallelScale(0.5f*static_cast<float>(yd));
  cout<<"Parallel Scakle : "<<leftCamera->GetParallelScale();
  leftCamera->SetFocalPoint(xc,yc,0.0);
  leftCamera->SetPosition(xc,yc,+d);
 //===================
 //===========================================
  vtkCamera* centerCamera = centerRenderer->GetActiveCamera();
  centerCamera->ParallelProjectionOn();
  float zcC = origin[2] + 0.5*(extent[4] + extent[5])*spacing[2];
  float ycC = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
  float ydC = (extent[1] - extent[0] + 1)*spacing[0];
 
  float dC = centerCamera->GetDistance();
  cout<<"dc : "<<dC<<endl;
  cout<<"zcC : "<<zcC<<endl;
  cout<<"ycC : "<<ycC<<endl;
  cout<<"ydC : "<<ydC<<endl;
  centerCamera->SetParallelScale(0.5f*static_cast<float>(yd));
  cout<<"Parallel Scakle Center: "<<centerCamera->GetParallelScale();
  centerCamera->SetFocalPoint(zcC,ycC,0.0);
  centerCamera->SetPosition(zcC,ycC,+dC);
  //centerCamera->SetFocalPoint(xc,yc,0.0);
  //centerCamera->SetPosition(xc,yc,+d);
 //centerCamera->SetFocalPoint(0,ycC,zcC);
 //centerCamera->SetPosition(+dC,ycC,zcC); 
 //===================
 //===========================================
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
 //===================

  this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style1);
  this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
  this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style2);
  this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
  this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style3);
  this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
  
  	 
  //cout<<"AFTER"<<endl;
    // Set up action signals and slots
  //this->qvtkWidgetLeft->connect(this->qvtkWidgetLeft,SIGNAL(mouseMoveEvent(QMouseEvent *event)),this,SLOT(mousemove(QMouseEvent *event)));
  this->horizontalSliderLeft->connect(this->horizontalSliderLeft,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberLeft(int))); 
  this->horizontalSliderCenter->connect(this->horizontalSliderCenter,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberCenter(int))); 
  this->horizontalSliderRight->connect(this->horizontalSliderRight,SIGNAL(valueChanged(int)),this,SLOT(setSliceNumberRight(int))); 
  this->clearLeft->connect(this->clearLeft,SIGNAL(clicked()),this,SLOT(renderLeft()));
  this->clearRight->connect(this->clearRight,SIGNAL(clicked()),this,SLOT(renderRight()));
  this->clearCenter->connect(this->clearCenter,SIGNAL(clicked()),this,SLOT(clearcenter()));

  //this->pushButtonCenter->connect(this->pushButtonCenter,SIGNAL(clicked()),this,SLOT(renderCenter()));
   //this->pushButtonRight->connect(this->pushButtonRight,SIGNAL(clicked()),this,SLOT(renderRight()));
    this->pushButtonBlue->connect(this->pushButtonBlue,SIGNAL(clicked()),this,SLOT(renderBlueColor()));
	this->pushButtonGreen->connect(this->pushButtonGreen,SIGNAL(clicked()),this,SLOT(renderGreenColor()));
	this->pushButtonRed->connect(this->pushButtonRed,SIGNAL(clicked()),this,SLOT(renderRedColor()));
	this->pushButtonEraser->connect(this->pushButtonEraser,SIGNAL(clicked()),this,SLOT(renderEraser()));
	QTimer *timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(clearleft()));
	//connect(timer, SIGNAL(timeout()), this, SLOT(clearright()));
	//connect(timer, SIGNAL(timeout()), this, SLOT(clearcenter()));
    connect(timer, SIGNAL(timeout()), this, SLOT(renderLeft()));
	connect(timer, SIGNAL(timeout()), this, SLOT(renderCenter()));
	connect(timer, SIGNAL(timeout()), this, SLOT(renderRight()));
    timer->start(20);
	FeatureVector f;
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
	Color[0] = 999;
	Color[1] = 999;
	Color[2] = 999;
	cout<<"Eraser " <<endl;
}

void VolVis::renderLeft()
{
	
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if((rasterize_array[i][j][k] == 1 || rasterize_array[i][j][k] == 2 || rasterize_array[i][j][k] == 3) && (rasterize_array_checkleft[i][j][k] != 1))
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
				 actor->SetProperty(actorprop_left_red);
				 leftRenderer->AddActor(actor);
			  }
	this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->Render();

	this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	this->qvtkWidgetLeft->GetRenderWindow()->Render();
	this->qvtkWidgetLeft->update();
			  
}
void VolVis::renderCenter()
{
	
  int resized_k,resized_j;
  std::vector<vtkSmartPointer<vtkActor> > actors;
  
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if((rasterize_array[i][j][k] == 1 || rasterize_array[i][j][k] == 2 || rasterize_array[i][j][k] == 3) && (rasterize_array_checkcenter[i][j][k] != 1))
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
					
					split = (split * z_dim)/ y_dim;
					split = y_dim - (split);
					//x := (c - a) * (z - y) / (b - a) + y
					     // cout <<" K "<<k<<endl;
					       resized_k = (k*split)/z_dim;
						   //cout<<"resized_k"<<k;
						   resized_j = (j);

				}
				/*else if(y_dim < z_dim)
				{
					int split = (200-((y_dim*200)/(2*y_dim)));
					resized_j = ((j+split) * (z_dim - split))/(y_dim + split);
					resized_k =k;       
				}*/ cout<<"CUBE :"<<resized_k<<" "<<resized_j<<" "<<i<<endl;
					CubeSource->SetCenter(resized_k,resized_j,i);
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
					actor->SetProperty(actorprop_center_red);
					centerRenderer->AddActor(actor);
			  
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetCenter->GetRenderWindow()->GetInteractor()->Render();
	this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
	this->qvtkWidgetCenter->GetRenderWindow()->Render();
	this->qvtkWidgetCenter->update();
	}

}

void VolVis::renderRight()
{
  int resized_i,resized_k;	
  for( int i =0 ;i< x_dim;i++)
	  for( int j =0 ;j< y_dim;j++)
		  for( int k =0 ;k< z_dim;k++)
			  if ((rasterize_array[i][j][k] == 1 || rasterize_array[i][j][k] == 2 || rasterize_array[i][j][k] == 3|| rasterize_array[i][j][k] == 0) && (rasterize_array_checkright[i][j][k] != 1))
			  {
				    if(rasterize_array[i][j][k] !=0){rasterize_array_checkright[i][j][k]=1;
					
					vtkSmartPointer<vtkCubeSource> CubeSource = vtkSmartPointer<vtkCubeSource>::New();
					vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
					vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
					//CubeSource->SetRadius(5);
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
					
					split = (split * z_dim)/ x_dim;
					split = x_dim - (split);
					resized_k = (k*split)/z_dim;
					resized_i =i;
					
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
					actor->SetProperty(actorprop_left_red);
					actor->SetReferenceCount(i*5+j*6+k*7);
					rightRenderer->AddActor(actor);
			  
	this->qvtkWidgetRight->GetRenderWindow()->RemoveRenderer(rightRenderer);
	this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->ReInitialize();
	this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->Render();
	this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
	this->qvtkWidgetRight->GetRenderWindow()->Render();
	this->qvtkWidgetRight->update();
					}}
}
void VolVis::clearleft()
{
	//cout<<"value"<<a;
	//cout<<"clearing:";
	this->qvtkWidgetLeft->GetRenderWindow()->RemoveRenderer(leftRenderer);
	this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	this->qvtkWidgetLeft->GetRenderWindow()->Render();
	this->qvtkWidgetLeft->update();

}
void VolVis::clearcenter()
{
	//cout<<"value"<<a;

	this->qvtkWidgetCenter->GetRenderWindow()->RemoveRenderer(centerRenderer);
	this->qvtkWidgetCenter->GetRenderWindow()->AddRenderer(centerRenderer);
	this->qvtkWidgetCenter->GetRenderWindow()->Render();
	this->qvtkWidgetCenter->update();

}
void VolVis::clearright()
{
	//cout<<"value"<<a;

	this->qvtkWidgetRight->GetRenderWindow()->RemoveRenderer(rightRenderer);
	//this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
	this->qvtkWidgetRight->GetRenderWindow()->Render();
	this->qvtkWidgetRight->update();

}
void VolVis::setSliceNumberLeft(int a)
{
	cout<<"value"<<a;
	imageSliceMapper->SetSliceNumber(a);
	
	//this->qvtkWidgetLeft->GetRenderWindow()->RemoveRenderer(leftRenderer);
	//this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
	//this->qvtkWidgetLeft->GetRenderWindow()->Render();
	//this->qvtkWidgetLeft->update();
	left_sliceNumber = a;
}
void VolVis::setSliceNumberCenter(int a)
{
	cout<<"value"<<a;
	
	//emit updateValue(a);
	imageSliceMapper2->SetSliceNumber(a);
	this->qvtkWidgetCenter->update();
	//emit updatevalue();
	center_sliceNumber = a;

}
void VolVis::setSliceNumberRight(int a)
{
	cout<<"value"<<a;
	imageSliceMapper3->SetSliceNumber(a);
	this->qvtkWidgetRight->update();
	right_sliceNumber = a;

}


void VolVis::slotExit() 
{
  qApp->exit();
}
 VolVis::~VolVis() {
 imageSliceMapper= NULL;
 imageSliceMapper2= NULL;
 imageSliceMapper3= NULL;
 };
void VolVis::GetPointGradient(int i, int j, int k, vtkImageData *s)
{
  double g[3];
  double *ar;
 // cout<<" I: :"<<i<<" j: :"<<j<<" k: :"<<k<<endl;
  //s->GetSpacing(ar);
  double sp, sm;
  int extent[6];
  extent[0] = extent[1] = extent[2] = extent[3] = extent[4] = extent[5] = 0; 
  s->GetExtent(extent);
  vtkIdType dims[3];
  dims[0] = extent[1] - extent[0] + 1;
  dims[1] = extent[3] - extent[2] + 1;
  dims[2] = extent[5] - extent[4] + 1;
  ar=s->GetSpacing();
  vtkIdType ijsize=dims[0]*dims[1];

  // Adjust i,j,k to the start of the extent
  i -= extent[0];
  j -= extent[2];
  k -= extent[4];
 // cout<<"After I: :"<<i<<" j: :"<<j<<" k: :"<<k<<endl;
  // Check for out-of-bounds
  if (i < 0 || i >= dims[0] || j < 0 || j >= dims[1] || k < 0 || k >= dims[2])
    {
    g[0] = g[1] = g[2] = 0.0;
    return;
    }

  // x-direction
  if ( dims[0] == 1 )
    {
    g[0] = 0.0;
    }
  else if ( i == 0 )
    {
		sp = s->GetScalarComponentAsFloat(i+1,j,k,0);
		sm = s->GetScalarComponentAsFloat(i,j,k,0);
    //sp = s->GetComponent(i+1 + j*dims[0] + k*ijsize, 0);
    //sm = s->GetComponent(i + j*dims[0] + k*ijsize, 0);
         g[0] = (sm - sp) / ar[0];
    }
  else if ( i == (dims[0]-1) )
    {
		sp = s->GetScalarComponentAsDouble(i,j,k,0);
		sm = s->GetScalarComponentAsDouble(i-1,j,k,0);
    //sp = s->GetComponent(i + j*dims[0] + k*ijsize,0);
    //sm = s->GetComponent(i-1 + j*dims[0] + k*ijsize,0);
        g[0] = (sm - sp) / ar[0];
    }
  else
    {
		sp = s->GetScalarComponentAsDouble(i+1,j,k,0);
		sm = s->GetScalarComponentAsDouble(i-1,j,k,0);
    //sp = s->GetComponent(i+1 + j*dims[0] + k*ijsize,0);
    //sm = s->GetComponent(i-1 + j*dims[0] + k*ijsize,0);
    g[0] = 0.5 * (sm - sp) / ar[0];
    }

  // y-direction
  if ( dims[1] == 1 )
    {
    g[1] = 0.0;
    }
  else if ( j == 0 )
    {
		sp = s->GetScalarComponentAsDouble(i,j+1,k,0);
		sm = s->GetScalarComponentAsDouble(i,j,k,0);
    //sp = s->GetComponent(i + (j+1)*dims[0] + k*ijsize,0);
    //sm = s->GetComponent(i + j*dims[0] + k*ijsize,0);
    g[1] = (sm - sp) / ar[1];
    }
  else if ( j == (dims[1]-1) )
    {
		sp = s->GetScalarComponentAsDouble(i,j,k,0);
		sm = s->GetScalarComponentAsDouble(i,j-1,k,0);
    //sp = s->GetComponent(i + j*dims[0] + k*ijsize,0);
    //sm = s->GetComponent(i + (j-1)*dims[0] + k*ijsize,0);
    g[1] = (sm - sp) / ar[1];
    }
  else
    {
		sp = s->GetScalarComponentAsDouble(i,j+1,k,0);
		sm = s->GetScalarComponentAsDouble(i,j-1,k,0);
    //sp = s->GetComponent(i + (j+1)*dims[0] + k*ijsize,0);
    //sm = s->GetComponent(i + (j-1)*dims[0] + k*ijsize,0);
    g[1] = 0.5 * (sm - sp) / ar[1];
    }

  // z-direction
  if ( dims[2] == 1 )
    {
    g[2] = 0.0;
    }
  else if ( k == 1 )
    {
		sp = s->GetScalarComponentAsDouble(i,j,k+1,0);
		sm = s->GetScalarComponentAsDouble(i,j,k,0);
    //sp = s->GetComponent(i + j*dims[0] + (k+1)*ijsize,0);
    //sm = s->GetComponent(i + j*dims[0] + k*ijsize,0);
    g[2] = (sm - sp) / ar[2];
    }
  else if ( k == (dims[2]-1) )
    {
		sp = s->GetScalarComponentAsDouble(i,j,k,0);
		sm = s->GetScalarComponentAsDouble(i,j,k-1,0);
    //sp = s->GetComponent(i + j*dims[0] + k*ijsize,0);
    //sm = s->GetComponent(i + j*dims[0] + (k-1)*ijsize,0);
    g[2] = (sm - sp) / ar[2];
    }
  else
    {
		sp = s->GetScalarComponentAsDouble(i,j,k+1,0);
		sm = s->GetScalarComponentAsDouble(i,j,k-1,0);
    //sp = s->GetComponent(i + j*dims[0] + (k+1)*ijsize,0);
    //sm = s->GetComponent(i + j*dims[0] + (k-1)*ijsize,0);
    g[2] = 0.5 * (sm - sp) / ar[2];
    }
  if((g[0]!=0) && (g[1]!=0) && (g[2]!=0))
  cout<<"GRADIEMT : "<< "I : "<<i<<"j : "<<j<<"k : "<<k<< " "<< g[0] << " " <<g[1] << " "<< g[2]<<endl;
}