#!/usr/bin/python3

import sys


def warning_message(msg):
    print(msg, file=sys.stderr)

def main():

    if len(sys.argv) != 3:
        warning_message("unexpected arguments");
        warning_message("try %s (text) (pattern)" % sys.argv[0])
        return 1

    text    = sys.argv[1]
    pattern = sys.argv[2]

    if len(text) < len(pattern):
        print("no match found")
        return 0

    for i in range(0, len(text) - len(pattern) + 1):
        for j in range(0, len(pattern)):
            if pattern[j] != text[i + j]:
                break;
        else:
            print("match at %d" % i)
            return 0
        
    print("no match found")
    return 0
    
if __name__ == "__main__":
    main()

    
