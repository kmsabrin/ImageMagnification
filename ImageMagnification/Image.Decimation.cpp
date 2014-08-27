#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FreeImage.h"

#define Max( i, j ) ( ( ( i ) > ( j ) ) ? ( i ) : ( j ) )
#define Min( i, j ) ( ( ( i ) < ( j ) ) ? ( i ) : ( j ) )
#define Max_Dimension ( 1000 )

char i_file[100]; 
char o_file[100]; 

int W;     // image width
int H;	   // image height
int D;     // image depth
int M = 2; // magnification factor

struct COLOR
{
	unsigned char r, g, b;
};
struct COLOR i_image[Max_Dimension][Max_Dimension];


void Image_Output()
{	
	FreeImage_Initialise();	
	FIBITMAP *m_image = FreeImage_Allocate( W / 2, H / 2, 24 );		  
	struct tagRGBQUAD rgbquad;
	
	int i, ii, j, jj;

	for ( i = ii = 0; i < H; i += 2, ++ii )
	{
		for ( j = jj = 0; j < W; j += 2, ++jj )
		{					
			rgbquad.rgbRed   = i_image[i][j].r;						
			rgbquad.rgbGreen = i_image[i][j].g;			
			rgbquad.rgbBlue  = i_image[i][j].b;				  				  
			FreeImage_SetPixelColor( m_image, jj, ii, &rgbquad );				  			
		}
	}
		  
	FreeImage_Save( FIF_BMP, m_image, o_file, BMP_DEFAULT );		
	FreeImage_DeInitialise();
}

void Image_Input()
{				
	unsigned char b0, b1, b2, b3;	
	int t1 = 0, t2;
	FILE *fp = fopen( i_file, "rb" );
	
	fseek( fp, 10, SEEK_SET );	
	fscanf( fp, "%c%c%c%c", &b0, &b1, &b2, &b3 );	
	int data_offset = ( ( ( ( b3 << 8 ) | b2 ) << 8 | b1 ) << 8 | b0 );
	
	fseek( fp, 18, SEEK_SET );	
	fscanf( fp, "%c%c%c%c", &b0, &b1, &b2, &b3 );	
	W = ( ( ( ( b3 << 8 ) | b2 ) << 8 | b1 ) << 8 | b0 );	
	if ( W * 3 % 4 )
	{
		t1 = ( ( W * 3 ) / 4 );
		t2 = ( t1 + 1 ) * 4;
		t1 = t2 - W * 3;
	}
	
	fseek( fp, 22, SEEK_SET );	
	fscanf( fp, "%c%c%c%c", &b0, &b1, &b2, &b3 );	
	H = ( ( ( ( b3 << 8 ) | b2 ) << 8 | b1 ) << 8 | b0 );		
				
	fseek( fp, 28, SEEK_SET );	
	fscanf( fp, "%c%c", &b0, &b1 );	
	D = ( ( b1 << 8 ) | b0 );
				
	fseek( fp, data_offset, SEEK_SET );	
	int i, j;	
	for ( i = 0; i < H; ++i )
	{
		for ( j = 0; j < W; ++j )
		{
			fscanf( fp, "%c%c%c", &i_image[i][j].b, &i_image[i][j].g, &i_image[i][j].r );
		}
		
		for ( j = 0; j < t1; ++j )
		{
			fscanf( fp, "%c", &b0 );
		}
	}
	
	fclose( fp );
}

void main()
{
	strcpy( i_file, ".\\Image\\football192.bmp");	
	strcpy( o_file, ".\\Image\\football96.bmp");		
	Image_Input(); 	
	Image_Output();		
}
