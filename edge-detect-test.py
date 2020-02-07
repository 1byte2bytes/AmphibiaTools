#pip3 install opencv-python numpy matplotlib tqdm

import cv2
import numpy as np
import time
from matplotlib import pyplot as plt
from tqdm import tqdm

start_time = time.time()

img = cv2.imread('963.png')

#hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
#mask = cv2.inRange(hsv, (20, 20, 20), (255, 255, 255))

#imask = mask > 0
# Make an array like img
green = np.zeros_like(img, np.uint8)
# Black threshold limit
threshold = np.array([30,30,30])
# Progress bar
#pbar = tqdm(total=1920*1080)
for x in range(1080 - 1):
    for y in range(1920 - 1):
        #pbar.update(1)
        # Use NumPy to find if R, G, and B values <= threshold
        matching = np.less_equal(img[x,y], threshold)

        # Check how many pixel values match with the threshold
        matches = sum(bool(match) for match in matching)
        
        # If all the pixels match, copy the pixel to our edge buffer
        # Otherwise fill with white
        if matches == 3:
            green[x,y] = img[x,y]
        else:
            green[x,y] = [255,255,255]
        
#green[imask] = img[imask]
#cv2.imshow("test", green)

#edges = cv2.Canny(img,1920,1080)

print("--- %s seconds ---" % (time.time() - start_time))

plt.subplot(121),plt.imshow(img,cmap = 'gray')
plt.title('Original Image'), plt.xticks([]), plt.yticks([])
#plt.subplot(122),plt.imshow(edges,cmap = 'gray')
#plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(green,cmap = 'gray')
plt.title('Edge Image'), plt.xticks([]), plt.yticks([])

cv2.imshow("", green)
plt.show()