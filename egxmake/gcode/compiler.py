import sys

#Globals
class g:
  scr = None
  look = None
  in_f = None
  out_f = None
  bctr = 0

class config:
  show_warning = True
  show_info = True
  show_fix = True
  show_error = True
  input_filename = 'in.ngc'
  output_filename = 'out.egx'

#Boilerplate and I/O
def init():
    g.scr.reverse()
    #EGX file preample, from header.egx
    preamble = bytes((\
	0x03, 0x3B, 0x49, 0x4E, 0x3B, 0x43, 0x53, 0x36, 0x3B, 0x43, 0x41, 0x38, \
	0x3B, 0x49, 0x50, 0x30, 0x2C, 0x30, 0x2C, 0x33, 0x30, 0x35, 0x30, 0x2C, \
	0x32, 0x30, 0x35, 0x30, 0x3B, 0x4C, 0x54, 0x3B, 0x50, 0x41, 0x3B, 0x0D, \
	0x0A, 0x0D, 0x0A, 0x50, 0x55, 0x3B, 0x0D, 0x0A, 0x0D, 0x0A))
    g.out_f.write(preamble)
    getToken()

def cleanup():
  g.out_f.close()

def getChar():
    g.bctr += 1
    return chr(g.scr.pop())

def expected(msg):
  if config.show_error:
    sys.stderr.write("[ERR] @%d Found <%s>. Expected: %s\n" % (g.bctr, g.look, msg))
  cleanup()
  quit(-1)

def abort(msg):
  if config.show_error:
    sys.stderr.write("[ERR] Error at %d: %s\n" % (g.bctr, msg))
  cleanup()
  quit(-2)

def warn(msg):
  if config.show_warning:
    sys.stderr.write("[WRN] Warning at %d: %s\n" % (g.bctr, msg))

def fix(code):
  if config.show_fix:
    sys.stderr.write("[FIX] %s\n" % code)

def info(msg):
  if config.show_info:
    sys.stderr.write("[NFO] %s\n" % msg)

def match_cmd(cmd, num=None):
    if g.look[0] == cmd:
        if num == None:
            return True
        elif int(g.look[1:]) == num:
          return True
    return False

def getToken():
    '''Read a token, ignoring comments'''
    whitespace = ['\r','\n',' ']
    tmp_token = ''
    g.look = ''
    c = '!'
    while (len(g.scr) > 0) and (not (c in whitespace)):
        try:
            c = getChar()
            tmp_token += c
        except IndexError:
            break
    #If it is a commment
    if tmp_token[0] == '(':
        #Read until you find a comment
        try:
            while(getChar() != ')'):
                pass
            #And get a new token
            getToken()
        except IndexError:
          pass
          #expected("End of comment")
    #If it is a whitespace-only
    elif tmp_token in whitespace:
        getToken()
    else:
        g.look = tmp_token[:-1]
        #print("TOKEN:", g.look)

def inch2egx(inch):
    return int(inch*2540)

#####################################
## Parser begins here
#####################################

def ignore_opcode(op):
    fix("Ignored opcode <%s>." % op)
    getToken()

def rapid_positioning():
    '''Move the tool while retracted or extend the tool'''
    if not match_cmd('G', 0):
        expected('G00')
    getToken()
    cx = 0
    cy = 0
    cz = 0
    if match_cmd('X'):
        cx = float(g.look[1:])
        getToken()
    if match_cmd('Y'):
        cy = float(g.look[1:])
        getToken()
    if match_cmd('Z'):
        cz = float(g.look[1:])
        getToken()
    if   cz > 0: #Retract tool
        g.out_f.write(bytes('PU;', 'ASCII'))
    elif cz < 0: #Extend tool
        g.out_f.write(bytes('PD;', 'ASCII'))
    else:
        g.out_f.write(bytes('PU%d,%d;' % (inch2egx(cx), inch2egx(cy)), 'ASCII'))

def tool_rpm():
  if not match_cmd('S'):
    expected("S[number]")
  info("Tool RPM must be set to %s RPM." % g.look[1:])
  getToken()

