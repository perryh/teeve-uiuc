require 'socket'

host, port = ARGV

s = TCPSocket.open(host, port)

while line = s.gets
	puts line.size
end

s.close