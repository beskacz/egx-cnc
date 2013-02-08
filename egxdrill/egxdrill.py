import gerber
import argparse
import sys

if __name__ == '__main__':
    #Parse options
    parser = argparse.ArgumentParser(sys.argv[0], description="Generate a drill-plot script for Roland EGX-30 engravers.")
    parser.add_argument('input_file', metavar='DRILL-PLOT', type=str, \
                        help='A Gerber drill layer')
    parser.add_argument("-o", '--out', dest='output_file', \
                      help='Write output to file', metavar="FILE", default='out.egx')
    parser.add_argument('--no-warnings', action="store_false", dest="warning", \
                      default=True, help="Don't print warnings")
    parser.add_argument('--no-info', action="store_false", dest="info", \
                      default=True, help="Don't print info")
    parser.add_argument('--no-fix', action="store_false", dest="fix", \
                      default=True, help="Don't output 'needs fixing' messages")
    parser.add_argument('--no-error', action="store_false", dest="error", \
                      default=True, help="Don't print errors")
    parser.add_argument('-f', '--input-format', dest='format', \
                      help="Input file format", metavar='FORMAT', default="GERBER", type=str)
    args = parser.parse_args()

    cnf = dict()
    cnf['input_file'] = args.input_file
    cnf['output_prefix'] = args.output_file
    cnf['warning'] = args.warning
    cnf['info'] = args.info
    cnf['fix']  = args.fix
    cnf['error'] = args.error
    cnf['format'] = args.format
    if cnf['format'] == 'GERBER':
      pass
      gerber.make(cnf)
    else:
      print("[FIX] Only gerber format is supported at the moment.")
      print("[FIX] File a feature request on the bugtracker site.")
