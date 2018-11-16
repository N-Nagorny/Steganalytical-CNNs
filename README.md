# Usage

1. Execute *./prepare_images.sh* to create a training and a test sets from *BOSSbase_1.01.zip* using WOW embedding algorithm.

You can set size of the processed square images using *--size=<desired size by one side>* command line argument. Also you can force the script to download the archives with the image DB and the WOW algorithm implementation from the web instead of using a local archive with the *--download* command line argument.

# TODO

1. Implement the archives' hash sum checking.
