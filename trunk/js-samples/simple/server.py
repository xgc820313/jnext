import socket
HOST = ''                 # Symbolic name meaning the local host
PORT = 64446              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
while 1:
	print "Waiting for incoming connection...";
	conn, addr = s.accept()
	print 'Incoming connection from ', addr
	fs = conn.makefile()
	for i in range(1,5):
		line = fs.readline()
		if not line: break
		print line.rstrip()
		conn.send("Hi There " + str(i) + "\n" )
	fs.close()
	conn.close()
s.close()