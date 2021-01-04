from imageio import imread
from scipy import misc

import matplotlib.pylab as plot
import numpy as np
import numpy.core.fromnumeric as np2
import math

import scipy.ndimage.filters as filters
import scipy.ndimage as ndimage


image = imread('pentagon.png')

print("image shape: ", image.shape)

plot.imshow(image)
plot.savefig("image1.png")
plot.close()

img_shape = image.shape

x_max = img_shape[0]
y_max = img_shape[1]

theta_max = 1.0 * math.pi
theta_min = 0.0

r_min = 0.0
r_max = math.hypot(x_max, y_max)

r_dim = 200
theta_dim = 300

hough_space = np.zeros((r_dim, theta_dim))

for x in range(x_max):
    for y in range(y_max):
        if image[x,y,0] == 255: continue
        for itheta in range(theta_dim):
            theta = 1.0 * itheta * theta_max / theta_dim
            r = x * math.cos(theta) + y * math.sin(theta)
            ir = r_dim * (1.0 * r) / r_max
            hough_space[int(ir), int(itheta)] = hough_space[int(ir), int(itheta)] + 1

plot.imshow(hough_space)
plot.xlim(0, theta_dim)
plot.ylim(0, r_dim)

tick_locs = [i for i in range(0, theta_dim, 40)]
tick_lbls = [round((1.0 * i * theta_max) / theta_dim, 1) for i in range(0, theta_dim, 40)]
plot.xticks(tick_locs, tick_lbls)

tick_locs = [i for i in range(0, r_dim, 20)]
tick_lbls = [round((1.0 * i * r_max) / r_dim, 1) for i in range(0, r_dim, 20)]
plot.yticks(tick_locs, tick_lbls)

plot.xlabel(r'Theta')
plot.ylabel(r'r')
plot.title('Hough Space')

plot.savefig("hough_space_r_theta.png")

plot.close()

neighborhood_size = 20
threshold = 140

data_max = filters.maximum_filter(hough_space, neighborhood_size)
maxima = (hough_space == data_max)

data_min = filters.minimum_filter(hough_space, neighborhood_size)
diff = ((data_max - data_min) > threshold)
maxima[diff == 0] = 0

labeled, num_objects = ndimage.label(maxima)
slices = ndimage.find_objects(labeled)

x, y = [], []
for dy, dx in slices:
    x_center = (dx.start + dx.stop - 1) / 2
    x.append(x_center)
    y_center = (dy.start + dy.stop - 1) / 2
    y.append(y_center)

print(x)
print(y)

plot.imshow(hough_space)
plot.savefig('hough_space_i_j.png')

plot.autoscale(False)
plot.plot(x, y, 'ro')
plot.savefig('hough_space_maximas.png')

plot.close()
lineCount = 1

for i, j in zip(y, x):
    r = round((1.0 * i * r_max) / r_dim, 1)
    theta = round((1.0 * j * theta_max) / theta_dim, 1)

    fig, ax = plot.subplots()

    ax.imshow(image)
    ax.autoscale(False)

    px = []
    py = []
    for i in range(-y_max - 40, y_max + 40, 1):
        px.append(math.cos(-theta) * i - math.sin(-theta) * r)
        py.append(math.sin(-theta) * i + math.cos(-theta) * r)
    ax.plot(px, py, linewidth=10)

    plot.savefig("image_line_" + "%02d" % lineCount + ".png", bbox_inches='tight')

    # plt.show()

    plot.close()

    lineCount = lineCount + 1
