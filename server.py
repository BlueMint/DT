#!/usr/bin/python
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from os import curdir, sep
import cgi
import sys
import time
import serial

PORT_NUMBER = 10020
ser = serial.Serial('COM4', 9600)
redBrightness = 0
greenBrightness = 0
blueBrightness = 0

#This class will handles any incoming request from
#the browser 
class myHandler(BaseHTTPRequestHandler):
    
    #Handler for the GET requests

    def do_GET(self):
        #remove / from start of string
        print self.path
        print self.path[1:]
        self.path = self.path[1:]
        self.mode = self.path[:4]
        self.data = self.path[3:]
        print self.mode
        self.sortRequestType()

    def sortRequestType(self):
        #if self.mode == "rRed" or "rGrn" or "rBlu":
            #self.do_POST()
        print "Current mode: " + self.mode
        if self.mode == "send":
            ser.write("/" + self.path)
        elif self.mode == "fire":
            ser.write("/" + self.path)
        elif self.mode == "wRed":
            print "yep"
            self.redBrightness = self.data
        elif self.mode == "wGrn":
            self.greenBrightness = self.data
        elif self.mode == "wBlu":
            self.blueBrightness = self.data
        elif self.mode == "conf":
            print ("/send" + self.redBrightness + self.greenBrightness + self.blueBrightness)
            ser.write("/send" + self.redBrightness + self.greenBrightness + self.blueBrightness)

    def do_POST(self):
        if self.mode == "rRed":
            f = redBrightness
        elif self.mode == "rGrn":
            f = greenBrightness
        elif self.mode == "rBlu":
            f = blueBrightness
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        self.wfile.write(f)
            
            
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


