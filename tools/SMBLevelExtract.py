# by SpiderDave
import os, sys

PY3 = sys.version_info > (3,)
if not PY3:
    print("This script requires Python 3.")
    exit()

import re, textwrap
from binascii import hexlify, unhexlify

AreaTypes = ["Water", "Ground", "Underground", "Castle"]

if not len(sys.argv):
    print("Error: no file specified.")
    exit()

out=";GAME LEVELS DATA\n\n"
for filename in sys.argv[1:]:
    try:
        file = open(filename, "rb")
    except:
        print("Error: could not open file.")
        exit()
    
    # read data from .nes file
    fileData = file.read()[0x10:]
    
    #greated 40e2ffe5fff9      1fffa
    #original 1f82800080f0    7ffa
    
    romType = ""
    adjust = 0
    
    if hexlify(fileData[0x1fff9:0x1fff9+6]) == b'40e2ffe5fff9':
        romType = "GreatEd"
        adjust = 0x6000
        #print("; Detected: GreatEd\n")
    if hexlify(fileData[0x7ff9:0x7ff9+6]) == b'1f82800080f0':
        #print("; Detected: original\n")
        romType = "original"
    
    # search for StoreStyle subroutine
    m = re.search(re.escape(unhexlify('8d3307a5e718690285e7a5e8690085e860')), fileData)
    
    # assume WorldAddrOffsets directly follows it
    # assume there are 8 entries (one for each world)
    WorldAddrOffsets = m.start()+0x11
    
    #out+=";WorldAddrOffsets={0:05x}\n\n".format(WorldAddrOffsets)
    
    # Assume AreaAddrOffsets immediately follows WorldAddrOffsets
    AreaAddrOffsets = WorldAddrOffsets + 8
    
    # part of GetAreaDataAddrs
    m = re.search(re.escape(unhexlify('a8ad5007291f8d')), fileData)
    
    # Get all these based on the above
    EnemyAddrHOffsets = fileData[m.start()+0x0a] + fileData[m.start()+0x0b] * 0x100 - 0x8000
    EnemyDataAddrLow = fileData[m.start()+0x12] + fileData[m.start()+0x13] * 0x100 - 0x8000
    EnemyDataAddrHigh = fileData[m.start()+0x17] + fileData[m.start()+0x18] * 0x100 - 0x8000
    AreaDataHOffsets = fileData[m.start()+0x1f] + fileData[m.start()+0x20] * 0x100 - 0x8000
    AreaDataAddrLow = fileData[m.start()+0x27] + fileData[m.start()+0x28] * 0x100 - 0x8000
    AreaDataAddrHigh = fileData[m.start()+0x2c] + fileData[m.start()+0x2d] * 0x100 - 0x8000
    
    #formatString = ";EnemyAddrHOffsets={0:05x}\n;EnemyDataAddrLow={1:05x}\n;EnemyDataAddrHigh={2:05x}\n;AreaDataHOffsets={3:05x}\n;AreaDataAddrLow={4:05x}\n;AreaDataAddrHigh={5:05x}\n"
    #out+=formatString.format(EnemyAddrHOffsets,EnemyDataAddrLow,EnemyDataAddrHigh,AreaDataHOffsets,AreaDataAddrLow,AreaDataAddrHigh)
    
    #print(WorldAddrOffsets, AreaAddrOffsets, EnemyAddrHOffsets, EnemyDataAddrLow, EnemyDataAddrHigh, AreaDataHOffsets, AreaDataAddrLow, AreaDataAddrHigh)
    
    file.seek(WorldAddrOffsets+0x10)
    wOffset = file.read(8)
    #out+=";WorldAddrOffsets:"
    #for n in wOffset:
    #    out+=" {0:02x}".format(n)
    #out+="\n\n"
    
    file.seek(EnemyAddrHOffsets+0x10)
    eOffsets = file.read(4)

    #out+=";EnemyAddrHOffsets:"
    #for n in eOffsets:
    #    out+=" {0:02x}".format(n)
    #out+="\n\n"


    file.seek(AreaDataHOffsets+0x10)
    aDataOffsets = file.read(4)
    
    #out+=";AreaDataHOffsets:"
    #for n in aDataOffsets:
    #    out+=" {0:02x}".format(n)
    #out+="\n\n"
    
    areas = [{},{},{},{}]
    for w in range(0,8):
        for a in range(0,100):
            file.seek(AreaAddrOffsets+wOffset[w]+a+0x10)
            b = file.read(1)[0]
            #out+="${0:02x}".format(b)
            if b >= 0x80:
                b = b - 0x80
            if b >= 0x60:
                b = b - 0x60
                aType = 3
            elif b >= 0x40:
                b = b - 0x40
                aType = 2
            elif b >= 0x20:
                b = b - 0x20
                aType = 1
            else:
                aType = 0
            #print(aType,b)
            
            areas[aType][b] = True
            
            if aType == 3:
                break
    
    areas[0][0x00] = True #water area (5-2/6-2)    - 00
    areas[0][0x02] = True #water area (8-4)        - 02
    areas[1][0x0f] = True #warp zone area (4-2)    - 2f
    areas[1][0x0b] = True #cloud area 1 (day)      - 2b
    areas[1][0x14] = True #cloud area 2 (night)    - 34
    areas[2][0x02] = True #underground bonus area  - c2
    
    # Fill in any gaps
    for area in range(0,4):
        for a in range(0, max(areas[area])+1):
            if a not in areas[area].keys(): 
                areas[area][a]=True
    
    out+="WorldAddrOffsets:\n"
    out+="      .db World1Areas-AreaAddrOffsets, World2Areas-AreaAddrOffsets\n"
    out+="      .db World3Areas-AreaAddrOffsets, World4Areas-AreaAddrOffsets\n"
    out+="      .db World5Areas-AreaAddrOffsets, World6Areas-AreaAddrOffsets\n"
    out+="      .db World7Areas-AreaAddrOffsets, World8Areas-AreaAddrOffsets\n"
    out+="\n"
    out+="AreaAddrOffsets:\n"
    
    for w in range(0,8):
        out+="World{0:d}Areas: .db ".format(w+1)
        c = 0;
        for a in range(0,100):
            file.seek(AreaAddrOffsets+wOffset[w]+a+0x10)
            b = file.read(1)[0]
            c = c+1
            out+="${0:02x}".format(b)
            if c in range (7,8):
                break
            else:
                out+=", "
        out+="\n"
    out+="\n"
    
    enemyOrArea = ["Enemy","Area"]
    enemyOrArea2 = ["EnemyAddr","AreaData"]
    eOrA = ["E","L"]
    for j in range(0,2):
        out+="{0:s}HOffsets:\n".format(enemyOrArea2[j])
        out+="      .db {0:s}DataAddrLow_WaterStart - {0:s}DataAddrLow          ; Water\n".format(enemyOrArea[j])
        out+="      .db {0:s}DataAddrLow_GroundStart - {0:s}DataAddrLow         ; Ground\n".format(enemyOrArea[j])
        out+="      .db {0:s}DataAddrLow_UndergroundStart - {0:s}DataAddrLow    ; Underground\n".format(enemyOrArea[j])
        out+="      .db {0:s}DataAddrLow_CastleStart - {0:s}DataAddrLow         ; castle\n".format(enemyOrArea[j])
        
        for i in range(0,2):
            if i==0:
                out+="\n{0:s}DataAddrLow:\n".format(enemyOrArea[j])
            else:
                out+="\n{0:s}DataAddrHigh:\n".format(enemyOrArea[j])
            
            for area in range(0,4):
                out+="      ; {0:s}\n".format(AreaTypes[area])
                
                if i==0:
                    out+="      {0:s}DataAddrLow_{1:s}Start:\n".format(enemyOrArea[j], AreaTypes[area])
                
                out+="      .db "
                for a in range(0, max(areas[area])+1):
                    if i==0:
                        out+="<"
                    else:
                        out+=">"

                    out+="{0:s}_{1:s}Area{2:d}".format(eOrA[j], AreaTypes[area], a+1)
                    if a == max(areas[area]):
                        out+="\n"
                    elif a % 6==5:
                        out+="\n      .db "
                    else:
                        out+=", "
        out+="\n"
    
    terminator=[0xff,0xfd]
    for j in range(0,2):
        for area in range(0,4):
            for a in range(0, max(areas[area])+1):
                out+="{0:s}_{1:s}Area{2:d}:\n".format(eOrA[j], AreaTypes[area], a+1)
                
                if j==0:
                    aEnemy = fileData[EnemyDataAddrHigh+eOffsets[area]+a] * 0x100 + fileData[EnemyDataAddrLow+eOffsets[area]+a]
                    
                    file.seek(aEnemy -0x8000+ adjust + 0x10)
                    
                    #out+=";{0:05x}\n".format(aEnemy - 0x8000)
                    
                    eData = file.read(1000)
                    eData = eData.split(bytes(0xff))[0]
                    eData = bytearray(eData)
                    eData.append(0xff)
                    data = eData
                else:
                    aArea = fileData[AreaDataAddrHigh+aDataOffsets[area]+a] * 0x100 + fileData[AreaDataAddrLow+aDataOffsets[area]+a]
                    
                    file.seek(aArea -0x8000+ adjust + 0x10)
                    aData = file.read(1000)
                    aData = aData.split(bytes(0xfd))[0]
                    aData = bytearray(aData)
                    aData.append(0xfd)
                    data = aData
                out+="      .db "
                for i in range(0, len(data)):
                    out+="${0:02x}".format(data[i])
                    if data[i] == terminator[j]:
                        out+="\n"
                        break
#                    if data[i] == terminator[j]:
#                        out+="\n      .db ${0:02x}\n\n".format(terminator[j])
#                        break
#                    else:
#                        out+="${0:02x}".format(data[i])
                        
                    if i == len(data)-1:
                        out+="\n"
                    elif i % 10==9:
                        out+="\n      .db "
                    else:
                        out+=", "
                    
                out+="\n"
    
    outputFileName = os.path.join(sys.path[0], filename + '.asm')
    print("Writing to file: {0}".format(outputFileName)) 
    outputFile = open(os.path.join(sys.path[0], filename + '.asm'), 'w')
    print(out, file = outputFile)
    outputFile.close()
    


file.close()