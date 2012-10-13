#!/usr/bin/env python3
'''
    egxmake - compile stripped gcode files (from gcodestrip) into egx
              milling scripts

    egxmake -o script.egx
'''

UNIT_RATIO = 1000

def load_bare(f):
    tokens = []
    commands = ['G00', 'G01', 'G04', 'X', 'Y', 'Z']
    l = f.readline()
    while l != '':
      if len(l) <= 1:
        continue
      if l[-1] == '\n':  #Remove newlines
        l = l[:-1]
      if l in commands:  #if it is a command, add it to the list
        tokens.append(l)
      else:
        try:             #if not, try to cast it as a float
          v = float(l)
          tokens.append(v)
        except ValueError:
          pass
      l = f.readline()
    return tokens

def make_egx(tokens, outf):
  tokens.reverse()
  px = 0
  py = 0
  pz = 0
  #wait for the first G00 command
  try:
    while tokens.pop() != 'G00':
      pass
    print('Initial G00 token found, %d tokens left' % len(tokens))
    tokens.append('G00')
  except IndexError:
    return None

  #Start interpreting commands
  try:
    while(True):
      #milling starts at a G04 command and ends on a G00 command
      c = tokens.pop()
      if c == 'G04': #Go SLOW command -> PD
        pass
      if c == 'G00':
        x = px
        y = py
        z = pz
        t = tokens.pop()
        while t in ['X', 'Y', 'Z']:
          if t == 'X':
            x = float(tokens.pop()*UNIT_RATIO)
            t = tokens.pop()
          if t == 'Y':
            y = float(tokens.pop()*UNIT_RATIO)
            t = tokens.pop()
          if t == 'Z':
            z = float(tokens.pop()*UNIT_RATIO)
            t = tokens.pop()
          print(x,y)
          outf.write(bytes('PU%d,%d;' % (x,y), 'ASCII'))
          px = x
          py = y
          pz = z
        tokens.append(t)
        
      else:
        pass
        print('Warning, orphaned token (%s)' % str(c) )
  except IndexError: #End of tokens
    outf.write(b'PU;\r\n')
    return None

if __name__ == '__main__':
    FILENAME = '../sample/strip/front.bare'
    tokens = None
    with open(FILENAME, 'r') as f:
      tokens = load_bare(f)
    with open('out.egx', 'wb') as f:
      make_egx(tokens, f)

