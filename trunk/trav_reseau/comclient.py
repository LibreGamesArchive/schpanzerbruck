#comclient

HOST = 'localhost'
PORT = 50003

import socket, sys, threading

class reception(threading.Thread):
	'''reception'''
	def __init__(self,conn):
		threading.Thread.__init__(self)
		self.connexion = conn
	
	def run(self) :
		nom = self.getName()
		print nom
		while (1) :	
			msgserveur = self.connexion.recv(1024)
			if msgserveur=="" :
				break
			print "voici le message \n"+msgserveur+"\n"
		
class emission(threading.Thread):
	'''emission de mesg'''
	def __init__(self,conn) :
		threading.Thread.__init__(self)
		self.connexion = conn
			
	def run(self):
		while (1) :
			msgsend = raw_input()
			self.connexion.send(msgsend)
			
			
connexion = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
try:
	connexion.connect((HOST,PORT))
except socket.error:
	print("la connection a echoue")
	sys.exit()
print "connection etablie"
th = emission(connexion)
th2 = reception(connexion)
th.start()
th2.start()
	


	
			
		
					
	
	
		
			
