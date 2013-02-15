
def hsv2rgbf(h,s,v):
  c = v * s
  hp = h/60.0
  x = c*(1-abs((hp % 2) - 1))
  rgb1 = None
  if   hp == None:
    rgb1 = (0,0,0)
  elif (hp <= 0) and (hp < 1):
    rgb1 = (c,x,0)
  elif (hp <= 1) and (hp < 2):
    rgb1 = (x,c,0)
  elif (hp <= 2) and (hp < 3):
    rgb1 = (0,c,x)
  elif (hp <= 3) and (hp < 4):
    rgb1 = (0,x,c)
  elif (hp <= 4) and (hp < 5):
    rgb1 = (x,0,c)
  elif (hp <= 5) and (hp < 6):
    rgb1 = (c,0,x)
  m = v - c
  rgb = (rgb1[0] + m, rgb1[1] + m, rgb1[2] + m)
  return rgb

def hsv2rgb(h,s,v):
  rv = hsv2rgbf(h,s,v)
  return (int(rv[0] * 255), int(rv[1] * 255), int(rv[2] * 255))
