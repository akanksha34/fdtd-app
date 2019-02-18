from __future__ import division
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

#############################################
#             Initialising                  #
#############################################
X = np.arange( 0 , 60 , 1 )
Y = np.arange( 0 , 60 , 1 )

#############################################
#            Collecting Data                #
#############################################
fileE = open("electric.txt", 'r')
fileH = open("magnetic.txt", 'r')

[ ele , mag ] = [ [[],[],[]] , [[],[],[]] ]
for e in fileE:
  e = e.split(',')
  ele[0].append(float(e[0]))
  ele[1].append(float(e[1]))
  ele[2].append(float(e[2]))
for h in fileE:
  h = h.split(',')
  mag[0].append(float(h[0]))
  mag[1].append(float(h[1]))
  mag[2].append(float(h[2]))
  
#############################################
#            Processing Data                #
#############################################
fig = plt.figure()
ax  = fig.add_subplot( 111, projection=3d )

Axis3D.plot_surface( X , Y , ele )
