#!/usr/bin/python3

import sys
import numpy


def warning_msg(msg):
    print(msg, file=sys.stderr)

## Construct DFA from pattern. DFA is described by transition
## function which is defined in terms of 2d table such that 1'st
## dimension is input character and 2'nd dimension is current
## state of automaton.
##
def construct_dfa(patt):

    ## Initial conditions
    dfa = numpy.zeros((256, len(patt)));
    dfa[ord(patt[0])][0] = 1

    ## In order to find state we should jump to in case of mismatch in
    ## j's character, we should emulate substring patt[1:j - 1] on the
    ## DFA. We can do it in more optimal way by saving state of
    ## emulation for patt[1:j - 2] from previouse iteration and look
    ## at the next state only for the last (j - 1)'s character. Here
    ## (x) is the state where we would be after [1:j - 2] substring
    ## simulation.
    ##
    x = 0

    ## Set values for the rest of transition function table.
    for i in range(0, len(patt)):
        ## Jump to next state in case of match
        dfa[ord(patt[i])][i] = i + 1

        ## Handle mismatch. Emulate substring patt[1:j - 1] on the DFA
        ## in order to find state to jump to.
        for j in range(0, 256):
            dfa[j][i] = dfa[j][x]

        ## Update saved state
        x = dfa[ord(patt[i])][x]
    return dfa

def main():
    if len(sys.argv) != 3:
        warning_msg("unexpected command line arguments")
        warning_msg("try %s (text) (patter)" % sys.argv[0])
        sys.exit(1)

    text  = sys.argv[1];
    patt  = sys.argv[2];
    dfa   = construct_dfa(patt)
    state = 0;
    
    for i in range(0, len(text)):
        state = dfa[ord(text[i])][state];

        ## Check, is automaton in final state?
        if state == len(patt):
            print("match found at %d" % i)
            print("ihha! 1 2 3 4")
            sys.exit(0)

    print("match not found")
    print("...")
    sys.exit(0)
        
if __name__ == "__main__":
    main()
    
