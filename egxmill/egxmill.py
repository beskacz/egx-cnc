#!/usr/bin/env python3
import io
import sys
import subprocess as sp

class g:
  config = {'info':True, 'warning':True, 'error':True}

def info(msg):
  if g.config['info']:
    print("[NFO] %s" % msg)

def warn(msg):
  if g.config['warning']:
    print("[WAR] %s" % msg)

def error(msg):
  if g.config['error']:
    print("[ERR] %s" % msg)

def nix_list_printers():
  #> lpstat -a -l
  p1  = sp.Popen(['lpstat', '-a', '-l'], stdout=sp.PIPE)
  output = str(p1.communicate()[0], sys.getdefaultencoding())
  iostr  = io.StringIO(output)

  ret_lst = list()
  for l in iostr.readlines():
    if len(l) == 0:
      continue
    space_idx = l.find(' ')
    ret_lst.append( [l[:6],l[6:-1]] )
  return ret_lst

def nix_select_printer():
  print_lst = nix_list_printers()
  if len(print_lst) == 0:
    error("No printers found")
    return None
  idx = 0
  for p in print_lst:
    print("[%d]" % idx, p[0], p[1])
  option = None
  while(option == None):
    sys.stdout.write("Select printer: ")
    sys.stdout.flush()
    try:
      option = print_lst[int(sys.stdin.readline()[:-1])][0]
    except IndexError as e:
      print("Index out of range.")
    except Exception as e:
      print("Option not valid <%s>." % str(e))
  return option
  
def nix_mill(printer_name, data):
  #Popen 'lp -d <printer-name>'
  #Push all data trough STDIN
  pass

if __name__ == '__main__':
  print("Testing messages")
  info("Hoot!")
  warn("Hey! Listen!")
  error("Kiaaa!")
  print(nix_select_printer())
  pass
