#!/usr/bin/env python

import pygame._view
import pygame, sys, os
from pygame.locals import *
import easygui
import argparse

XRES   = 640
YRES   = 480
SRATIO = XRES/YRES

def input(events):
   '''Process SDL events'''
   for event in events: 
      if event.type == QUIT: 
         sys.exit(0) 
      else:
          #print event
          pass

def parse_cnc(in_txt):
    charset  = '.,;ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
    tokens = None
    txt = ''
    for c in in_txt:
        if c in charset:
            txt = txt + c
    return (txt.split(';'))

def draw_line(surface, color, start_pos, end_pos, mill_max):
    mill_ratio = mill_max[0]/mill_max[1] #Ratio >1 if landscape, <1 if portrait
    tick = None
    #print(surface, color, start_pos, end_pos, mill_max)
    if mill_ratio >= SRATIO: #width (x) is limiting
        tick = XRES/mill_max[0]
    else:               #height (y) is limiting
        tick = YRES/mill_max[1]

    orig_ = (int(start_pos[0] * (tick)), YRES - int((start_pos[1] * (tick))))
    dest_ = (int(end_pos[0] * (tick)), YRES - int((end_pos[1] * (tick))))
    #print('Line from %s to %s' % (str(orig_), str(dest_)))
    pygame.draw.aaline(surface, color, orig_, dest_)
    pygame.display.flip()

def draw_cnc(tokens, surface, verbose = False):
    line_count = 0
    min_x = 0
    min_y = 0
    max_x = None
    max_y = None
    lines = 0
    color_up = (0,0,255)
    color_dn = (255,255,0)
    #Search for IP token
    for t in tokens:
        if t[:2] == 'IP':
            v = t[2:].split(',')
            min_x = int(v[0])
            min_y = int(v[1])
            max_x = int(v[2])
            max_y = int(v[3])
            break
    if verbose:
        print("CNC space from (%d,%d) to (%d, %d)" % (min_x, min_y, max_x, max_y))
    #search for PD|PU tokens
    last_x = 0
    last_y = 0
    target_x = 0
    target_y = 0
    for t in tokens:
        if   t[:2] == 'PD':
            v = t[2:].split(',')
            if len(t) == 2: #parameter-less
                pass #print('[MILL DOWN]')
            else:
                target_x = int(v[0])
                target_y = int(v[1])
                if verbose:
                    print ('goto (%s,%s) [DOWN]' % (v[0], v[1]))
                lines+=1
                draw_line(surface, color_dn, (last_x, last_y), (target_x, target_y), (max_x, max_y))
        elif t[:2] == 'PU':
            v = t[2:].split(',')
            if len(t) == 2: #parameter-less
                pass #print('[MILL UP]')
            else:
                target_x = int(v[0])
                target_y = int(v[1])
                if verbose:
                    print ('goto (%s,%s) [UP]' % (v[0], v[1]))
                lines+=1
                draw_line(surface, color_up, (last_x, last_y), (target_x, target_y), (max_x, max_y))
        last_x = target_x
        last_y = target_y
        line_count += 1
        if (line_count%100) == 0:
            input(pygame.event.get())
    #Display the result
    if verbose:
        print("%d segments drawn" % lines)
    pygame.display.flip()
    
if __name__ == '__main__':
    #Argument parser
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", default=None, metavar='EGX file', dest='in_file')
    parser.add_argument('-v', default=False, action='store_true', dest='is_verbose')
    args = parser.parse_args()
    #Load CNC file
    try:
        filename = args.in_file
    except Exception as e:
        pass
    if filename == None:
        filename = easygui.fileopenbox()
    tokens = None
    with open(filename, 'r') as f:
        tokens = parse_cnc(f.read())
    #Set-up pygame
    print ("Pygame initialized, Modules loaded: %d. Failed: %d" % pygame.init())
    window = pygame.display.set_mode((XRES,YRES))
    pygame.display.set_caption("CNC viewer")
    screen = pygame.display.get_surface()
    #Draw CNC
    draw_cnc(tokens, screen, args.is_verbose)
    while True: 
        input(pygame.event.get())
        pygame.time.delay(100)

