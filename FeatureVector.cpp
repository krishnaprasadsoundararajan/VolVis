#include<FeatureVector.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataReader.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iostream>
#include <ctime>
#include<vtkDataArray.h>
#include <vtkPointData.h> // Added to access the instensity colorImage1->GetPointData()-> ....

#define PTR vtkSmartPointer
PTR<vtkImageData> colorImage1;
FeatureVector::FeatureVector()
{
	

	PTR<vtkXMLImageDataReader> source = PTR<vtkXMLImageDataReader>::New();
	source->SetFileName("I:/assignment-06-octree-solution (1)/assignment-06-octree-solution/headsq-half.vti");
	source->Update();
	


// ->GetScalars()->GetTuple1(10);
	colorImage1 = source->GetOutput();
	colorImage1->UpdateInformation();
	


	}
void FeatureVector::CreateTestTrainData(int x_dim,int y_dim,int z_dim,int (*rasterize_array))
{
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
 int one1=0,two1=0,three1=0;
  cout<<"Extents "<<extent[0]<<" "<<extent[1]<<" "<<extent[2]<<" "<<extent[3]<<" "<<extent[4]<<" "<<extent[5]<<" "<<endl;
  std::clock_t c_start= std::clock();
	for(int x = 0; x < x_dim; x++) //extent[1]
	   for(int y = 0; y < y_dim; y++)//extent[3]
		  for(int z = 0; z < z_dim; z++)//extent[5]
		   {
			one1++;   

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
					if((rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]!=0))// || ((x+(y*x_dim)+(z*x_dim*y_dim))%1000 == 0))
						{
							//cout<<"x+(y*x_dim)+(z*x_dim*y_dim)"<< x+(y*x_dim)+(z*x_dim*y_dim);
							//ssTrain<<rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]<<" "<<"1:"<<colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim))<<" 2:"<<g[0]<<" 3:" <<g[1]<<" 4:"<<g[2]<<" 5:"<<gradient_magnitude<<endl;
				        	//ssIndex1<<x+(y*(extent[1] - extent[0]))+(z*(extent[1] - extent[0])*(extent[3] - extent[2]))<<endl;
					        fprintf(outputFileTrain,"%d 1:%lf 2:%lf 3:%lf 4:%lf 5:%lf 6:%d 7:%d 8:%d\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),x,y,z);
					two1++;
							fprintf(outputFileIndex1,"%d\n",x+(y*x_dim)+(z*x_dim*y_dim));
					   }
					else
					{
						//cout<<"writing test"<<
						

						//cout<<"G: "<<g[0]<<" "<<g[1]<<" "<<g[2];
			            //ssTest<<rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]<<" "<<"1:"<<g[0]<<" 2:"<<g[1]<<" 3:" <<g[2]<<" 4:"<<gradient_magnitude<<" 5:"<<colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim))<<endl;
						//outputFileTest<<rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]<<" "<<"1:"<<g[0]<<" 2:"<<g[1]<<" 3:" <<g[2]<<" 4:"<<gradient_magnitude<<" 5:"<<colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim))<<endl;
					   //ssIndex<<x+(y*(extent[1] - extent[0]))+(z*(extent[1] - extent[0])*(extent[3] - extent[2]))<<endl;
					   fprintf(outputFileTest,"%d 1:%lf 2:%lf 3:%lf 4:%lf 5:%lf 6:%d 7:%d 8:%d\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),x,y,z);
					   fprintf(outputFileIndex,"%d\n",x+(y*x_dim)+(z*x_dim*y_dim));
					three1++;
					}

				
						


			   }}
		  std::clock_t end = std::clock();
		  cout<<"time taken " <<end-c_start<<endl;
		  cout<<"one :"<<one1<<"two1 "<<two1<<"three1 "<<three1<<endl;
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
		  cout<<"============================COMPLETED===================================================";
}
void FeatureVector::updateRasterizeArrayafterTraining(int (*rasterize_array))
{
	cout<<"called============";
    ifstream inputPredictedFile("E:/SemesterIII/Thesis/libsvm-3.17/windows/PredictedOutput");
	ifstream outputFileIndex("E:/SemesterIII/Thesis/libsvm-3.17/windows/Index.dat");
	while (!inputPredictedFile.eof())
	{
        int x,y;
		outputFileIndex >> x;
		inputPredictedFile >> y;
		rasterize_array[x]=y;
	}
	cout<<"completed================";
}


void FeatureVector::GetPointGradient(int i, int j, int k, vtkImageData *s,double *g)
{
  //double g[3];
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
  //vtkIdType ijsize=dims[0]*dims[1];

  // Adjust i,j,k to the start of the extent
  //i -= extent[0];
  //j -= extent[2];
  //k -= extent[4];
 // cout<<"After I: :"<<i<<" j: :"<<j<<" k: :"<<k<<endl;
  // Check for out-of-bounds
  if (i < extent[0] || i >= dims[0] || j < extent[2] || j >= dims[1] || k < extent[4] || k >= dims[2])
    {
    g[0] = g[1] = g[2] = 0.0;
    return;
    }

  // x-direction
  if ( dims[0] == 1 )
    {
    g[0] = 0.0;
    }
  else if ( i == extent[0]  )
    {
		sp = s->GetScalarComponentAsFloat(i+1,j,k,0);
		sm = s->GetScalarComponentAsFloat(i,j,k,0);
        g[0] = (sm - sp) / ar[0];
    }
  //else if ( i == (dims[0]-1) )
  else if ( i == extent[1]) 
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
    g[0] = (0.5 * (sm - sp)) / ar[0];
    }

  // y-direction
  if ( dims[1] == 1 )
    {
    g[1] = 0.0;
    }
  else if ( j == extent[2] )
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
    g[1] = (0.5 * (sm - sp)) / ar[1];
    }

  // z-direction
  if ( dims[2] == 1 )
    {
    g[2] = 0.0;
    }
  else if ( k == extent[4] )
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
  return;
 // if((g[0]!=0) && (g[1]!=0) && (g[2]!=0))
  //cout<<"GRADIEMT : "<< "I : "<<i<<"j : "<<j<<"k : "<<k<< " "<< g[0] << " " <<g[1] << " "<< g[2]<<endl;
  
}