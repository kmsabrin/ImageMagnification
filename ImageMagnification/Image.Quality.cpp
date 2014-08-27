#include <math.h>
#include <stdio.h>

#define Max_Dimension 512 + 1

char i_file_a[] = "lena511.bmp"; 
//char i_file_b[] = "lancoz511.bmp";
char i_file_b[] = "m2.bmp";

int W;     // image width
int H;	   // image height
int D;     // image depth

struct COLOR
{
	unsigned char r, g, b;
};
struct COLOR image_a[Max_Dimension][Max_Dimension];
struct COLOR image_b[Max_Dimension][Max_Dimension];

void MSE()
{
	double E = 0;

	for ( int i = 0; i < H; ++i )
	{
		for ( int j = 0; j < W; ++j )
		{
			E += ( image_a[i][j].r - image_b[i][j].r ) * ( image_a[i][j].r - image_b[i][j].r );
		}
	}

	E /= W * H;

	printf( "%lf\n", E );

}

void PSNR()
{
	double E = 0;
	
	for ( int i = 0; i < H; ++i )
	{
		for ( int j = 0; j < W; ++j )
		{
			E += ( image_a[i][j].r - image_b[i][j].r ) * ( image_a[i][j].r - image_b[i][j].r );
		}
	}

	E = E / ( W * H );

	E = 255.0 / E;

	E = 20.0 * log10( E );
	
	printf( "%lf\n", E );
}

void Image_Input( char i_file[], struct COLOR image[][Max_Dimension] )
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

	printf( "W=%d, H=%d, D=%d\n", W, H, D );
				
	fseek( fp, data_offset, SEEK_SET );	
	int i, j;	
	for ( i = 0; i < H; ++i )
	{
		for ( j = 0; j < W; ++j )
		{
			fscanf( fp, "%c%c%c", &image[i][j].b, &image[i][j].g, &image[i][j].r );
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
	Image_Input( i_file_a, image_a ); 				
	Image_Input( i_file_b, image_b ); 	
	//MSE();
	PSNR();
}
