#! /usr/bin/env python3

import argparse
import sys
from milling_nix import *

if __name__ == '__main__':
    #Parse options
    parser = argparse.ArgumentParser(sys.argv[0], description="Send a milling job to a printer-like device.")
    parser.add_argument('input_file', metavar='EGX-FILE', type=str, \
                        help='A Gerber drill layer', default=None, nargs='?')
    parser.add_argument('-d', '--printer', dest='printer', default=None, help='Which printer-like device to send the job')
    parser.add_argument('-i', '--interactive', dest='interactive', action='store_true', default=False, help='Use the interactive ui')

    parser.add_argument('--nowarning', action="store_false", dest="warning", \
                      default=True, help="Don't print warnings")
    parser.add_argument('--nofix', action="store_false", dest="fix", \
                      default=True, help="Don't output 'needs fixing' messages")
    parser.add_argument('--noerror', action="store_false", dest="error", \
                      default=True, help="Don't print errors")
    args = parser.parse_args()

    cnf = dict()
    cnf['input_file']  = args.input_file
    cnf['printer']     = args.printer
    cnf['interactive'] = args.interactive
    cnf['warning']     = args.warning
    cnf['fix']         = args.fix
    cnf['error']       = args.error

    if len(sys.argv) == 1:
      print("No parameters used, starting interactive mode.")
      print("Use -h for help.")

    if cnf['interactive'] or (len(sys.argv) == 1):
      interactive()
    else:
      if cnf['printer'] == None:
        cnf['printer'] = default_printer()
      mill(cnf['printer'], cnf['input_file'])
