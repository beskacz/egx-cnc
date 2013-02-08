import re

preamble = bytes((\
	0x03, 0x3B, 0x49, 0x4E, 0x3B, 0x43, 0x53, 0x36, 0x3B, 0x43, 0x41, 0x38, \
	0x3B, 0x49, 0x50, 0x30, 0x2C, 0x30, 0x2C, 0x33, 0x30, 0x35, 0x30, 0x2C, \
	0x32, 0x30, 0x35, 0x30, 0x3B, 0x4C, 0x54, 0x3B, 0x50, 0x41, 0x3B, 0x0D, \
	0x0A, 0x0D, 0x0A, 0x50, 0x55, 0x3B, 0x0D, 0x0A, 0x0D, 0x0A))

class config:
  input_file    = None
  output_prefix = None
  show_fix      = False
  show_info     = False
  

def ignore_opcode(o):
  fix("Ignoring command <%s>" % o)

def info(msg):
  if config.show_info:
    print("[NFO] %s" % msg)

def fix(msg):
  if config.show_fix:
    print("[FIX] %s" % msg)

def inch2egx(inch):
    return int(inch*0.2540)

def mm2egx(inch):
    return int(inch*100)

def drill(tool, coord, f):
  x = coord[0]
  y = coord[1]
  cmd = "PU%d,%d;PD;PU;" % (inch2egx(x), inch2egx(y))
  f.write(bytes(cmd, 'ASCII'))

def make_drill():
  unit  = 'INCH'
  tools = dict()
  files = dict()

  tool_cmd = re.compile("^T[0-9]+$")
  tool_new = re.compile("^T[0-9]+C[0-9]*\\.?[0-9]+$")

  current_tool = None

  line = config.input_file.readline()
  while line != '':
    if   line[0] == 'M':
      ignore_opcode(line[:-1])
    elif line[:-1] == 'INCH':
      unit = 'INCH'
    elif line[0] == '%':
      pass
    elif tool_new.match(line[:-1]):
      t = line[:-1].split("C")
      tools[t[0]] = "%s %s" % (t[1], unit)
      info("New drill: %s -> %s" % (t[0], tools[t[0]]))
      files[t[0]] = open('%s-%s.egx' % (config.output_prefix, t[0]), 'wb')
      files[t[0]].write(preamble)
    elif tool_cmd.match(line[:-1]):
      current_tool = line[:-1]
    elif line[0] == 'X':
      xy = line[1:-1].split('Y')
      xy = (float(xy[0]), float(xy[1]))
      drill(current_tool, xy, files[current_tool])
    line = config.input_file.readline()
  for e in files.keys():
    files[e].close()

def make(cnf):
  with open(cnf['input_file'], 'r', encoding='ASCII') as in_f:
    config.show_fix      = cnf['fix']
    config.show_info     = cnf['info']
    config.input_file    = in_f
    config.output_prefix = cnf['output_prefix']
    make_drill()

if __name__ == '__main__':
  with open('test_drill.txt', 'r', encoding='ASCII') as f_in:
    make_drill(f_in, None)
  
