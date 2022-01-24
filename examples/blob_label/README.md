# BLOB_LABEL_EXAMPLE
Blob Labeling Example

This is an example of how to feed an imageinto the Blob Label methods.
Normally in an embedded application, the camera driver will collect an image and format it as an rgb or greyscale image.
This example calls python to pull in an example image, convert it to greyscale, then put it in a file to be read.
This example then fills the img_buf struct with the relevant info and passes it into the actual blob labelling methods.
Finally the example puts it out to a csv file to open with a datasheet program to easily view what it's putting out.


