#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");

def function():
    print("function : Wait 10 sec \n");
    wait(10);
    print("function : finished waiting \n");
    
print "create a python thread"
x = threading.Thread( target=function )

print "start the thread"
x.start();

while (x.is_alive()):
    print "Prints this while the thread is working"

x.join();
print "Prints this only when function is done"
