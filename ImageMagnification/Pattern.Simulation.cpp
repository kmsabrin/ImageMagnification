#include <stdio.h>

#define Max( i, j ) ( ( ( i ) > ( j ) ) ? ( i ) : ( j ) )

#define Min( i, j ) ( ( ( i ) < ( j ) ) ? ( i ) : ( j ) )

char a[3][3];

char b[5][5];

char used[3][3];

int d = 3;

int n = 0;

FILE *fp = fopen( "a.txt", "w" );

void process()
{
	fprintf( fp, "PATTERN %2d:\n\n", ++n );

	int i, j;

	for ( i = 0; i < d; ++i )
	{
		for ( j = 0; j < d; ++j )
		{
			fprintf( fp, "%4d", a[i][j] );

			b[2 * i][2 * j] = a[i][j];
		}

		fprintf( fp, "\n" );
	}

	fprintf( fp, "\n" );

	for ( i = 0; i < 2 * d - 1; ++i )
	{
		for ( j = 0; j < 2 * d - 1; ++j )
		{
			if ( ( i % 2 ) && ( j % 2 ) ) // i odd, j odd, center pixel
			{
				b[i][j] = Max( Min( b[i + 1][j + 1], b[i - 1][j - 1] ), Min( b[i + 1][j - 1], b[i - 1][j + 1] ) );		
			}
			else if ( !( i % 2 ) && ( j % 2 ) ) // i even j odd, horizontal pixel
			{		
				b[i][j] = Min( b[i][j - 1], b[i][j + 1] );
			}
			else if ( ( i % 2 ) && !( j % 2 ) ) // i odd j even, vertical pixel
			{		
				b[i][j] = Min( b[i + 1][j], b[i - 1][j] );				
			}	
			else
			{
				// i even j even is the original image
			}

			fprintf( fp, "%4d", b[i][j] );
		}

		fprintf( fp, "\n" );
	}

	fprintf( fp, "\n\n" );
}

void call()
{
	int k = 0;

	for ( int i = 0; i < d; ++i )
	{
		for ( int j = 0; j < d; ++j )
		{
			if ( used[i][j] == 0 )
			{
				used[i][j] = 1;

				a[i][j] = 0;

				call();

				a[i][j] = 1;

				call();

				used[i][j] = 0;

				k = 1;

				break;
			}
		}

		if ( k == 1 )
		{
			break;
		}
	}

	if ( k == 0 )
	{
		process();
	}
}

void main()
{	
	call();	
	
	fclose( fp );
}