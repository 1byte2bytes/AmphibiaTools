#pip3 install opencv-python numpy matplotlib

import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('963.png')

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
mask = cv2.inRange(hsv, (20, 20, 20), (255, 255, 255))

#imask = mask > 0
green = np.zeros_like(img, np.uint8)
for x in range(1080 - 1):
    for y in range(1920 - 1):
        green[x,y] = [255,255,255]

        threshold = np.array([30,30,30])
        matching = np.less_equal(img[x,y], threshold)

        matches = 0
        for match in matching:
            if match == True:
                matches += 1
        
        if matches == 3:
            green[x,y] = img[x,y]
        
#green[imask] = img[imask]
#cv2.imshow("test", green)

#edges = cv2.Canny(img,1920,1080)

plt.subplot(121),plt.imshow(img,cmap = 'gray')
plt.title('Original Image'), plt.xticks([]), plt.yticks([])
#plt.subplot(122),plt.imshow(edges,cmap = 'gray')
#plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(green,cmap = 'gray')
plt.title('Edge Image'), plt.xticks([]), plt.yticks([])

cv2.imshow("", green)
plt.show()