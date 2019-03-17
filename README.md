# Usage

1. Execute *./prepare_images.sh* to create training, validation and test datasets from *BOSSbase_1.01.zip* using WOW embedding algorithm. This script unpacks *BOSSbase_1.01.zip*, takes selected number of images, resizes them if it is needed and does embedding. The result of work of this script is a united dataset which can be splitted into training, validation and test sets. Number of images is equal to two times the selected number (one half is cover images and another one is stego images).

You can set size of the square images using *--side-size=<desired size by one side>* command line argument. Also, you can force the script to download the archives with the image DB and the WOW algorithm implementation from the web instead of using a local archives with the *--download* command line argument. *--set-size=<images thousands number>* allows to set how many images from the DB should be selected for embedding. *--payload=<payload value>* stands for a payload size of WOW algorithm.
 
 * ./prepare_images.sh --side-size=256 --set-size=1 --payload=1.6 *

## GNCNN
2. Open *convolution.py* and assign *input_cover_path*, *input_stego_path*, *output_cover_path* and *output_stego_path* variables with your paths. Also, be sure that folders by *output_cover_path* and *output_stego_path* exist.
