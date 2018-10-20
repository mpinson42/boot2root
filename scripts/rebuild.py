#!/usr/bin/python3

import sys
import os


# this function just return the files names, it remove all hiden files or
# files who not finish by .pcap
def getfiles(path):
    if path[-1] != '/':
        path.append('/')
    files = []
    for f in os.listdir(path):
        # ignore hiden files
        if f[0] == '.':
            continue
        # just keep .pcap ones
        if f[-5:] != ".pcap":
            continue
        files.append(path + f)
    files.sort()
    return (files)

# logic of this stuff:
# use a main_content dict, each key is a file index
# each .pcap file has a filename with index at the end, the data are
# ordered in integer order, so we open each files, append the content on the
# right index then get the keys list, pass it into integer then sort it and
# rebuild the main.c file from them

def rebuild(files):
    main_content = {}
    for file in files:
        with open(file, 'r') as f:
            content = f.read()
            lines = content.split('\n')
            index = lines[-1][6:]
            if main_content.get(index):
                main_content[index] += content
            else:
                main_content[index] = content + "\n"
            f.close()

    keys = []
    for k in main_content.keys():
        keys.append(int(k))
    keys.sort()


    if os.path.exists('main.c'):
        os.unlink('main.c')
    with open('main.c', 'w') as main:
        for k in keys:
            main.write(main_content[str(k)])
        main.close()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: {} /path/to/read/pcap/files".format(sys.argv[0]))
        sys.exit(1)
    files = getfiles(sys.argv[1]);
    print(len(files), "files to read")
    rebuild(files)
    print("done")
