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
	source->SetFileName("E:/SemesterIII/Thesis/VolVis_build/headsq-half.vti");
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


	FILE *outputFileTest,*outputFileTest2,*outputFileTest3,*outputFileTrain,*outputFileTrain2,*outputFileTrain3,*outputFileTrainValue,*outputFileIndex,*outputFileIndex1,*timeFile;
	FILE *outputFileTest_2,*outputFileTest2_2,*outputFileTest3_2,*outputFileIndex2;
	FILE *outputFileTest_3,*outputFileTest2_3,*outputFileTest3_3,*outputFileIndex3;
	FILE *outputFileTest_4,*outputFileTest2_4,*outputFileTest3_4,*outputFileIndex4;
	outputFileTest = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test.dat","w");
	outputFileTest2 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test2.dat","w");
	outputFileTest3 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test3.dat","w");
	/*outputFileTest_2 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_2.dat","w");
	outputFileTest2_2 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_22.dat","w");
	outputFileTest3_2 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_23.dat","w");
	outputFileTest_3 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_3.dat","w");
	outputFileTest2_3 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_32.dat","w");
	outputFileTest3_3 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_33.dat","w");
    outputFileTest_4 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_4.dat","w");
	outputFileTest2_4 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_42.dat","w");
	outputFileTest3_4 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Test_43.dat","w");
	*/outputFileTrain = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Train.dat","w");
	outputFileTrain2 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Train2.dat","w");
	outputFileTrain3 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Train3.dat","w");
	

	outputFileTrainValue = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/TrainValue.dat","w");
	outputFileIndex = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Index.dat","w");
	outputFileIndex1 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Index1.dat","w");
	//outputFileIndex2 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Index2.dat","w");
	//outputFileIndex3 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Index3.dat","w");
	//outputFileIndex4 = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/Index4.dat","w");
	timeFile = fopen("E:/SemesterIII/Thesis/VolVis_build/Results/timeFile.dat","w");

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
					double *g1 = new double[3];
	double *g2 = new double[3];
	double *g3 = new double[3];
	double *g4 = new double[3];
	double *g5 = new double[3];
	double *g6 = new double[3];
	double *g7 = new double[3];
	double *g8 = new double[3];
	double *g9 = new double[3];
	double *g10 = new double[3];
	double *g11 = new double[3];
	double *g12 = new double[3];
	double *g13 = new double[3];
	double *g14 = new double[3];
	double *g15 = new double[3];
	double *g16 = new double[3];
	double *g17 = new double[3];
	double *g18 = new double[3];
	double *g19 = new double[3];
	double *g20 = new double[3];
	double *g21 = new double[3];
	double *g22 = new double[3];
	double *g23 = new double[3];
	double *g24 = new double[3];
	double *g25 = new double[3];
	double *g26 = new double[3];
	double *g27 = new double[3];
	float gradientMagnitude[27];// = new double[27];
	float gradientMagnitude2[7];// = new double[7];
