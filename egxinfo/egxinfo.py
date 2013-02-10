#! /usr/bin/env python3
'''
egxinfo shows some notable information about egx milling scripts.

 * Header board area
 * Real board area
 * Min. x/y values
 * Max. x/y values
 * Number of mech segments
 * Number of travel segments
 * Redundant points
'''
import egx
import argparse
import sys
import json

if __name__ == '__main__':
  #Parse options
  parser = argparse.ArgumentParser(sys.argv[0], description="Show egx file info.")
  parser.add_argument('egx_file', metavar='EGX-FILE', type=str, \
                        help='An egx milling file.')
  parser.add_argument('-j', '--json', action='store_true', default=False, dest='json', \
                        help='Output as json.')
  args = parser.parse_args()
  cnf = dict()
  cnf['egx_file'] = args.egx_file
  cnf['json'] = args.json
  #Tokenize the file
  tokens = None
  with open (cnf['egx_file'], 'r') as f:
    tokens = egx.tokenize(f.read())
  # --- Extract the info ---
  info = dict()
  #Get max/min xy
  minmax = egx.min_max(tokens)
  info['max'] = minmax[1]
  info['min'] = minmax[0]
  if not args.json:
    print("Minimum X/Y values:", str(minmax[0]))
    print("Maximum X/Y values:", str(minmax[1]))
  #Get header minmax
  hminmax = egx.header_min_max(tokens)
  info['header_min'] = hminmax[0]
  info['header_max'] = hminmax[1]
  if not args.json:
    print("Header minimum X/Y values:", str(hminmax[0]))
    print("Header maximum X/Y values:", str(hminmax[1]))
  #Get PU's
  pu = egx.count_pu(tokens)
  info['pu']    = pu[0]
  info['pu-xy'] = pu[1]
  if not args.json:
    print("Tool-up commands:", pu[0])
    print("Tool-up & move commands:", pu[1])
  #get PD's
  pd = egx.count_pd(tokens)
  info['pd']    = pd[0]
  info['pd-xy'] = pd[1]
  if not args.json:
    print("Tool-down commands:", pd[0])
    print("Tool-down & move commands:", pd[1])
  #get redundancies
  rc = egx.count_redundant(tokens)
  info['redundant'] = rc
  if not args.json:
    print("Detected redundancies:", rc)
  #output json (if needed)
  if args.json:
    print(json.dumps(info, sort_keys=True, indent=4))
