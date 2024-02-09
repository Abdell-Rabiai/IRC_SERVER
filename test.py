import socket
import sys
import time

def connect_and_send(port, num_users):
    # Create a list to store the socket connections
    io_list = []

    # Create connections for all users
    j = 0
    for i in range(num_users):
        io = socket.create_connection(('localhost', int(port)))

        # Formulate the nick and send the command
        nick_command = 'nick bot{}\n'.format(i)
        command = 'pass 1337\nuser bot\n{}'.format(nick_command)
        io.sendall(command.encode('utf-8'))

        # Append the socket connection to the list
        io.sendall(f"privmsg lim :HELLO HELLO HELLO HELLO HELLO WORLD\n".encode('utf-8'))
        io.sendall(f"join #chaa\n".encode('utf-8'))
        io.sendall(f"privmsg #chaa :HIIIIIIIIIIIIIIIIII CHANNEL\n".encode('utf-8'))
        io_list.append(io)
        j += 1
        print(j)
        # Simulate a delay to avoid flooding the server too quickly
        time.sleep(0.1)

    # Keep the connections open (optional)
    # and send a message from each user to a user named lim in the while true loop
    # like thiS: io.sendall(f"privmsg {recipient} :HELLO HELLO HELLO HELLO HELLO WORLD\n".encode('utf-8')) {recipient} is "lim"

    # try:
    #     while True:
    #         for io in io_list:
    #             io.sendall(f"privmsg lim :HELLO HELLO HELLO HELLO HELLO WORLD\n".encode('utf-8'))
    #             io.sendall(f"join #chaa\n".encode('utf-8'))
    #             io.sendall(f"privmsg #chaa :HIIIIIIIIIIIIIIIIII CHANNEL\n".encode('utf-8'))
    #             time.sleep(0.1)
    # except KeyboardInterrupt:
        # Close all socket connections on KeyboardInterrupt
        for io in io_list:
            io.close()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <port>")
        sys.exit(1)

    port = sys.argv[1]
    num_users = 1024

    connect_and_send(port, num_users)
