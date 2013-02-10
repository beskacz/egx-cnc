#!/usr/bin/env python3

import pygame._view
import pygame, sys, os
from pygame.locals import *
import easygui
import argparse

XRES      = 800
YRES      = 600
SRATIO    = XRES/YRES
FLIP_RATE = 100

def input(events):
   '''Process SDL events'''
   for event in events: 
      if event.type == QUIT: 
         sys.exit(0) 
      else:
          #print event
          pass

def parse_cnc(in_txt):
    charset  = '-.,;ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
    tokens = None
    txt = ''
    for c in in_txt:
        if c in charset:
            txt = txt + c
    return (txt.split(';'))

def pixel_tick(mill_max):
    mill_ratio = mill_max[0]/mill_max[1] #Ratio >1 if landscape, <1 if portrait
    tick = None
    if mill_ratio >= SRATIO: #width (x) is limiting
        tick = (XRES-20)/mill_max[0]
    else:               #height (y) is limiting
        tick = (YRES-20)/mill_max[1]
    return tick


def draw_line(surface, color, start_pos, end_pos, mill_max, aa=True):
    mill_ratio = mill_max[0]/mill_max[1] #Ratio >1 if landscape, <1 if portrait
    tick = pixel_tick(mill_max)
    orig_ = (int(start_pos[0] * (tick)) +10, YRES - int((start_pos[1] * (tick))) -10)
    dest_ = (int(end_pos[0] * (tick)) +10, YRES - int((end_pos[1] * (tick))) -10)
    if aa:
      pygame.draw.aaline(surface, color, orig_, dest_)
    else:
      pygame.draw.line(surface, color, orig_, dest_)

def draw_drill(surface, color, position, mill_max):
  draw_line(surface, color, (position[0] - 50, position[1] - 50), (position[0] + 50, position[1] + 50), mill_max, aa=False)
  draw_line(surface, color, (position[0] - 50, position[1] + 50), (position[0] + 50, position[1] - 50), mill_max, aa=False)

def adjust_magnitude(n):
  return n*0.1

def draw_cnc(tokens, surface, verbose = False, max_xy=None, line=True):
    line_count = 0
    min_x = 0
    min_y = 0
    max_x = None
    max_y = None
    lines = 0
    color_up = (0,0,255)
    color_dn = (255,255,0)
    color_dr = (255,0,0)
    verborhea = False
    #Search for IP token
    if max_xy == None:
      for t in tokens:
          if t[:2] == 'IP':
              v = t[2:].split(',')
              min_x = int(v[0])
              min_y = int(v[1])
              max_x = int(v[2])
              max_y = int(v[3])
              break
    else:
      min_x = 0
      min_y = 0
      max_x = max_xy[0]
      max_y = max_xy[1]
    if verbose:
        print("CNC space from (%d,%d) to (%d, %d)" % (min_x, min_y, max_x, max_y))
    #search for PD|PU tokens
    last_x = 0
    last_y = 0
    target_x = 0
    target_y = 0
    draw_count = 0
    for t in tokens:
        if   t[:2] == 'PD':
            v = t[2:].split(',')
            if len(t) == 2: #parameter-less
                pass #print('[MILL DOWN]')
            else:
                target_x = int(v[0])
                target_y = int(v[1])
                if verborhea:
                    print ('goto (%s,%s) [DOWN]' % (v[0], v[1]))
                lines+=1
                if line:
                  draw_line(surface, color_dn, (last_x, last_y), (target_x, target_y), (max_x, max_y))
                else:
                  draw_drill(surface, color_dr, (target_x, target_y), (max_x, max_y))
                draw_count += 1
        elif t[:2] == 'PU':
            v = t[2:].split(',')
            if len(t) == 2: #parameter-less
                pass #print('[MILL UP]')
            else:
                target_x = int(v[0])
                target_y = int(v[1])
                if verborhea:
                    print ('goto (%s,%s) [UP]' % (v[0], v[1]))
                lines+=1
                if line:
                  draw_line(surface, color_up, (last_x, last_y), (target_x, target_y), (max_x, max_y))
                else:
                  draw_drill(surface, color_dr, (target_x, target_y), (max_x, max_y))
                draw_count += 1
        last_x = target_x
        last_y = target_y
        line_count += 1
        if (line_count%100) == 0:
            input(pygame.event.get())
        if (draw_count % FLIP_RATE) == 0:
          pygame.display.flip()
    #Display the result
    if verbose:
        print("%d segments drawn" % lines)
    pygame.display.flip()
    
def max_xy(tokens):
  mx = -9999999
  my = -9999999
  for t in tokens:
    if t[:2] in ['PU', 'PD']:
      if len(t) > 2:
        xy = t[2:].split(',')
        xy = (int(xy[0]), int(xy[1]))
        if xy[0] > mx:
          mx = xy[0]
        if xy[1] > my:
          my = xy[1]
  return (mx, my)
        

if __name__ == '__main__':
    #Argument parser
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", default=None, metavar='EGX-FILE', dest='in_file')
    parser.add_argument("-d", default=None, metavar='DRILL-PREFIX', dest='drill', type=str)
    parser.add_argument('-v', default=False, action='store_true', dest='is_verbose')
    parser.add_argument('-r', default=100, metavar='REDRAW-RATE', action='store', dest='redraw_rate')
    args = parser.parse_args()
    FLIP_RATE = int(args.redraw_rate)
    #Load CNC file
    try:
        filename = args.in_file
    except Exception as e:
        pass
    if filename == None:
        filename = easygui.fileopenbox()
        if easygui.boolbox("Also load drill plots?") == 1:
          drill_file = easygui.fileopenbox()
          if drill_file != None:
            drill_file = drill_file[:-8]
            args.drill = drill_file

    tokens = None
    with open(filename, 'r') as f:
        tokens = parse_cnc(f.read())
    board_max = max_xy(tokens)
    print("Board maximum:", str(board_max))
    #Set-up pygame
    print ("Pygame initialized, Modules loaded: %d. Failed: %d" % pygame.init())
    window = pygame.display.set_mode((XRES,YRES))
    pygame.display.set_caption("CNC viewer")
    screen = pygame.display.get_surface()
    #Draw CNC
    draw_cnc(tokens, screen, args.is_verbose, board_max)
    #Try to load and draw drill plots
    if args.drill != None:
      tool_ctr = 1 
      try:
        while(True):
          drill_file = None
          if tool_ctr < 10:
            drill_file = args.drill + '-T0' + str(tool_ctr) + '.egx'
          else:
            drill_file = args.drill + '-T' + str(tool_ctr)  + '.egx'
          tokens = None
          with open(drill_file, 'r') as f:
            tokens = parse_cnc(f.read())
          board_max = max_xy(tokens)
          draw_cnc(tokens, screen, args.is_verbose, board_max, line=False)
          tool_ctr += 1
      except Exception as e:
        print("Failed to load drill-plot for tool #%d (no more tools?)" % tool_ctr)
    #Wait for user to exit
    while True: 
        input(pygame.event.get())
        pygame.time.delay(500)

