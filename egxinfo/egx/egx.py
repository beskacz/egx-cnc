'''
Common operations over .egx files
'''

preamble = b'\x03;IN;CS6;CA8;IP0,0,30500,20500;LT;PA;\r\n\r\nPU;\r\n\r\n'

def tokenize(in_txt):
  charset  = '-.,;ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
  tokens = None
  txt = ''
  for c in in_txt:
      if c in charset:
          txt = txt + c
  return (txt.split(';'))

def min_max(tokens):
  max_x = None
  max_y = None
  min_x = None
  min_y = None
  #Read the first pu/pd token
  for t in tokens:
    if len(t) > 2:
      if (t[:2] == 'PU') or (t[:2] == 'PD'):
        cmd_args = t[2:].split(',')
        max_x = int(cmd_args[0])
        min_x = int(cmd_args[0])
        max_y = int(cmd_args[1])
        min_y = int(cmd_args[1])
  #Search for max & mins
  for t in tokens:
    if len(t)>2:
      if (t[:2] == 'PU') or (t[:2] == 'PD'):
        cmd_args = t[2:].split(',')
        cx = int(cmd_args[0])
        cy = int(cmd_args[1])
        max_x = max((cx, max_x))
        min_x = min((cx, min_x))
        max_y = max((cy, max_y))
        min_y = min((cy, min_y))
  #Return the values
  return ((min_x, min_y),(max_x, max_y))

def header_min_max(tokens):
  for t in tokens:
    if len(t) > 2:
      if t[:2] == 'IP':
        cmd_args = t[2:].split(',')
        max_x = int(cmd_args[2])
        min_x = int(cmd_args[0])
        max_y = int(cmd_args[3])
        min_y = int(cmd_args[1])
        return ((min_x, min_y),(max_x, max_y))

def count_cmd(tokens):
  return len(tokens)

def count_pu(tokens):
  pu_xy = 0
  pu = 0
  for t in tokens:
    if len(t) > 2:
      if t[:2] == 'PU':
        pu_xy += 1
    elif t == 'PU':
      pu += 1
  return (pu, pu_xy)

def count_pd(tokens):
  pd_xy = 0
  pd = 0
  for t in tokens:
    if len(t) > 2:
      if t[:2] == 'PD':
        pd_xy += 1
    elif t == 'PD':
      pd += 1
  return (pd, pd_xy)

def count_redundant(tokens):
  pdpu = list()
  rc = 0
  for i in range(len(tokens)):
    if len(tokens[i]) >= 2:
      if tokens[i][:2] in ('PD', 'PU'):
        pdpu.append(tokens[i])
  for i in range(len(pdpu) - 1):
    if pdpu[i] == pdpu[i+1]:
      rc += 1
    if (pdpu[i] == pdpu[i+1][:2]):
      rc += 1
  return rc

