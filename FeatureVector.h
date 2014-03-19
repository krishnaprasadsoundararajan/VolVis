#ifndef FeatureVector_H
#define FeatureVector_H
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <iostream>
//#include "ui_VolVis.h"
class FeatureVector 
{
	public:
	FeatureVector();
	void GetPointGradient(int,int,int,vtkImageData*,double*);
	void FeatureVector::CreateTestTrainData(int x,int y,int z,int* (g));
	void updateRasterizeArrayafterTraining(int (*g));
	
};
#endif