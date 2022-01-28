# BLOB_LABEL_METHODS
Blob Labeling Methods

To understand what this algorithm does, see: https://en.wikipedia.org/wiki/Connected-component_labeling
This method thresholds the image using the OTSU method, then binarizes the images and labels each detected blob.
This method checks all corners for blobs, then top row, left column, then right column. 
Then it scans through the entire picture looking to ID blobs and will label pixels then it IDs any surrounding pixels as currently part of a blob.
Finally it scans through the picture one more time and checks for missing gaps in blob IDs and calculates the centroids of each blob and stores it in the "blobpos" struct.
This is essentially a two-pass algorithm thats effectively a breadth-first search.
This struct can then be referenced elswhere for post-processing.

This method was made to reduce or eliminate copying of the input image to save RAM. With that in mind, if an image is desired to be kept
after processing, its best to feed in a copy of the image buffer as whatever buffer is passed in will be overwritten during processing.

Current weakness is that this can only ID 255 blobs and does not have the cleanup methods in place to handle overrun of the blob ID.
All inputs to this must be unsigned 8 bit integers (0-255) for the pixel values.

