#!/usr/bin python

from subprocess import Popen, call
import sys

GCC = 'gcc'
cfil = 'measure_execution_window.c'
targ = '-ooutput'

run_targ = './output'



for i in range(200):
    nops = '-DNOPS=.rept %s' % (i+1)
    call([GCC, cfil, nops, targ])
    call([run_targ])