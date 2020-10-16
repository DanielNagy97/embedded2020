fp = open('Fonts.txt', 'r')
out = open('Output.txt', 'a')

while(1):
    line = fp.readline()
    if (line == ""):
        print("File finished")
        break;
    if (line[:-1].isdigit()):
        hex_form = hex(int(line[:-1],2))
        out.write(str(hex_form)+", ")
    else:
        out.write(line)
fp.close()
out.close()

