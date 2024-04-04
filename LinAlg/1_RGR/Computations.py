from math import log
from math import *

x = [100, 150, 200, 250, 300, 350]
y = [9.6, 10.4, 11.2, 12.1, 12.7, 13.2]

s1 = lambda x: 0.015*x + 8.216
s2 = lambda x: 2.8391*x**0.2617
s3 = lambda x: 8.5626*e**(0.0013*x)
s4 = lambda x : -1.6429e-05 * x**2 + 0.0221*x + 7.5043

s = 0
for i in range(6):
    s += (y[i] - s4(x[i]))**2
print(s)

##x4 = [x_**4 for x_ in x]
##x3 = [x_**3 for x_ in x]
##x2 = [x_**2 for x_ in x]
##
##
##log_x = [log(x_) for x_ in x]
##log_y = [log(y_) for y_ in y]
##
##x_i4 = round(sum(x4), 4)
##x_i3 = round(sum(x3), 4)
##x_i2 = round(sum(x2), 4)
##x_i = round(sum(x), 4)
##x_iy_i = round(sum([x[i]*y[i] for i in range(6)]), 4)
##x_i2y_i = round(sum([x2[i]*y[i] for i in range(6)]), 4)
##y_i = round(sum(y), 4)
##
##
##print(x_i4)
##print(x_i3)
##print(x_i2)
##print(x_i)
##print(x_iy_i)
##print(x_i2y_i)
##print(y_i)

##print()
##
##print(round(x_i2 * 6 - x_i**2, 4))
##print(round(x_iy_i*6 - x_i*y_i, 4))
##print(round(x_i2*y_i - x_iy_i*x_i, 4))




