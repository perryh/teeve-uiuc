require 'socket'
start_time = Time.now

server = TCPServer.new 2000 # Server bind to port 2000
loop do
  client = server.accept    # Wait for a client to connect
  while input = client.recv(1000000)
    puts input.length.to_s + " at " + ((Time.now - start_time) * 1000).to_s
  end
  client.close
end