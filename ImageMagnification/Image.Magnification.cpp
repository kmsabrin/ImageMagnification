#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FreeImage.h"

#define Max( i, j ) ( ( ( i ) > ( j ) ) ? ( i ) : ( j ) )
#define Min( i, j ) ( ( ( i ) < ( j ) ) ? ( i ) : ( j ) )
#define Max_Dimension ( 5000 + 2 )

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
struct COLOR i_image[Max_Dimension / 2][Max_Dimension / 2];
struct COLOR o_image[Max_Dimension][Max_Dimension];

unsigned char r_image[Max_Dimension][Max_Dimension];
unsigned char g_image[Max_Dimension][Max_Dimension];
unsigned char b_image[Max_Dimension][Max_Dimension];

bool valid( int i, int j )
{
	if ( ( i > 0 ) && ( i < M * H - 1 ) && ( j > 0 ) && ( j < M * W - 1 ) )
	{
		return true;
	}
	
	return false;
}

unsigned char min5( unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5 )
{
	unsigned char mn = a1;
	mn = Min( mn, a2 );
	mn = Min( mn, a3 );
	mn = Min( mn, a4 );
	mn = Min( mn, a5 );
	return mn;
}

unsigned char AbsDif( unsigned char a1, unsigned char a2 )
{
	if ( a1 > a2 ) 
	{
		return a1 - a2;
	}

	return a2 - a1;
}

void Interpolation( int i, int j, unsigned char t_image[][Max_Dimension] )
{						
	if ( ( i % 2 ) && ( j % 2 ) ) // i odd, j odd, center pixel
	{						
		t_image[i][j] = Max( Min( t_image[i + 1][j + 1], t_image[i - 1][j - 1] ), Min( t_image[i + 1][j - 1], t_image[i - 1][j + 1] ) );								
		return;
	
		unsigned char TH = 127;
		
		// NW
		if ( valid( i - 3, j - 1 ) && valid( i + 1, j + 3 ) && 
			( min5( t_image[i + 1][j - 1], t_image[i + 1][j + 1], t_image[i - 1][j - 3], t_image[i - 3][j - 1], t_image[i + 1][j + 3] ) - t_image[i - 1][j + 1] > TH ) )			 
		{				
			t_image[i][j] = t_image[i - 1][j + 1];						
		}		
		// SE
		else if ( valid( i + 3, j + 1 ) && valid( i - 1, j - 3 )    && 
			( min5( t_image[i + 1][j + 1], t_image[i - 1][j + 1], t_image[i - 1][j - 1], t_image[i + 3][j + 1], t_image[i - 1][j - 3] ) - t_image[i + 1][j - 1] > TH ) )			 			 
		{		
			t_image[i][j] = t_image[i + 1][j - 1];						
		}		
		// SW
		else if ( valid( i + 3, j - 1 ) && valid( i - 1, j + 3 )    && 
			( min5( t_image[i + 1][j - 1], t_image[i - 1][j + 1], t_image[i - 1][j - 1], t_image[i + 3][j - 1], t_image[i - 1][j + 3] ) - t_image[i + 1][j + 1] > TH ) )			 			
		{						
			t_image[i][j] = t_image[i + 1][j + 1];			
		}		
		// NE
		else if ( valid( i + 1, j - 3 ) && valid( i - 3, j + 1 )    &&
			( min5( t_image[i + 1][j + 1], t_image[i + 1][j - 1], t_image[i - 1][j + 1], t_image[i + 1][j - 3], t_image[i - 3][j + 1] ) - t_image[i - 1][j - 1] > TH ) )			 		
		{			
			t_image[i][j] = t_image[i - 1][j - 1];						
		}				
		else
		{
			t_image[i][j] = Max( Min( t_image[i + 1][j + 1], t_image[i - 1][j - 1] ), Min( t_image[i + 1][j - 1], t_image[i - 1][j + 1] ) );								
		}
	}
	else if ( !( i % 2 ) && ( j % 2 ) ) // i even j odd, horizontal pixel
	{		
		t_image[i][j] = Min( t_image[i][j - 1], t_image[i][j + 1] );
		//t_image[i][j] = Min( t_image[i][j - 1], t_image[i][j + 1] ) + AbsDif( t_image[i][j - 1], t_image[i][j + 1] ) / 2;
		
	}
	else if ( ( i % 2 ) && !( j % 2 ) ) // i odd j even, vertical pixel
	{		
		t_image[i][j] = Min( t_image[i + 1][j], t_image[i - 1][j] );				
		//t_image[i][j] = Min( t_image[i + 1][j], t_image[i - 1][j] ) + AbsDif( t_image[i + 1][j], t_image[i - 1][j] ) / 2;
	}	
	else
	{
		// i even j even = The original image pixels
	}
}

