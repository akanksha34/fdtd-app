#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
  float x;
  float y;
  float z;
}Vector;

Vector VectorAdd( Vector V1, Vector V2 ){
  Vector V3;
  V3.x = V1.x + V2.x;
  V3.y = V1.y + V2.y;
  V3.z = V1.z + V2.z;
  return V3;
}

Vector ScalarMul( Vector V1, float a ){
  Vector V2;
  V2.x = a*V1.x;
  V2.y = a*V1.y;
  V2.z = a*V1.z;
  return V2;
}

Vector VectorMul( Vector V1, Vector V2 ){
  Vector V3;
  V3.x = V1.y*V2.z - V1.z*V2.y;
  V3.y = V1.z*V2.x - V1.x*V2.z;
  V3.z = V1.x*V2.y - V1.y*V2.x;
  return V3;
}

# define X_MAX 60
# define Y_MAX 60
# define Z_MAX 60

int main()
{
  FILE *file1;
  file1 = fopen("electricF.txt","w");
  FILE *file2;
  file2 = fopen("magneticF.txt","w");
  Vector D[X_MAX][Y_MAX][Z_MAX], E[X_MAX][Y_MAX][Z_MAX], H[X_MAX][Y_MAX][Z_MAX];
  int n;
  double constant, dt = 1e-8, dx = 1e1, ep = 8.854*1e-12, mu = 4*3.141*1e-7;
  float time, omeg;
  char usr_ch;
  Vector sourcePosition;
  sourcePosition.x = X_MAX/2;
  sourcePosition.y = Y_MAX/2;
  sourcePosition.z = Z_MAX/2;
  
  printf( "Enter point source frequency\n" );
  scanf( "%f" , &omeg );
  printf( "Enter total time\n" );
  scanf( "%f" , &time );
  printf( "Default timestep is %e\nWish to change?(y/n) " , dt );
  scanf( " %c" , &usr_ch );
  if( usr_ch == 'y' )
    scanf( "%lf" , &dt );
  printf( "Default spacestep is %lf\nWish to change?(y/n) " , dx );
  scanf( " %s" , &usr_ch );
  if( usr_ch == 'y' )
    scanf( "%lf" , &dx );
  printf( "epsilon_0 is %e mu_0 is %e\n" , ep , mu );
  constant = dt/(dx*pow( ep*mu , 2 ));
  
  Vector zero;
  zero.x = 0.;
  zero.y = 0.;
  zero.z = 0.;
  for( int i = 0 ; i < X_MAX ; i++ ){
    for( int j = 0 ; j < Y_MAX ; j++ ){
      for( int k = 0 ; k < Z_MAX ; k++ ){
        D[i][j][k] = zero;
        E[i][j][k] = zero;
        H[i][j][k] = zero;
      }
    }
  }
  
  n = time/dt;
  while( n-- > 0 ){
    printf("\rValue of n is: %d", n);
    fflush(stdout);
    for( int i = 1 ; i < X_MAX ; i++ ){
      for( int j = 1 ; j < Y_MAX ; j++ ){
        for( int k = 1 ; k < Z_MAX ; k++ ){
          D[i][j][k].x = D[i][j][k].x + constant*( H[i][j][k].z - H[i][j-1][k].z - H[i][j][k].y + H[i][j][k-1].y );
          D[i][j][k].z = D[i][j][k].y + constant*( H[i][j][k].x - H[i][j][k-1].x - H[i][j][k].z + H[i-1][j][k].z );
          D[i][j][k].y = D[i][j][k].z + constant*( H[i][j][k].x - H[i-1][j][k].y - H[i][j][k].x + H[i][j-1][k].x );
          if( n%10 == 0 )
            fprintf( file1 , "%f,%f,%f" , D[i][j][k].x , D[i][j][k].y , D[i][j][k].z );
        }
      }
    }
    
    for( int i = 0 ; i < X_MAX ; i++ ){
      for( int j = 0 ; j < Y_MAX ; j++ ){
        for( int k = 0 ; k < Z_MAX ; k++ ){
          E[i][j][k] = ScalarMul( D[i][j][k] , 2 ); //I have assumed epsilon_relative = 2
        }
      }
    }
    
    Vector source;
    source.x = 0.57735*sin( omeg*n*dt );
    source.y = 0.57735*sin( omeg*n*dt );
    source.z = 0.57735*sin( omeg*n*dt );
    E[(int)sourcePosition.x][(int)sourcePosition.y][(int)sourcePosition.z] = source;
    
    for( int i = 0 ; i < X_MAX-1 ; i++ ){
      for( int j = 0 ; j < Y_MAX-1 ; j++ ){
        for( int k = 0 ; k < Z_MAX-1 ; k++ ){
          H[i][j][k].x = H[i][j][k].x - constant*( E[i][j+1][k].z - E[i][j][k].z - E[i][j][k+1].y + E[i][j][k].y );
          H[i][j][k].z = H[i][j][k].y - constant*( E[i][j][k+1].x - E[i][j][k].x - E[i+1][j][k].z + E[i][j][k].z );
          H[i][j][k].y = H[i][j][k].z - constant*( E[i+1][j][k].y - E[i][j][k].y - E[i][j+1][k].x + E[i][j][k].x );
          if( n%10 == 0 )
            fprintf( file2 , "%f,%f,%f" , H[i][j][k].x , H[i][j][k].y , H[i][j][k].z );
        }
      }
    }
  }
  printf("\n");
  return 0;
}
