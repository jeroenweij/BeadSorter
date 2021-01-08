#/bin/python3
from PIL import Image

im = Image.open('font.png') # Can be many different formats.
pix = im.load()
width = im.width
height = im.height
print (width)  
print (height)  
print (pix[0,0])  
print (pix[1,1]) 
print (pix[2,2]) 
#arr = [[[0]*12]*18]*6 
arr = [[[0 for _ in range(12)] for _ in range(18)] for _ in range(6)]
x=5
y=5
tresh = 200

l=0
detected = False
detected2 = False
yb=0
xb=0
        
for h in range(height):
    if pix[x,h] == (255, 0, 0, 255) :
        yb=-1
        if detected:
            print("Start new line")
            l = l+1
            detected = False
        continue
    yb=yb+1
    i=0    
    for w in range(width):
    
        #print("w {} h {} i {} xb {} yb {} P {}".format(w,h,i, xb+1,yb , pix[w,h]))
        if pix[w,h] == (255, 0, 0, 255):
            xb=-1
            if detected2:
                i=i+1
                detected2 = False
        else:
            xb=xb+1
            if pix[w,h] == (0, 0, 0, 255):
                detected = True
                detected2 = True
                
            if pix[w,h][0] > tresh and pix[w,h][1] > tresh and pix[w,h][2] > tresh:
                detected = True
                detected2 = True
                mask = 1 << (15-yb)
                print (pix[w,h])
                print("w {} h {} i {} xb {} yb {} P {}".format(w,h,i, xb+1,yb , pix[w,h]))
                print("WHOO {} {} {} {} |= {}".format(l, i, xb, yb, mask))
                arr[l][i][xb] |= mask #pix[w,h][0]

#exit(0)
print()
print()
print()

c = ord(' ')
for i in range(6):
    for y in range(18):
        s="{"
        first=True
        
        for x in range(12):
            if not first:
                s+=", "
            first=False
            s += hex(arr[i][y][x])
        
        s+="}, "
        s+= "// \"{}\"".format(chr(c))
        c=c+1
        print(s)
        
        
        
        
