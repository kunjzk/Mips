import os
import subprocess
import sys

with open('testbench/tests.txt', 'r') as f:
    x=f.read().splitlines()

for i in range(len(x)):
    y=x[i].split(',')

#y[0] --> testID
#y[1] --> username
#y[2] --> instruction
#y[3] --> path
#y[4] --> Message
#y[5] --> Expected return code
#y[6] --> expected I/O

    proc = subprocess.Popen(["./"+ sys.argv[1], "testbench/binaries/"+y[3]], stdin = subprocess.PIPE, stdout = subprocess.PIPE)

    # TestID, Instruction, P/F, Username, Message
    output_message = [y[0], y[2], "Placeholder", y[1], y[4]]

    #for I/O
    Output_Passed = True
    if(y[6] != "-/-"):
        I = y[6].split('/')[0]
        O = '\x00' if y[6].split('/')[1] == "0" else y[6].split('/')[1]

        if(I == "-"): #only expecting output
            result = proc.communicate()[0]
            Output_Passed = True if result == O else False

        else: #no output, supply input
            if(I=="EOF"):
                proc.communicate()
            else:
                proc.communicate(I)

    retcode = proc.wait()

    if(retcode == int(y[5]) and Output_Passed):
        output_message[2] = "Pass"
    else:
        output_message[2] = "Fail"

    output_message = ',\t'.join(map(str,output_message))
    print output_message