def max_deviation():
  if not match_cmd('G', 64):
    expected("G64")
  getToken()
  if not match_cmd('P'):
    expected("Parameter (P??).")
  ignore_opcode('G64 P?? - Set max deviation')

def dwell():
  if not match_cmd('G', 4):
    expected("G04")
  getToken()
  if not match_cmd('P'):
    expected("Parameter (P??)")
    info("Ignoring dwell time")
  getToken()
  #Dwell can optionally take a list of XYZ coordinates
  #We will try to get them here
  while match_cmd('X') or match_cmd('Y') or match_cmd('Z'):
    cx = 0
    cy = 0
    cz = 0
    if match_cmd('X'):
      cx = float(g.look[1:])
      getToken()
    if match_cmd('Y'):
      cy = float(g.look[1:])
      getToken()
    if match_cmd('Z'):
      cz = float(g.look[1:])
      getToken()
    if cz != 0:
      warn("Dwell command with nonzero Z movement (Z ignored)")
    g.out_f.write(bytes('PD%d,%d;' % (inch2egx(cx), inch2egx(cy)),'ASCII'))

def move_linear():
  if not match_cmd('G', 1):
    expected("G01")
  getToken()
  #G01 has a coordinate value
  cx = None
  cy = None
  cz = 0
  if match_cmd('X'):
    cx = float(g.look[1:])
    getToken()
  if match_cmd('Y'):
    cy = float(g.look[1:])
    getToken()
  if match_cmd('Z'):
    cz = float(g.look[1:])
    getToken()
  #Check if it is a tool-up/down
  if   (cy != None) and (cy < 0):
    #Plunge tool
    g.out_f.write(bytes('PD;', 'ASCII'))
  elif (cy != None) and (cy > 0):
    #Raise tool
    g.out_f.write(bytes('PU;', 'ASCII'))
    if (cx != None) or (cy != None):
      warn("G01 with positive Z and non-null X/Y, ignoring Z.")
  #If there are X/Y values, do a PDx,y;
  if (cx != None) or (cy != None):
    if cx == None:
      cx = 0
    if cy == None:
      cy = 0
    g.out_f.write('PD%d,%d;' % (inch2egx(cx), inch2egx(cy)))
  #It also may have a Feed parameter
  if match_cmd('F'):
    feed_rate = float(g.look[1:])
    info("Ignoring feed rate <%s>" % str(feed_rate))
    getToken()

def program_end():
  if not match_cmd('M', 2):
    expected("M2")
  g.out_f.write(bytes('PU0,0;PU;', 'ASCII'))
  getToken()
  info("Program finished")
    
def command():
    '''A command begins with either G or M, some of them require extra parameters'''
    if   g.look[0] == 'G':
      #G-Commands
      g_num = int(g.look[1:])
      if   g_num == 0:
        rapid_positioning()
      elif g_num == 1:
        move_linear()
      elif g_num == 4:
        dwell()
      elif g_num == 20:
        ignore_opcode('G20 - Units: Inches')
      elif g_num == 64:
        max_deviation()
      elif g_num == 90:
        ignore_opcode('G90 - Absolute coordinates')
      elif g_num == 94:
        ignore_opcode('G94 - Inches per minute')
      else:
        abort("Unrecognized G opcode <%s>" % g.look)
    elif g.look[0] == 'M':
      #M-Commands
      m_num = int(g.look[1:])
      if m_num == 2:
        program_end()
      elif m_num == 3:
        ignore_opcode('M3 - Spin CW')
      elif m_num == 9:
        ignore_opcode('M9 - Coolant off')
      else:
        abort("Unrecognized M opcode <%s>" % g.look)
    elif g.look[0] == 'S':
      #Tool speed (S[number])
      tool_rpm()
    else:
      #Error
      expected("Command (G/M/S)")

####
## Test main procedure
####

def test_compiler():
  g.scr = list(open(config.input_filename, 'rb').read())
  g.out_f = open(config.output_filename, 'wb')
  #g.out_f = sys.stdout
  init()
  while g.look != '':
    command()
  cleanup()

if __name__ == '__main__':
  test_compiler()
