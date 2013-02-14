require 'socket'

output_string = "x" * 100000

input_file = ARGV[0]

server = TCPServer.open(2000)

loop {
  Thread.start(server.accept) do |client|
    100.times { |n|
      client.puts(output_string[0, n])
    }
    client.close
    end
}