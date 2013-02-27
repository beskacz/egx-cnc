#! /usr/bin/env python3

import gcode
import argparse
import sys

if __name__ == '__main__':
    #Parse options
    parser = argparse.ArgumentParser(sys.argv[0], description="Generate a milling script for Roland EGX engravers from a G-Code script.")
    parser.add_argument('gcode_file', metavar='GCODE-FILE', type=str, \
                        help='A G-Code script file')
    parser.add_argument("-o", '--out', dest='filename', \
                      help='Write output to file', metavar="FILE", default='out.egx')
    parser.add_argument('--no-warnings', action="store_false", dest="warning", \
                      default=True, help="Don't print warnings")
    parser.add_argument('--no-info', action="store_false", dest="info", \
                      default=True, help="Don't print info")
    parser.add_argument('--no-fix', action="store_false", dest="fix", \
                      default=True, help="Don't output 'needs fixing' messages")
    parser.add_argument('--no-error', action="store_false", dest="errors", \
                      default=True, help="Don't print errors")
    parser.add_argument('--ack-feed-rate', action='store_true', dest='ack_feed_rate', \
                      default=False, help="Acknowledge script feed rate instead of leaving it to the machine.")
    args = parser.parse_args()
    conf = dict()
    conf['input_filename'] = args.gcode_file
    conf['output_filename'] = args.filename
    conf['show_warning'] = args.warning
    conf['show_error'] = args.errors
    conf['show_info'] = args.info
    conf['show_fix'] = args.fix
    conf['ack_feed_rate'] = args.ack_feed_rate
    gcode.compile(conf)
