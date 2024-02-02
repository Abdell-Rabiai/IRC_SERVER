import socket
import sys
from datetime import date
import random

# get port from the command line
port = sys.argv[1]
io = socket.create_connection(('localhost', int(port)))

# log into the server
io.sendall(b'pass 1337\nuser bot\nnick bot\n')

def fill_quest_answers():
    quest_ans = dict()

    quest_ans["who created this server ??"] = "tabi3a"
    quest_ans["what is the capital of italy ??"] = "rome"
    quest_ans["what is the capital of japan ??"] = "tokyo"
    quest_ans["what is the capital of russia ??"] = "moscow"
    quest_ans["what is the capital of spain ??"] = "madrid"
    quest_ans["what is the capital of turkey ??"] = "ankara"
    quest_ans["what is the capital of china ??"] = "beijing"
    quest_ans["what is the capital of india ??"] = "new delhi"
    quest_ans["what is the capital of brazil ??"] = "brasilia"
    quest_ans["what is the capital of canada ??"] = "ottawa"
    quest_ans["what is the capital of australia ??"] = "canberra"
    quest_ans["what is the capital of argentina ??"] = "buenos aires"
    quest_ans["what is the capital of south africa ??"] = "pretoria"
    quest_ans["what is the capital of nigeria ??"] = "abuja"
    quest_ans["what is the capital of egypt ??"] = "cairo"
    quest_ans["what is the capital of japan ??"] = "tokyo"
    quest_ans["what is the capital of russia ??"] = "moscow"
    quest_ans["do you condemn hamas ??"] = "No"
    quest_ans["Which planet has the most moons?"] = "saturn"
    quest_ans["What is the most popular social media platform?"] = "facebook"
    quest_ans["What Netflix show had the most streaming views in 2021?"] = "squid game"

    keys = list(quest_ans.keys())      # Python 3; use keys = d.keys() in Python 2
    random.shuffle(keys)
    quest_shuffled = dict()
    for key in keys:
        quest_shuffled[key] = quest_ans[key]
    return quest_shuffled

def do_quiz(io, recipient):
    quest_ans = fill_quest_answers()
    score = 0
    i = 0
    list_of_quests = list(quest_ans.keys())
    max = len(list_of_quests)
    send_new_quest = 1
    while (True):

        ans = quest_ans[list_of_quests[i]]
        msg = "privmsg " + recipient + " :" + list_of_quests[i] + "\n"

        if (send_new_quest == 1):
            io.sendall(msg.encode('utf-8'))

        answer = io.recv(4096).decode('utf-8')
        print(answer)
        if ("quit quiz" in answer):
            msg = "privmsg " + recipient + " " + " :Quitting Quiz :)! Your score is: " + str(score) + "\n"
            io.sendall(msg.encode('utf-8'))
            break
        if (ans in answer and "PRIVMSG" in answer and ":bot!bot" not in answer):
            msg = "privmsg " + recipient + " " + " :Correct\n"
            score += 5
            i += 1
            send_new_quest = 1
            io.sendall(msg.encode('utf-8'))
        elif ("PRIVMSG" in answer and ":bot!bot" not in answer):
            msg = "privmsg " + recipient + " " + " :Wrong\n"
            i += 1
            send_new_quest = 1
            io.sendall(msg.encode('utf-8'))
        else:
            send_new_quest = 0

        if (i == max):
            msg = "privmsg " + recipient + " " + " :Quiz finished:)! Your score is: " + str(score) + "\n"
            io.sendall(msg.encode('utf-8'))
            break


def get_date(io, recipient):
    today = date.today()
    msg = "privmsg " + recipient + " :Today's date: " + str(date.today()) + "\n"

    io.sendall(msg.encode('utf-8'))

nickname = ""
recipient = ""
while (True):

    print(nickname)
    command = io.recv(4096).decode('utf-8')
    if ("PRIVMSG" in command):
        nickname = command.split('!')[0][1:]
        recipient = command.split(' ')[2]
    if ("#" not in recipient):
        recipient = nickname
    if ("!quiz" in command):
        do_quiz(io, recipient)

    elif ("!date" in command):
        get_date(io, recipient)
    if ("quit bot" in command):
        io.sendall(b'quiting ...\n')
        break

# close the connection
# io.close()
