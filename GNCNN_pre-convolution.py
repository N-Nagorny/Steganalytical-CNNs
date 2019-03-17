import numpy as np
import cv2
import os
import glob
import threading
from skimage.exposure import rescale_intensity

height, width = 256, 256
input_cover_path = 'BOSSbase_1.01-256/cover/'
input_stego_path = 'BOSSbase_1.01-256/stego_wow_1.6/'
output_cover_path = 'BOSSbase_1.01-256-convolved/cover/'
output_stego_path = 'BOSSbase_1.01-256-convolved/stego_wow_1.6/'

if not os.path.exists(output_cover_path):
    os.makedirs(output_cover_path)
if not os.path.exists(output_stego_path):
    os.makedirs(output_stego_path)

def convolve(image, kernel):
    i_width, i_height = image.shape[0], image.shape[1]
    k_width, k_height = kernel.shape[0], kernel.shape[1]
    pad = k_width // 2
    output = np.zeros((i_width - 2*pad, i_height - 2*pad), dtype="float32")

    for y in range(pad, i_height - pad):
        for x in range(pad, i_width - pad):
            weighted_pixel_sum = 0
            roi = image[y - pad:y + pad + 1, x - pad:x + pad + 1]
            for ky in range(0, k_height):
                for kx in range(0, k_width):
                    weighted_pixel_sum += roi[ky, kx] * kernel[ky, kx]
            output[y - pad, x - pad] = weighted_pixel_sum

    output = rescale_intensity(output, in_range=(0, 255))
    output = (output * 255).astype("uint8")
 
    # return the output image
    return output

kernel = np.array((
    [-1,  2,  -2,  2, -1],
    [ 2, -6,   8, -6,  2],
    [-2,  8, -12,  8, -2],
    [ 2, -6,   8, -6,  2],
    [-1,  2,  -2,  2, -1]), dtype="float")
kernel = np.divide(kernel, 12)

def process_batch (input_path, output_path):
    for filename in glob.glob(os.path.join(input_path, '*.pgm')):
        orig_image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
        conv_image = convolve(orig_image, kernel)
        cv2.imwrite(output_path + os.path.splitext(os.path.basename(filename))[0] + '.ppm', np.einsum('kij->ijk', np.array([conv_image, conv_image, conv_image])))
        print(filename)

class myThread (threading.Thread):
   def __init__(self, input_path, output_path):
      threading.Thread.__init__(self)
      self.input_path = input_path
      self.output_path = output_path
   def run(self):
      process_batch(self.input_path, self.output_path)

# Create new threads
thread1 = myThread(input_cover_path, output_cover_path)
thread2 = myThread(input_stego_path, output_stego_path)

# Start new Threads
thread1.start()
thread2.start()
