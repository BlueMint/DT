#!/usr/bin/python
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from os import curdir, sep
import cgi
import sys
import time
import serial

PORT_NUMBER = 10020
ser = serial.Serial('COM4', 9600)

#This class will handles any incoming request from
#the browser 
class myHandler(BaseHTTPRequestHandler):
    
    #Handler for the GET requests
    def do_GET(self):
        #remove / from start of string
        #self.path = self.path[1:]
        self.mode = self.path[:4]
        print self.mode
        self.sortRequestType()

    def sortRequestType(self):
        ser.write(self.path)         
            
            
try:
    #Create a web server and define the handler to manage the
    #incoming request
    server = HTTPServer(('', PORT_NUMBER), myHandler)
    print 'Started httpserver on port ' , PORT_NUMBER
    
    #Wait forever for incoming htto requests
    server.serve_forever()

except KeyboardInterrupt:
    print '^C received, shutting down the web server'
    server.socket.close()


