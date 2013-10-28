

#include "stdio.h"
#include "iostream.h"
#include "stdlib.h"

// define image size 512*512
// you can change it to other size
#define Size 512
using namespace std;

int main( int argc, char *argv[])
{
	// file pointer
	FILE *file;
	// image data array
	unsigned char Imagedata[Size][Size];
	

	
	
	

	// read image "lena.raw" into image data matrix
	
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
		
			//printf("\n\n¶i¤J²Ä %d, %d °j°é.", i, j ); 
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
						/******    write image to "ouput.raw"  ******/
						/******                                ******/
						/********************************************/		
	

	if (!(file=fopen("output.raw","wb")))
	{
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fwrite(afterGauss, sizeof(unsigned char), Size*Size, file);
	fclose(file);

	system("PAUSE");
	exit(0);
}