colorImage1->GetPointGradient(x-1,y+1,z,colorImage1->GetPointData()->GetScalars(),g1);
			   colorImage1->GetPointGradient(x,y+1,z,colorImage1->GetPointData()->GetScalars(),g2);
			   colorImage1->GetPointGradient(x+1,y+1,z,colorImage1->GetPointData()->GetScalars(),g3);
			   colorImage1->GetPointGradient(x-1,y,z,colorImage1->GetPointData()->GetScalars(),g4);
			   colorImage1->GetPointGradient(x,y,z,colorImage1->GetPointData()->GetScalars(),g5);
			   colorImage1->GetPointGradient(x+1,y,z,colorImage1->GetPointData()->GetScalars(),g6);
			   colorImage1->GetPointGradient(x-1,y+1,z,colorImage1->GetPointData()->GetScalars(),g7);
			   colorImage1->GetPointGradient(x,y-1,z,colorImage1->GetPointData()->GetScalars(),g8);
			   colorImage1->GetPointGradient(x+1,y-1,z,colorImage1->GetPointData()->GetScalars(),g9);
			   

			   colorImage1->GetPointGradient(x-1,y+1,z+1,colorImage1->GetPointData()->GetScalars(),g10);
			   colorImage1->GetPointGradient(x,y+1,z+1,colorImage1->GetPointData()->GetScalars(),g11);
			   colorImage1->GetPointGradient(x+1,y+1,z+1,colorImage1->GetPointData()->GetScalars(),g12);
			   colorImage1->GetPointGradient(x-1,y,z+1,colorImage1->GetPointData()->GetScalars(),g13);
			   colorImage1->GetPointGradient(x,y,z+1,colorImage1->GetPointData()->GetScalars(),g14);
			   colorImage1->GetPointGradient(x+1,y,z+1,colorImage1->GetPointData()->GetScalars(),g15);
			   colorImage1->GetPointGradient(x-1,y+1,z+1,colorImage1->GetPointData()->GetScalars(),g16);
			   colorImage1->GetPointGradient(x,y-1,z+1,colorImage1->GetPointData()->GetScalars(),g17);
			   colorImage1->GetPointGradient(x+1,y-1,z+1,colorImage1->GetPointData()->GetScalars(),g18);
			   

			   
			   colorImage1->GetPointGradient(x-1,y+1,z-1,colorImage1->GetPointData()->GetScalars(),g19);
			   colorImage1->GetPointGradient(x,y+1,z-1,colorImage1->GetPointData()->GetScalars(),g20);
			   colorImage1->GetPointGradient(x+1,y+1,z-1,colorImage1->GetPointData()->GetScalars(),g21);
			   colorImage1->GetPointGradient(x-1,y,z-1,colorImage1->GetPointData()->GetScalars(),g22);
			   colorImage1->GetPointGradient(x,y,z-1,colorImage1->GetPointData()->GetScalars(),g23);
			   colorImage1->GetPointGradient(x+1,y,z-1,colorImage1->GetPointData()->GetScalars(),g24);
			   colorImage1->GetPointGradient(x-1,y+1,z-1,colorImage1->GetPointData()->GetScalars(),g25);
			   colorImage1->GetPointGradient(x,y-1,z-1,colorImage1->GetPointData()->GetScalars(),g26);
			   colorImage1->GetPointGradient(x+1,y-1,z-1,colorImage1->GetPointData()->GetScalars(),g27);
					if((x==0)||(x==(x_dim-1))||(y==0)||(y==(x_dim-1))||(z==0)||(z==(z_dim-1)))
				   {
                    gradientMagnitude[0] = 0;
					gradientMagnitude[1] = 0;//sqrt(sq_gradientMagnitude1);
				    gradientMagnitude[2] = 0;//sqrt(sq_gradientMagnitude2);
				    gradientMagnitude[3] = 0;//sqrt(sq_gradientMagnitude3);
				    gradientMagnitude[4] = 0;//sqrt(sq_gradientMagnitude4);
				    gradientMagnitude[5] = 0;//sqrt(sq_gradientMagnitude5);
				    gradientMagnitude[6] =0;// sqrt(sq_gradientMagnitude6);
				    gradientMagnitude[7] = 0;//sqrt(sq_gradientMagnitude7);
				    gradientMagnitude[8] = 0;//sqrt(sq_gradientMagnitude8);
				    gradientMagnitude[9] = 0;//sqrt(sq_gradientMagnitude9);
				    gradientMagnitude[10] = 0;//sqrt(sq_gradientMagnitude10);
				    gradientMagnitude[11] = 0;//sqrt(sq_gradientMagnitude11);
				    gradientMagnitude[12] = 0;//sqrt(sq_gradientMagnitude12);
				    gradientMagnitude[13] = 0;//sqrt(sq_gradientMagnitude13);
				    gradientMagnitude[14] = 0;//sqrt(sq_gradientMagnitude14);
				    gradientMagnitude[15] = 0;//sqrt(sq_gradientMagnitude15);
				    gradientMagnitude[16] = 0;//sqrt(sq_gradientMagnitude16);
				    gradientMagnitude[17] = 0;//sqrt(sq_gradientMagnitude17);
				    gradientMagnitude[18] = 0;//sqrt(sq_gradientMagnitude18);
				    gradientMagnitude[19] = 0;//sqrt(sq_gradientMagnitude19);
				    gradientMagnitude[20] = 0;//sqrt(sq_gradientMagnitude20);
				    gradientMagnitude[21] = 0;//sqrt(sq_gradientMagnitude21);
				    gradientMagnitude[22] = 0;//sqrt(sq_gradientMagnitude22);
				    gradientMagnitude[23] = 0;//sqrt(sq_gradientMagnitude23);
				    gradientMagnitude[24] = 0;//sqrt(sq_gradientMagnitude24);
				    gradientMagnitude[25] = 0;//sqrt(sq_gradientMagnitude25);
					gradientMagnitude[26] = 0;//sqrt(sq_gradientMagnitude26);
				    //gradientMagnitude[27] = 0;
				   }
				   else
				   {
					double sq_gradientMagnitude1 = (g1[0]*g1[0]) +(g1[1]*g1[1])+(g1[2]*g1[2]);
				    gradientMagnitude[1] = sqrt(sq_gradientMagnitude1);
					double sq_gradientMagnitude2 = (g2[0]*g2[0]) +(g2[1]*g2[1])+(g2[2]*g2[2]);
				    gradientMagnitude[2] = gradientMagnitude2[4] = sqrt(sq_gradientMagnitude2);
					double sq_gradientMagnitude3 = (g3[0]*g3[0]) +(g3[1]*g3[1])+(g3[2]*g3[2]);
				    gradientMagnitude[3] = sqrt(sq_gradientMagnitude3);
					double sq_gradientMagnitude4 = (g4[0]*g4[0]) +(g4[1]*g4[1])+(g4[2]*g4[2]);
				    gradientMagnitude[4] = gradientMagnitude2[1] = sqrt(sq_gradientMagnitude4);
					double sq_gradientMagnitude5 = (g5[0]*g5[0]) +(g5[1]*g5[1])+(g5[2]*g5[2]);
				    gradientMagnitude[5] = gradientMagnitude2[2] = sqrt(sq_gradientMagnitude5);
					double sq_gradientMagnitude6 = (g6[0]*g6[0]) +(g6[1]*g6[1])+(g6[2]*g6[2]);
				    gradientMagnitude[6] = gradientMagnitude2[3] = sqrt(sq_gradientMagnitude6);
					double sq_gradientMagnitude7 = (g7[0]*g7[0]) +(g7[1]*g7[1])+(g7[2]*g7[2]);
				    gradientMagnitude[0] = sqrt(sq_gradientMagnitude7);
					double sq_gradientMagnitude8 = (g8[0]*g8[0]) +(g8[1]*g8[1])+(g8[2]*g8[2]);
				    gradientMagnitude[8] = gradientMagnitude2[5] =  sqrt(sq_gradientMagnitude8);
					double sq_gradientMagnitude9 = (g9[0]*g9[0]) +(g9[1]*g9[1])+(g9[2]*g9[2]);
				    gradientMagnitude[9] = sqrt(sq_gradientMagnitude9);
					double sq_gradientMagnitude10 = (g10[0]*g10[0]) +(g10[1]*g10[1])+(g10[2]*g10[2]);
				    gradientMagnitude[10] = sqrt(sq_gradientMagnitude10);
					double sq_gradientMagnitude11 = (g11[0]*g11[0]) +(g11[1]*g11[1])+(g11[2]*g11[2]);
				    gradientMagnitude[11] = sqrt(sq_gradientMagnitude11);
					double sq_gradientMagnitude12 = (g12[0]*g12[0]) +(g12[1]*g12[1])+(g12[2]*g12[2]);
				    gradientMagnitude[12] = sqrt(sq_gradientMagnitude12);
					double sq_gradientMagnitude13 = (g13[0]*g13[0]) +(g13[1]*g13[1])+(g13[2]*g13[2]);
				    gradientMagnitude[13] = sqrt(sq_gradientMagnitude13);
					double sq_gradientMagnitude14 = (g14[0]*g14[0]) +(g14[1]*g14[1])+(g14[2]*g14[2]);
				    gradientMagnitude[14] = gradientMagnitude2[6] = sqrt(sq_gradientMagnitude14);
					double sq_gradientMagnitude15 = (g15[0]*g15[0]) +(g15[1]*g15[1])+(g15[2]*g15[2]);
				    gradientMagnitude[15] = sqrt(sq_gradientMagnitude15);
					double sq_gradientMagnitude16 = (g16[0]*g16[0]) +(g16[1]*g16[1])+(g16[2]*g16[2]);
				    gradientMagnitude[16] = sqrt(sq_gradientMagnitude16);
					double sq_gradientMagnitude17 = (g17[0]*g17[0]) +(g17[1]*g17[1])+(g17[2]*g17[2]);
				    gradientMagnitude[17] = sqrt(sq_gradientMagnitude17);
					double sq_gradientMagnitude18 = (g18[0]*g18[0]) +(g18[1]*g18[1])+(g18[2]*g18[2]);
				    gradientMagnitude[18] = sqrt(sq_gradientMagnitude18);
					double sq_gradientMagnitude19 = (g19[0]*g19[0]) +(g19[1]*g19[1])+(g19[2]*g19[2]);
				    gradientMagnitude[19] = sqrt(sq_gradientMagnitude19);
					double sq_gradientMagnitude20 = (g20[0]*g20[0]) +(g20[1]*g20[1])+(g20[2]*g20[2]);
				    gradientMagnitude[20] = sqrt(sq_gradientMagnitude20);
					double sq_gradientMagnitude21 = (g21[0]*g21[0]) +(g21[1]*g21[1])+(g21[2]*g21[2]);
				    gradientMagnitude[21] = sqrt(sq_gradientMagnitude21);
					double sq_gradientMagnitude22 = (g22[0]*g22[0]) +(g22[1]*g22[1])+(g22[2]*g22[2]);
				    gradientMagnitude[22] = sqrt(sq_gradientMagnitude22);
					double sq_gradientMagnitude23 = (g23[0]*g23[0]) +(g23[1]*g23[1])+(g23[2]*g23[2]);
				    gradientMagnitude[23] = gradientMagnitude2[0] = sqrt(sq_gradientMagnitude23);
					double sq_gradientMagnitude24 = (g24[0]*g24[0]) +(g24[1]*g24[1])+(g24[2]*g24[2]);
				    gradientMagnitude[24] = sqrt(sq_gradientMagnitude24);
					double sq_gradientMagnitude25 = (g25[0]*g25[0]) +(g25[1]*g25[1])+(g25[2]*g25[2]);
				    gradientMagnitude[25] = sqrt(sq_gradientMagnitude25);
					double sq_gradientMagnitude26= (g26[0]*g2[0]) +(g26[1]*g26[1])+(g26[2]*g26[2]);
					gradientMagnitude[26] = sqrt(sq_gradientMagnitude26);
					double sq_gradientMagnitude27 = (g27[0]*g27[0]) +(g27[1]*g27[1])+(g27[2]*g27[2]);
				    gradientMagnitude[27] = sqrt(sq_gradientMagnitude27);
					}
					double mean=0;
					for(int i=0;i<27;i++)
						mean=mean+gradientMagnitude[i];
					mean = mean/27;
					double variance=0;
					for(int i=0;i<27;i++)
						variance+=((mean - gradientMagnitude[i])*(mean - gradientMagnitude[i]));
					variance = variance/27;
                    

					double mean2=0;
					for(int i=0;i<7;i++)
						mean2+=gradientMagnitude2[i];
					mean2= mean2/7;
					double variance2=0;
					for(int i=0;i<7;i++)
						variance2+=((mean2 - gradientMagnitude2[i])*(mean2 - gradientMagnitude2[i]));
					variance2 = variance2/7;



					
					if((rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]!=0))// || ((x+(y*x_dim)+(z*x_dim*y_dim))%1000 == 0))
						{
							fprintf(outputFileTrainValue,"%d\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)]);
							fprintf(outputFileTrain,"%d 1:%lf 2:%lf 3:%lf 4:%lf 5:%lf 6:%d 7:%d 8:%d\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g[0],g[1],g[2],gradientMagnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),x,y,z);
							if((x==0)||(x==(x_dim-1))||(y==0)||(y==(x_dim-1))||(z==0)||(z==(z_dim-1)))
								{
									//fprintf(outputFileTrain2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,gradientMagnitude5,g5[0],g5[1],g5[2],0,gradient_magnitude4,gradient_magnitude2,gradient_magnitude6,gradient_magnitude8);
									fprintf(outputFileTrain2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean2,sqrt(variance2));
									fprintf(outputFileTrain3,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean,sqrt(variance));
									//fprintf(outputFileTrain3,"%d 1:%lf 2:%lf 3:%lf 4:%d 5:%d 6:%d 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf 13:%lf 14:%lf 15:%lf 16:%lf 17:%lf 18:%lf 19:%lf 20:%lf 21:%lf 22:%lf 23:%lf 24:%lf 25:%lf 26:%lf 27:%lf 28:%lf 29:%lf 30:%lf 31:%lf 32:%lf 33:%lf 34:%lf 35:%lf 36:%lf 37:%lf 38:%lf 39:%lf 40:%lf 41:%lf 42:%lf 43:%lf 44:%lf 45:%lf 46:%lf 47:%lf 48:%lf 49:%lf 50:%lf 51:%lf 52:%lf 53:%lf 54:%lf 55:%lf 56:%lf 57:%lf 58:%lf 59:%lf 60:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g5[0],g5[1],g5[2],x,y,z,gradient_magnitude5,0,gradient_magnitude1,gradient_magnitude2,gradient_magnitude3,gradient_magnitude4,gradient_magnitude6,gradient_magnitude7,gradient_magnitude8,gradient_magnitude9,gradient_magnitude10,gradient_magnitude11,gradient_magnitude12,gradient_magnitude13,gradient_magnitude14,gradient_magnitude15,gradient_magnitude16,gradient_magnitude17,gradient_magnitude18,gradient_magnitude19,gradient_magnitude20,gradient_magnitude21,gradient_magnitude22,gradient_magnitude23,gradient_magnitude24,gradient_magnitude25,gradient_magnitude26,gradient_magnitude27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
								}
							else
								{
									//fprintf(outputFileTrain2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,gradient_magnitude5,g5[0],g5[1],g5[2],colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),gradient_magnitude4,gradient_magnitude2,gradient_magnitude6,gradient_magnitude8);
									fprintf(outputFileTrain2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean2,variance2);
									fprintf(outputFileTrain3,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean,variance);
									//fprintf(outputFileTrain3,"%d 1:%lf 2:%lf 3:%lf 4:%d 5:%d 6:%d 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf 13:%lf 14:%lf 15:%lf 16:%lf 17:%lf 18:%lf 19:%lf 20:%lf 21:%lf 22:%lf 23:%lf 24:%lf 25:%lf 26:%lf 27:%lf 28:%lf 29:%lf 30:%lf 31:%lf 32:%lf 33:%lf 34:%lf 35:%lf 36:%lf 37:%lf 38:%lf 39:%lf 40:%lf 41:%lf 42:%lf 43:%lf 44:%lf 45:%lf 46:%lf 47:%lf 48:%lf 49:%lf 50:%lf 51:%lf 52:%lf 53:%lf 54:%lf 55:%lf 56:%lf 57:%lf 58:%lf 59:%lf 60:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g5[0],g5[1],g5[2],x,y,z,gradient_magnitude5,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),gradient_magnitude1,gradient_magnitude2,gradient_magnitude3,gradient_magnitude4,gradient_magnitude6,gradient_magnitude7,gradient_magnitude8,gradient_magnitude9,gradient_magnitude10,gradient_magnitude11,gradient_magnitude12,gradient_magnitude13,gradient_magnitude14,gradient_magnitude15,gradient_magnitude16,gradient_magnitude17,gradient_magnitude18,gradient_magnitude19,gradient_magnitude20,gradient_magnitude21,gradient_magnitude22,gradient_magnitude23,gradient_magnitude24,gradient_magnitude25,gradient_magnitude26,gradient_magnitude27,colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+(y*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+(y*x_dim)+((z)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y-1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y-1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+(y*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x)+(y*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+(y*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y-1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y-1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+(y*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x)+(y*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+(y*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y-1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y-1)*x_dim)+((z+1)*x_dim*y_dim)));
								}
							fprintf(outputFileIndex1,"%d\n",x+(y*x_dim)+(z*x_dim*y_dim));
					   
					}
						{
					   fprintf(outputFileTest,"%d 1:%lf 2:%lf 3:%lf 4:%lf 5:%lf 6:%d 7:%d 8:%d\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),x,y,z);
					   fprintf(outputFileIndex,"%d\n",x+(y*x_dim)+(z*x_dim*y_dim));
					   if((x==0)||(x==(x_dim-1))||(y==0)||(y==(x_dim-1))||(z==0)||(z==(z_dim-1)))
					   {
						   fprintf(outputFileTest2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean2,variance2);
									fprintf(outputFileTest3,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean,variance);
					   //fprintf(outputFileTest2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,gradient_magnitude5,g5[0],g5[1],g5[2],0,gradient_magnitude4,gradient_magnitude2,gradient_magnitude6,gradient_magnitude8);
					   //fprintf(outputFileTest3,"%d 1:%lf 2:%lf 3:%lf 4:%d 5:%d 6:%d 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf 13:%lf 14:%lf 15:%lf 16:%lf 17:%lf 18:%lf 19:%lf 20:%lf 21:%lf 22:%lf 23:%lf 24:%lf 25:%lf 26:%lf 27:%lf 28:%lf 29:%lf 30:%lf 31:%lf 32:%lf 33:%lf 34:%lf 35:%lf 36:%lf 37:%lf 38:%lf 39:%lf 40:%lf 41:%lf 42:%lf 43:%lf 44:%lf 45:%lf 46:%lf 47:%lf 48:%lf 49:%lf 50:%lf 51:%lf 52:%lf 53:%lf 54:%lf 55:%lf 56:%lf 57:%lf 58:%lf 59:%lf 60:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g5[0],g5[1],g5[2],x,y,z,gradient_magnitude5,0,gradient_magnitude1,gradient_magnitude2,gradient_magnitude3,gradient_magnitude4,gradient_magnitude6,gradient_magnitude7,gradient_magnitude8,gradient_magnitude9,gradient_magnitude10,gradient_magnitude11,gradient_magnitude12,gradient_magnitude13,gradient_magnitude14,gradient_magnitude15,gradient_magnitude16,gradient_magnitude17,gradient_magnitude18,gradient_magnitude19,gradient_magnitude20,gradient_magnitude21,gradient_magnitude22,gradient_magnitude23,gradient_magnitude24,gradient_magnitude25,gradient_magnitude26,gradient_magnitude27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
					   }
					   else
					   {
						   fprintf(outputFileTest2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean2,variance2);
									fprintf(outputFileTest3,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,g[0],g[1],g[2],gradient_magnitude,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),mean,variance);
					   //fprintf(outputFileTest2,"%d 1:%d 2:%d 3:%d 4:%lf 5:%lf 6:%lf 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],x,y,z,gradient_magnitude5,g5[0],g5[1],g5[2],colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),gradient_magnitude4,gradient_magnitude2,gradient_magnitude6,gradient_magnitude8);
					   //fprintf(outputFileTest3,"%d 1:%lf 2:%lf 3:%lf 4:%d 5:%d 6:%d 7:%lf 8:%lf 9:%lf 10:%lf 11:%lf 12:%lf 13:%lf 14:%lf 15:%lf 16:%lf 17:%lf 18:%lf 19:%lf 20:%lf 21:%lf 22:%lf 23:%lf 24:%lf 25:%lf 26:%lf 27:%lf 28:%lf 29:%lf 30:%lf 31:%lf 32:%lf 33:%lf 34:%lf 35:%lf 36:%lf 37:%lf 38:%lf 39:%lf 40:%lf 41:%lf 42:%lf 43:%lf 44:%lf 45:%lf 46:%lf 47:%lf 48:%lf 49:%lf 50:%lf 51:%lf 52:%lf 53:%lf 54:%lf 55:%lf 56:%lf 57:%lf 58:%lf 59:%lf 60:%lf\n",rasterize_array[x+(y*x_dim)+(z*x_dim*y_dim)],g5[0],g5[1],g5[2],x,y,z,gradient_magnitude5,colorImage1->GetPointData()->GetScalars()->GetTuple1(x+(y*x_dim)+(z*x_dim*y_dim)),gradient_magnitude1,gradient_magnitude2,gradient_magnitude3,gradient_magnitude4,gradient_magnitude6,gradient_magnitude7,gradient_magnitude8,gradient_magnitude9,gradient_magnitude10,gradient_magnitude11,gradient_magnitude12,gradient_magnitude13,gradient_magnitude14,gradient_magnitude15,gradient_magnitude16,gradient_magnitude17,gradient_magnitude18,gradient_magnitude19,gradient_magnitude20,gradient_magnitude21,gradient_magnitude22,gradient_magnitude23,gradient_magnitude24,gradient_magnitude25,gradient_magnitude26,gradient_magnitude27,colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+(y*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+(y*x_dim)+((z)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y-1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y-1)*x_dim)+(z*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+(y*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x)+(y*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+(y*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y-1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y-1)*x_dim)+((z-1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+(y*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x)+(y*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+(y*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x-1)+((y+1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1(x+((y-1)*x_dim)+((z+1)*x_dim*y_dim)),colorImage1->GetPointData()->GetScalars()->GetTuple1((x+1)+((y-1)*x_dim)+((z+1)*x_dim*y_dim)));
					   }
					   three1++;
					   }
			
					}}
		  std::clock_t end = std::clock();
		  cout<<"time taken " <<end-c_start<<endl;
		  cout<<"one :"<<one1<<"two1 "<<two1<<"three1 "<<three1<<endl;
		  fclose(outputFileTest);
		  fclose(outputFileTest2);
		  fclose(outputFileTest3);

		//   fclose(outputFileTest_2);
		 // fclose(outputFileTest2_2);
		 // fclose(outputFileTest3_2);

		   //fclose(outputFileTest_3);
		  //fclose(outputFileTest2_3);
		  //fclose(outputFileTest3_3);

		   //fclose(outputFileTest_4);
		  //fclose(outputFileTest2_4);
		  //fclose(outputFileTest3_4);
		  fclose(outputFileTrain);
		  fclose(outputFileTrain2);
		  fclose(outputFileTrain3);
		  fclose(outputFileIndex);
		  fclose(outputFileIndex1);
		  //fclose(outputFileIndex2);
		  //fclose(outputFileIndex3);
		  //fclose(outputFileIndex4);
		  fclose(outputFileTrainValue);
		  cout<<"============================COMPLETED===================================================";
  
}
