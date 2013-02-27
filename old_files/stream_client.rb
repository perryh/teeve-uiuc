require 'socket'

host, port = ARGV

s = TCPSocket.open(host, port)
start_time = Time.now

while line = s.gets
	#puts line.size
	puts line.size.to_s + " bytes received at " + ((Time.now - start_time) * 1000).to_s
end

s.close