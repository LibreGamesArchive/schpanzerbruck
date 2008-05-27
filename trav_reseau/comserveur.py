#Comserveur

HOST = ''
PORT = 50003




import socket,sys,threading

class reception(threading.Thread):
	'''reception des messages clients'''
	def __init__(self,conn,liste) :
		threading.Thread.__init__(self)
		self.connexion = conn
		self.liste = liste
		
	        
	def run(self) :
		nom = self.getName()
		print nom
		while (True) :
			msgclient = self.connexion.recv(1024)
			if msgclient=="" :
				break
			print "voici le message\n"+msgclient+"\n"
		self.connexion.close()
		i=0
		del self.liste[self.getName()]
				
class emission(threading.Thread):
	'''emission des messages'''
	def __init__(self,conn,conn_client) :
		threading.Thread.__init__(self)
		self.connexion = conn
		self.liste = conn_client 
			
	def run(self):
		while (1) :
			print self.getName()
			msgsend = raw_input()
			for i in self.liste :
				self.liste[i].send(msgsend)
			
class connexion(threading.Thread):
	'''Creer une nouvelle connection'''
	def __init__(self,conn_client):
		threading.Thread.__init__(self)
		self.liste = conn_client
	def run(self):
		
			
		mysocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try : 
			mysocket.bind((HOST,PORT))	
		except socket.error :
			print "pb de connection"
			sys.exit()
		while True :	
			mysocket.listen(5)
			connection,adresse = mysocket.accept()
			print "coucou"				
			print "client connecte"
			th = reception(connection,self.liste)
			
			th.start()	
			
			it = th.getName()
			self.liste[it] = connection
			

			
			
conn_client ={}
connection =0
thread = connexion(conn_client)
th2 = emission(connection,conn_client)

th2.start()
thread.start()


							
				
