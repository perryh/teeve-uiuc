require 'socket'
require 'csv'

output_string = "x" * 100000

input_name = ARGV[0]
input_file = File.open(input_name, "r")
server = TCPServer.open(2000)
input_csv = CSV.parse(input_file, {:headers => true})

loop {
  Thread.start(server.accept) do |client|
    start_time = Time.now
    input_csv.each do |row|
      frame_size = row['FrameSize'].to_i
      while(((Time.now - start_time) * 1000) < row['Timestamp'].to_i)
      end
      client.print(output_string[0, frame_size]) 
      client.flush
    end
    client.close
    end
}