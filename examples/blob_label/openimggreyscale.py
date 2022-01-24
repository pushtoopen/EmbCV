from PIL import Image
import sys
filename = sys.argv[1]
im = Image.open(filename).convert('L')
data = list(im.getdata())
data = [(str(x)+",") for x in data]
dataout = "".join(data).rstrip(',')
#print(im.format, im.size, im.mode)
#print(dataout)
text_file = open("tempcsv.txt", "w")
text_file.write(dataout)
text_file.close
sizeparams = str(im.size).replace(")","")
sizeparams = sizeparams.replace("(","")
sizeparams = sizeparams.replace(" ","")
sizeparams = sizeparams + "," + str(len(im.getbands())) 
print(sizeparams) #prints height, width, and number of channels
#im.show()