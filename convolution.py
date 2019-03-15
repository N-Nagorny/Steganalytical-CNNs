import numpy as np
import cv2
from skimage.exposure import rescale_intensity

height, width = 256, 256
folders_amount = 10
input_cover_path = 'BOSSbase_1.01-256/cover/'
input_stego_path = 'BOSSbase_1.01-256/stego_wow_0.4/'
output_cover_path = 'BOSSbase_1.01-256-convolved/cover/'
output_stego_path = 'BOSSbase_1.01-256-convolved/stego_wow_0.4/'

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

for folder_number in range(0, folders_amount):
    for file_number in range(folder_number * 1000 + 1, (folder_number + 1) * 1000 + 1):
        cover_image = cv2.imread(input_cover_path + str(folder_number) + '/' + str(file_number) + '.pgm', cv2.IMREAD_GRAYSCALE)
        cover_conv_image = convolve(cover_image, kernel)
        cv2.imwrite(output_cover_path + str(file_number) + '.ppm', np.einsum('kij->ijk', np.array([cover_conv_image, cover_conv_image, cover_conv_image])))

        stego_image = cv2.imread(input_stego_path + str(folder_number) + '/' + str(file_number) + '.pgm', cv2.IMREAD_GRAYSCALE)
        stego_conv_image = convolve(stego_image, kernel)
        cv2.imwrite(output_stego_path + str(file_number) + '.ppm', np.einsum('kij->ijk', np.array([stego_conv_image, stego_conv_image, stego_conv_image])))

        print(str(file_number) + '.ppm/' + str(folders_amount * 1000))
