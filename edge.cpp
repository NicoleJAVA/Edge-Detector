

#include "stdio.h"
#include "iostream.h"
#include "stdlib.h"
#include "math.h"

#define PI 3.14159265
#define Size 512
using namespace std;

int main( int argc, char *argv[])
{
	// file pointer
	FILE *file;
	// image data array
	unsigned char Imagedata[Size][Size];
	

	
						/************************************************/
						/******                                	 ******/
						/****** Read "input.raw" into Imagedata[][]******/
						/******                                	 ******/
						/************************************************/	
						
	
	
	


	
	/******************************************************* M A G I C *****/
	if (!(file=fopen("sample1.raw","rb")))
	{
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);

	/***************************************************************/



	// do some image processing task...
	
	
	
	/*************************************************   M A G I C   ***
	printf("\n%d.", Imagedata[50][50]);
	printf("\n%d.", Imagedata[60][50]);
	/******************************************************************/
	
	
	
	float gauss[5][5] = {{2,4,5,4,2}, {4,9,12,9,4}, {5,12,15,12,5}, {4,9,12,9,4}, {2,4,5,4,2}};
	int i = 0, j = 0;
	int iOffset = 0, jOffset = 0; 	//	 the offset within the moving-window
	unsigned char afterGauss[Size][Size];
	float  neighborSum = 0;
	
	float test = 0;
	
	
						/********************************************/
						/******                                ******/
						/******     Blur with Gaussian Filter  ******/
						/******                                ******/
						/********************************************/	
	
	//								   < Construct Gaussian mask >
	
	for( i = 0; i < 5; i++){
		for( j = 0; j < 5; j++ ){
			gauss[i][j] = gauss[i][j] / 159.0;
		}
	}
	
	
	//								   < Move the Gaussian window >		
	
	for( i = 2; i < Size-2; i++ ){
		for( j = 2; j < Size-2; j++){
		
			//printf("\n\n進入第 %d, %d 迴圈.", i, j ); 
			neighborSum = 0;
			test = 0;
			for( iOffset = -2; iOffset <= 2; iOffset++ ){
				for( jOffset = -2; jOffset <= 2; jOffset++ ){
					
					neighborSum +=  Imagedata[i+iOffset][j+jOffset] * gauss[2+iOffset][2+jOffset];
					test += gauss[2+iOffset][2+jOffset];

				}	//		End-4-for
			}	//		End-3-for					
			//Imagedata[i][j] = neighborSum;
			//if( i == 50 && j == 50 ){printf("\nneighborSum : %f. test: %f.\n", neighborSum, test );}
			afterGauss[i][j] = static_cast<unsigned char>(neighborSum);
		 	//printf("  %f", afterGauss[i][j] );
					
		}	//		End-2-for
	}	//		End-1-for

	
	
	
						/********************************************/
						/******                                ******/
						/******    Compute Gradient            ******/
						/******                                ******/
						/********************************************/
								
	/*****************************************************   M A G I C   ****/	
	static float Gr[Size][Size];	// row gradient
	static float Gc[Size][Size];	// column gradient
	static float gradient[Size][Size];
	static float gradTheta[Size][Size];
	float tempGr;	
	float tempGc;
	float actualTheta;
	
	/*************************************************************************/

/*************************************************************************/
	for( i = 1; i < Size - 1; i++ ){
		for( j = 1; j < Size - 1 ; j++){
			tempGr = (float) ( (Imagedata[i-1][j+1]+2*Imagedata[i][j+1]+Imagedata[i+1][j+1]) - (Imagedata[i-1][j-1]+2*Imagedata[i][j-1]+Imagedata[i+1][j-1]) ) / 4.0;
			tempGc = (float) ( (Imagedata[i-1][j-1]+2*Imagedata[i-1][j]+Imagedata[i-1][j+1]) - (Imagedata[i+1][j-1]+2*Imagedata[i+1][j]+Imagedata[i+1][j+1]) ) / 4.0;
			Gr[i][j] = tempGr;
			Gc[i][j] = tempGc;
			gradient[i][j] = sqrt( (Gr[i][j] * Gr[i][j]) + (Gc[i][j] * Gc[i][j]) );
			actualTheta = atan(Gr[i][j]/Gc[i][j]) * 180 / PI;
			
			//								  < map 360-system-angle sto 180-system-angle >		
			
			// if actulThtheta is either near east or near west,
			// then set gradTheta as either exact east or exact west.
			// 正東或者是正西 
			if( (actualTheta<22.5 && actualTheta>-22.5) || actualTheta>157.5 || actualTheta>-157.5 ){
				gradTheta[i][j] = 0;
			}	
			// if actulThtheta is either near north-east or near south-west,
			// then set gradTheta as either exact north-east or exact south-west.
			// 東北或者是西南 
			if( (actualTheta>22.5 && actualTheta<67.5) || (actualTheta<-112.5&&actualTheta>-157.5) ){
				gradTheta[i][j] = 45;
			}	
			// Nearby : Either north or south.
			// 正北或者是正南 
			if( (actualTheta>67.5&& actualTheta<112.5) || (actualTheta<-67.5&&actualTheta>-112.5) ){
				gradTheta[i][j] = 90;
			}	
			// Nearby : Either north-west or south-east
			// 西北或者是東南 
			if( (actualTheta>112.5 && actualTheta<157.5)|| (actualTheta<-22.5&&actualTheta>-67.5) ){
				gradTheta[i][j] = 135;
			}	//		End-3-if 
		}	//		End-2-for 
	}	//		End-1-for
/*************************************************************************/
	
	
	
	
						/********************************************/
						/******                                ******/
						/******    Non-Maximal Suppression     ******/
						/******                                ******/
						/********************************************/
	static float gradNonMax[Size][Size];
	for( i = 2; i < Size -2; i++ ){
		for( j = 2; j < Size -2; j++ ){
			
			if( gradTheta[i][j] == 0 ){	//	Compare with [left] and with [right]
				if( (gradient[i][j] > gradient[i][j-1]) && (gradient[i][j] > gradient[i][j+1]) ){
					gradNonMax[i][j] = gradient[i][j];
				}
				else{
					gradNonMax[i][j] = 0;
				}
			}	//		End-3-if
			else if( gradTheta[i][j] == 45 ){	//	 	[up-right]. [low-left].
				if( (gradient[i][j] > gradient[i-1][j+1]) && (gradient[i][j] > gradient[i+1][j-1]) ){
					gradNonMax[i][j] = gradient[i][j];
				}
				else{
					gradNonMax[i][j] = 0;
				}
			}	//		End-3-else-if			
			else if( gradTheta[i][j] == 90 ){  // Compare With [up] and with [low]
				if( (gradient[i][j] > gradient[i-1][j]) && (gradient[i][j] > gradient[i+1][j]) ){
					gradNonMax[i][j] = gradient[i][j];
				}
				else{
					gradNonMax[i][j] = 0;
				}
			}	//		End-3-else-if			
			else if( gradTheta[i][j] == 135 ){ 	 //  	[up-left].  [low-right].
				if( (gradient[i][j] > gradient[i-1][j-1]) && (gradient[i][j] > gradient[i+1][j+1]) ){
					gradNonMax[i][j] = gradient[i][j];
				}
				else{
					gradNonMax[i][j] = 0;
				}
			}	//		End-3-else-if	
			
		}
	}
	
						/********************************************/
						/******                                ******/
						/******    write image to "ouput.raw"  ******/
						/******                                ******/
						/********************************************/		
	

	if (  !(file=fopen("output.raw","wb"))  )
	{
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else 
	{
		fwrite(afterGauss, sizeof(unsigned char), Size*Size, file);
		fclose(file);
	}

	system("PAUSE");
	exit(0);
}

