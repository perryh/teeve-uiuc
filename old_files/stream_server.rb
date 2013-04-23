#!/home/perryh/.rvm/bin/ruby

require 'socket'

#Process.setpriority(Process::PRIO_PROCESS, 0, -20)
server = TCPServer.new 2000 # Server bind to port 2000

loop do
  client = server.accept    # Wait for a client to connect
  start_time = Time.now
  while input = client.recv(1000000)
  	current_time = Time.now
    puts input.length.to_s + " at " + ((current_time - start_time) * 1000).to_s
  end
  client.close
end