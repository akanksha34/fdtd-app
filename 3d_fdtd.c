#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct Vector{
  float x;
  float y;
  float z;
};

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

# define X_MAX 200
# define Y_MAX 200
# define Z_MAX 200

int main()
{
  Vector[X_MAX][Y_MAX][Z_MAX] E, D, H;
  int n;
  double dt = 1e-8, dx = 1e1, ep = 8.854*1e-12, mu = 4*3.141*1e−7, const;
  float time, omeg;
  char usr_ch;
  Vector sourcePosition;
  sourcePosition.x = X_MAX/2;
  sourcePosition.y = Y_MAX/2;
  sourcePosition.z = Z_MAX/2;
  
  printf( "Enter point source frequency " );
  scanf( "%f\n" , &omeg );
  printf( "Enter total time " );
  scanf( "%f\n" , &time );
  printf( "Default timestep is %lf\nWish to change?(y/n) " , dt );
  scanf( "%s\n" , &usr_ch );
  if( usr_ch == 'y' )
    scanf( "%lf\n" , &dt );
  printf( "Default spacestep is %lf\nWish to change?(y/n) " , dx );
  scanf( "%s\n" , &usr_ch );
  if( usr_ch == 'y' )
    scanf( "%lf\n" , &dx );
  printf( "epsilon_0 is %lf mu_0 is %lf" , ep , mu );
  const = dt/(dx*pow( ep*mu , 2 ));
  
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
    for( int i = 1 ; i < X_MAX ; i++ ){
      for( int j = 1 ; j < Y_MAX ; j++ ){
        for( int k = 1 ; k < Z_MAX ; k++ ){
          D[i][j][k].x = D[i][j][k].x + const*( H[i][j][k].z - H[i][j-1][k].z - H[i][j][k].y + H[i][j][k-1].y );
          D[i][j][k].z = D[i][j][k].x + const*( H[i][j][k].x - H[i][j][k-1].x - H[i][j][k].z + H[i-1][j][k].z );
          D[i][j][k].y = D[i][j][k].x + const*( H[i][j][k].x - H[i-1][j][k].y - H[i][j][k].x + H[i][j-1][k].x );
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
    E[sourcePosition.x][sourcePosition.y][sourcePosition.z] = source;
    
    for( int i = 0 ; i < X_MAX-1 ; i++ ){
      for( int j = 0 ; j < Y_MAX-1 ; j++ ){
        for( int k = 0 ; k < Z_MAX-1 ; k++ ){
          H[i][j][k].x = H[i][j][k].x - const*( E[i][j+1][k].z - E[i][j][k].z - E[i][j][k+1].y + E[i][j][k].y );
          H[i][j][k].z = H[i][j][k].x - const*( E[i][j][k+1].x - E[i][j][k].x - E[i+1][j][k].z + E[i][j][k].z );
          H[i][j][k].y = H[i][j][k].x - const*( E[i+1][j][k].y - E[i][j][k].y - E[i][j+1][k].x + E[i][j][k].x );
        }
      }
    }
  }
  return 0;
}
