from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from os import curdir, sep
import cgi
import sys
import time
import serial

PORT_NUMBER = 10020
ser = serial.Serial('COM4', 9600)

redBrightness = "069"
greenBrightness = "000"
blueBrightness = "000"
address = "*.*"

class myHandler(BaseHTTPRequestHandler, object):

    def do_GET(self):
        self.path = self.path[1:]
        self.mode = self.path[:4]
        self.data = self.path[4:]
        self.sortRequestType()

    def sortRequestType(self):
        global redBrightness, greenBrightness, blueBrightness, address
        print " "
        print " "
        print redBrightness
        if self.mode == "rRed" or "rGrn" or "rBlu":
            self.do_POST()
        print "Current mode: " + self.mode
        if self.mode == "send":
            ser.write("/" + self.path)
        elif self.mode == "fire":
            ser.write("/" + self.path)
        elif self.mode == "door":
            ser.write("/" + self.path)
        elif self.mode == "wRed":
            while len(self.data) < 3:
                self.data = "0" + self.data
            redBrightness = self.data
        elif self.mode == "wGrn":
            while len(self.data) < 3:
                self.data = "0" + self.data
            greenBrightness = self.data
        elif self.mode == "wBlu":
            while len(self.data) < 3:
                self.data = "0" + self.data
            blueBrightness = self.data
        elif self.mode == "conf":
            print ("/send" + address + redBrightness + greenBrightness + blueBrightness)
            ser.write("/send" + address + redBrightness + greenBrightness + blueBrightness)

    def do_POST(self):
        global redBrightness, greenBrightness, blueBrightness
        self.webPage = "Nothing"
        print "trying..."
        if self.mode == "rRed":
            print redBrightness
            self.webPage = redBrightness
        elif self.mode == "rGrn":
            self.webPage = greenBrightness
        elif self.mode == "rBlu":
            self.webPage = blueBrightness
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        self.wfile.write(self.webPage)
            
            
try:
    server = HTTPServer(('', PORT_NUMBER), myHandler)
    print 'Started httpserver on port ' , PORT_NUMBER
    server.serve_forever()

except KeyboardInterrupt:
    print '^C received, shutting down the web server'
    server.socket.close()


