#!/usr/bin/python
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from os import curdir, sep
import cgi
import sys
import time
from nrf24 import Nrf24

PORT_NUMBER = 10020

#This class will handles any incoming request from
#the browser 
class myHandler(BaseHTTPRequestHandler):
	
	#Handler for the GET requests
	def do_GET(self):
		#remove / from start of string
		self.path = self.path[1:]
		self.elapsed = (time.clock() - start)
		print self.elapsed
		self.sortRequestType()

	#Handler for the POST requests
	def do_POST(self):
		if self.path=="/send":
			form = cgi.FieldStorage(
				fp=self.rfile, 
				headers=self.headers,
				environ={'REQUEST_METHOD':'POST',
		                 'CONTENT_TYPE':self.headers['Content-Type'],
			})

			print "Your name is: %s" % form["your_name"].value
			self.send_response(200)
			self.end_headers()
			self.wfile.write("Thanks %s !" % form["your_name"].value)
			return

	def sortRequestType(self):
		#send
		#mode
			#fire
			#door
			#dead
			#mood
			#prty
		#stat
		#dead
		self.mode = self.path[:4]
		if self.mode == "send":
			print "Red: " + self.path[4:7]
			print "Green: " + self.path[7:10]
			print "Blue: " + self.path[10:13]
		elif self.mode == "stat":
			if not nrf.isSending():
    			nrf.send(map(ord,"Helloooo"))
		elif self.mode == "mode":
			print "sure"

		elif self.mode == "quit":
			server.socket.close()
			sys.exit()			
			

nrf = Nrf24(cePin=2,csnPin=3,channel=10,payload=8)
nrf.config()
nrf.setRADDR("host1")
nrf.setTADDR("host2")
			
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