void Image_Magnification()
{
	int i, j;
	
	for ( i = 0; i < H; ++i )
	{
		for ( j = 0; j < W; ++j )
		{
			r_image[i << 1][j << 1] = i_image[i][j].r;							
			g_image[i << 1][j << 1] = i_image[i][j].g;							
			b_image[i << 1][j << 1] = i_image[i][j].b;	
		}
	}
	
	for ( i = 0; i < M * H  - 1; ++i )
	{
		for ( j = 0; j < M * W - 1; ++j )
		{
			Interpolation( i, j, r_image );			
			Interpolation( i, j, g_image );			
			Interpolation( i, j, b_image );	
		}
	}

	for ( i = 0; i < M * H  - 1; ++i )
	{
		for ( j = 0; j < M * W - 1; ++j )
		{				
			o_image[i][j].r = r_image[i][j];			
			o_image[i][j].g = g_image[i][j];			
			o_image[i][j].b = b_image[i][j];			
		}
	}	
}		

void Image_Output()
{	
	/*
	FreeImage_Initialise();	
	FIBITMAP *m_image = FreeImage_Allocate( M * W - 1, M * H - 1, 24 );		  
	struct tagRGBQUAD rgbquad;
	
	for ( int i = 0; i < M * H - 1; ++i )
	{
		for ( int j = 0; j < M * W - 1; ++j )
		{					
			rgbquad.rgbRed   = o_image[i][j].r;						
			rgbquad.rgbGreen = o_image[i][j].g;			
			rgbquad.rgbBlue  = o_image[i][j].b;				  				  
			FreeImage_SetPixelColor( m_image, j, i, &rgbquad );				  
		}
	}
		  
	FreeImage_Save( FIF_BMP, m_image, o_file, BMP_DEFAULT );		
	FreeImage_DeInitialise();
	*/

	FreeImage_Initialise();	
	FIBITMAP *m_image = FreeImage_Allocate( M * W, M * H, 24 );		  
	struct tagRGBQUAD rgbquad;
	
	for ( int i = 0; i < M * H; ++i )
	{
		for ( int j = 0; j < M * W; ++j )
		{					
			if ( i == ( M * H - 1 ) || j == ( M * W - 1 ) )
			{
				int u = 0, v = 0;

				if ( i == ( M * H - 1 ) )
				{
					u = -1;	
				}
		
				if ( j == ( M * W - 1 ) )
				{
					v = -1;
				}

				rgbquad.rgbRed   = o_image[i+u][j+v].r;						
				rgbquad.rgbGreen = o_image[i+u][j+v].g;			
				rgbquad.rgbBlue  = o_image[i+u][j+v].b;				  				  
				FreeImage_SetPixelColor( m_image, j, i, &rgbquad );				  
			}
			else
			{			
				rgbquad.rgbRed   = o_image[i][j].r;						
				rgbquad.rgbGreen = o_image[i][j].g;			
				rgbquad.rgbBlue  = o_image[i][j].b;				  				  
				FreeImage_SetPixelColor( m_image, j, i, &rgbquad );				  
			}
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
	char tmp[100];
	printf( "Image name: " );
	gets( tmp );
	strcpy( i_file, ".\\Image\\");
	strcat( i_file, tmp );
	strcpy( o_file, ".\\Image\\x2bdai.bmp");

	double s = clock();
	Image_Input(); 				
	Image_Magnification();		
	Image_Output();
	double e = clock();		
		
	strcpy( i_file, ".\\Image\\x2bdai.bmp");
	strcpy( o_file, ".\\Image\\x4bdai.bmp");
	Image_Input(); 				
	Image_Magnification();		
	Image_Output();
	
	printf( "Time : %.0lf Milliseconds\n", e - s );		
}
