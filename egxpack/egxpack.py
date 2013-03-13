#! /usr/bin/env python3

import argparse
import sys
import json
import zipfile
import os.path

class g:
  conf = None

def warn(msg):
  if g.conf['warning']:
    print("[WAR] %s" % msg)

def error(msg):
  print("[ERR] %s" % msg)

def info(msg):
  if g.conf['info']:
    print("[NFO] %s" % msg)

def fix(msg):
  if g.conf['fix']:
    print("[FIX] %s" % msg)

def pack(conf):
  merged_meta = dict()
  z = zipfile.ZipFile(conf['output_file'], 'w', zipfile.ZIP_DEFLATED)
  info("Output file %s." % conf['output_file'])
  try:
    for p in conf['input_files']:
      info("Processing %s." % p)
      meta = json.loads(str(open(p, 'rb').read(), 'utf-8'))
      for k in meta.keys():
        if k in merged_meta.keys():
          fix("Implement some type of resource renaming")
          warn("Ignoring duplicate filename <%s>" % k)
          continue
        merged_meta[k] = meta[k]
        file_dir = os.path.split(p)[0]
        info("  Copying referenced file <%s>" % k )
        z.write(os.path.join(file_dir, k), k )
    info("Writing <all.meta>")
    z.writestr('all.meta', bytes(json.dumps(merged_meta, indent=4), 'utf-8') )
  except Exception as e:
    error(str(e))
    z.close()
  #write the all.meta file
  z.close()

if __name__ == '__main__':
    #Parse options
    parser = argparse.ArgumentParser(sys.argv[0], description="Merges the given .meta files into one, optionally zipping the resulting .meta file and the referenced files.")
    parser.add_argument('input_files', metavar='META-FILE', type=str, help='.meta files to merge', nargs='+')
    parser.add_argument("-o", '--out', dest='output_file', \
                      help='Write output to file, default out.zip.', metavar="ZIPFILE", default='out.zip')
    parser.add_argument('-v', '--info', action="store_true", dest="info", \
                      default=False, help="Print additional info (be verbose).")
    parser.add_argument('--nowarning', action="store_false", dest="warning", \
                      default=True, help="Don't print warnings")
    parser.add_argument('--nofix', action="store_false", dest="fix", \
                      default=True, help="Don't output 'needs fixing' messages")
    parser.add_argument('--noerror', action="store_false", dest="error", \
                      default=True, help="Don't print errors")
    args = parser.parse_args()

    cnf = dict()
    cnf['input_files'] = args.input_files
    cnf['output_file'] = args.output_file
    cnf['warning'] = args.warning
    cnf['info'] = args.info
    cnf['fix']  = args.fix
    cnf['error'] = args.error
    g.conf = cnf
    pack(cnf)
