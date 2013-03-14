#!/usr/bin/env python3
import io
import os
import os.path
import sys
import subprocess as sp
import json
import zipfile
import tempfile
import easygui

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

def nix_get_default_printer():
  local_conf  = os.path.join(os.environ['HOME'], '.egx/egxmill.conf')
  glob_conf = os.path.join('/etc/egx/', 'egxmill.conf' )
  local_est   = './egxmill.conf'
  files = [local_est, local_conf, glob_conf]
  retv = None

  #Try to load a config file
  for p in files:
    if os.path.isfile(p):
      try:
        with open(local_conf, 'rb') as f:
          config = json.loads(str(f.read(), 'utf-8'))
          retv = config['default_printer']
      except Exception as e:
        return None
  return retv
  
  

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
  print_lst   = nix_list_printers()
  print_names = list()
  for i in range(len(print_lst)):
    print_names.append( "[%d] %s  -- %s" % (i, print_lst[i][0], print_lst[i][1]) )
  if len(print_lst) == 0:
    easygui.msgbox("No printer-like devices found")
    return None
  choice = easygui.choicebox("Pick a device to send the milling job.", "Device selection", print_names)
  index = int(choice[1:choice.find(']')])
  return(print_lst[index][0])

def nix_select_printer_cli():
  print_lst = nix_list_printers()
  if len(print_lst) == 0:
    error("No printers found")
    return None
  idx = 0
  print('+------------------------+')
  print("| Printer list:          |")
  print('+------------------------+')
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
  
def nix_interactive():
  printer = nix_get_default_printer()
  if len(nix_list_printers()) == 0:
    easygui.msgbox("No printers available")
    return None
  if printer == None:
    printer = nix_select_printer()
  else:
    if easygui.ynbox("Use default printer? [%s]" % printer, "Default printer"):
      pass
    else:
      printer = nix_select_printer()
  if printer == None:
    easygui.msgbox("No printer selected.")
    return None

  #Open zipfile, egx or a metafile
  f_path = easygui.fileopenbox("Select a file", "Open", '*', ['*.zip', '*.meta', '*.egx'])
  if f_path == None:
    easygui.msgbox("No file selected")
    return None  
  #switch by extenxion
  ext = os.path.splitext(f_path)[1]
  if   ext == '.meta':
    try:
      raise Exception("Unpacked .meta files not supported yet")
    except:
      easygui.exceptionbox()
    pass #METAFILE
  elif ext == '.zip':
    pass #ZIPFILE
    #unzip to temporal space
    z = zipfile.ZipFile(f_path, 'r')
    with tempfile.TemporaryDirectory() as td:
      td_path = str(td)
      z.extract('all.meta', td_path)
      try:
        egx_files = json.loads( open(os.path.join(td_path, 'all.meta')).read(), 'utf-8' )
      except:
        easygui.exceptionbox("Unable to load all.meta from zipfile")
      for k in egx_files.keys():
        z.extract(k, td_path)
      file_infos = list()
      for k in egx_files.keys():
        file_infos.append("[%s] %s %s (%s %s)" % (k, egx_files[k]['type'], egx_files[k]['name'], egx_files[k]['tool'], egx_files[k]['unit']) )
      #Show a milling selection box
      while len(file_infos) > 0:
        rv = easygui.multchoicebox("Select jobs to send at once", "Select", file_infos)
        if (rv == None) or (len(rv) == 0):
          easygui.msgbox("Job aborted")
          return None
        for e in rv:
          f_name = e[1:e.find(']')]
          file_infos.remove(e)
          nix_mill(printer, os.path.join(td_path, f_name))
      easygui.msgbox("Nothing left to mill. Finished.")
      
  else:
    if ext != '.egx':
      if easygui.ynbox("Selected file has no known extension, mill as .egx file?"):
        pass
      else:
        return None
    nix_mill(printer, f_path)
  

def nix_mill(printer_name, file_path):
  if printer_name == None:
    error("No printer specified")
    return None
  p1  = sp.Popen(['lp', '-d', printer_name, file_path])
  rv = None
  info("Sending job to [%s] using lp." % printer_name)
  while(rv == None):
    try:
      if sys.version_info[1] < 3:
        warn("Python version <3.3, egxmill will not respond while sending the milling job")
        rv = p1.wait()
      else:
        rv = p1.wait(5)
    except:
      rv = None
      info("Waiting for lp to finish...")
  if rv == 0:
    info("Job sent successfuly")
  else:
    error("Lp reported an error")

if __name__ == '__main__':
  info( "Default printer: %s" % str(nix_get_default_printer()) )
  nix_interactive()
  #nix_mill(printer, '/home/neonman/hg/egx-cnc/sample/egxmake/outline.egx')
