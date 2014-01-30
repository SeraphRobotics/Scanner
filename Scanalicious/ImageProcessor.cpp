
<!-- saved from url=(0064)https://scanalicious.googlecode.com/svn/trunk/ImageProcessor.cpp -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"><style type="text/css"></style></head><body><pre style="word-wrap: break-word; white-space: pre-wrap;">/*
 *  ImageProcessor.cpp
 *  Cocoa OpenGL
 *
 *  Created by Ryan Dunn on 4/20/09.
 *  Copyright 2009 Cornell University. All rights reserved.
 *
 */

#include "ImageProcessor.h"

ImageProcessor* ImageProcessor::pinstance = 0;

ImageProcessor* ImageProcessor::Instance()
{
	if (pinstance == 0) //first call?
	{
		pinstance = new ImageProcessor; //create sole instance
	}
	
	return pinstance;	//address of sole instance
}

ImageProcessor::ImageProcessor()
{ 
	redThresh = 200;
	MeshContainer &amp;meshBag = *MeshContainer::Instance();
	res = meshBag.getRes();
	
	//declare the amount of sampled points
	//depending on the resolution of the mesh
	samplePoints = new int*[res];
	for(int i = 0; i &lt; res; i++)
		samplePoints[i] = new int[2];
}

ImageProcessor::~ImageProcessor()
{
	delete red;
	delete green;
	delete blue;
	delete alpha;
}


char ImageProcessor::imageR(int xPic, int yPic)
{
	return red[xPic + yPic*xRes];
}

void ImageProcessor::setImageR(int xPic, int yPic, char val)
{
	red[xPic + yPic*xRes] = val;
}

//gets the red threshold
void ImageProcessor::setRedThresh( char *data, int xRes, int yRes)
{	
	char temp = 0;
	
	//load image and set resolution
	red = data;
	
	ImageProcessor::xRes = xRes;
	ImageProcessor::yRes = yRes;
		
	for(int x=0; x&lt; xRes; x++)
	{
		for(int y=0; y&lt;yRes; y++)
		{
			if( imageR(x,y) &gt; temp)
				temp = imageR(x,y);
		}
	}
	
	//set the new thresh -- TODO determine what works best here
	redThresh = temp + 10; 
	
}

//processes the line in an image and sends it to calibrator
void ImageProcessor::processLine( char * data, int xRes, int yRes, double zWorld, double theta)
{
	
	//load in the image
	red = data;
	ImageProcessor::xRes = xRes;
	ImageProcessor::yRes = yRes;
		
	//information about line 
	int lineLen = 0;
	int count;
	int temp;
	
	//step size for sampling
	double step;
		
	//this will be used to pass on the information
	CalibrationUnit &amp;calibrator = *CalibrationUnit::Instance();
	
	
	//first scan through image and take out anything that is lower
	//than the specified threshold and make the line one pixel wide
	for(int y=0; y&lt; yRes; y++)
	{
		count = 0;
		temp = 0;
		
		for(int x=0; x&lt;xRes; x++)
		{
			if( imageR(x,y) &gt;= redThresh )
			{
				count++;
				temp += x;
			}
				setImageR(x,y, 0);
		}//end for x
		
		//make the line one pixel wide
		
		/*might want to look into something that
		 takes into account if the points are close
		 to one another -- this would help eliminate noise*/
	
		if(count &gt;= 3)
		{
			count = temp/count;
			setImageR( ((int)temp/count), y, 255);
			lineLen++;
		}
	
	}//end for y

	//step size when we sample
	step = (double)lineLen/res;
	
	//reuse count as a counter
	count = 0;
	
	//loop through the image again
	for(int y=0; y &lt; yRes; y++)
	{		
		for(int x=0; x &lt; xRes; x++)
		{
			if( imageR(x,y) &gt; 0 )
			{
				linePts[count][0] = x;
				linePts[count][1] = y;
				count++;
			}
		}//end for x
	}//end for y
	
	//now finally sample the points
	for(int i=0; i &lt; res;i++)
	{
		samplePoints[i][0] = linePts[((int)floor(i*step))][0];
		samplePoints[i][1] = linePts[((int)floor(i*step))][1];
	}
	
	//send the data over to the CalibrationUnit
	calibrator.setPicRes(xRes, yRes);
	calibrator.lineToMesh(samplePoints, zWorld, theta);
	
}

</pre></body></html>