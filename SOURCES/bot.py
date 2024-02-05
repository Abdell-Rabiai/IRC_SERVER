import socket
import sys
from datetime import date
import random
import os
# try:
#   import python_weather
# except:
#     os.system('pip install python-weather')
# import python_weather
import time
import select

import asyncio
import os

# get port from the command line
port = sys.argv[1]
io = socket.create_connection(('localhost', int(port)))


async def getweather(io, recipient):
  # declare the client. the measuring unit used defaults to the metric system (celcius, km/h, etc.)
  async with python_weather.Client(unit=python_weather.IMPERIAL) as client:
    # fetch a weather forecast from a city
    weather = await client.get('Casablanca')
    
    temp = round((weather.current.temperature - 32) * 5/9, 2)
    io.sendall(f'privmsg {recipient} :Today\'s temperature in Casablanca is: {temp} °C\n'.encode('utf-8'))
    
    # get the weather forecast for a few days
    for forecast in weather.forecasts:
      temp = round((forecast.temperature - 32) * 5/9, 2)
      io.sendall(f'privmsg {recipient} :{forecast.date}: {temp} °C\n'.encode('utf-8'))


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
    
    while i < max:
        ans = quest_ans[list_of_quests[i]]
        msg = "privmsg " + recipient + " :" + list_of_quests[i] + "\n"
        io.sendall(msg.encode('utf-8'))

        ready = select.select([io], [], [], 10)  # Wait for 10 seconds

        if not ready[0]:
            # Timeout occurred
            msg = "privmsg " + recipient + " " + " :Time's up! You didn't answer in time.\n"
            io.sendall(msg.encode('utf-8'))
            i += 1  # Move to the next question
            continue

        answer = io.recv(4096).decode('utf-8')

        if "quit quiz" in answer:
            msg = "privmsg " + recipient + " " + " :Quitting Quiz :)! Your score is: " + str(score) + "/" + str((i + 1 % 100) * 5) + "\n"
            io.sendall(msg.encode('utf-8'))
            break

        if ans in answer and "PRIVMSG" in answer and ":bot!bot" not in answer:
            msg = "privmsg " + recipient + " " + " :Correct\n"
            score += 5
        elif "PRIVMSG" in answer and ":bot!bot" not in answer:
            msg = "privmsg " + recipient + " " + " :Wrong\n"
        else:
            msg = ""  # Handle other cases as needed

        i += 1
        io.sendall(msg.encode('utf-8'))
        time.sleep(1)  # Add a short delay before moving to the next question

    msg = "privmsg " + recipient + " " + " :Quiz finished! Your score is: " + str(score) + "/" + str((i + 1 % 100)  * 5) + "\n"
    io.sendall(msg.encode('utf-8'))

def send_commands(io, recipient):
    io.sendall(f"privmsg {recipient} :Bot is ready :)\n".encode('utf-8'))
    time.sleep(1)
    io.sendall(f"privmsg {recipient} :commands: \n".encode('utf-8'))
    time.sleep(1)
    io.sendall(f"privmsg {recipient} :!date: shows todays date\n".encode('utf-8'))
    time.sleep(1)
    io.sendall(f"privmsg {recipient} :!quiz: launches a quiz game\n".encode('utf-8'))
    time.sleep(1)
    io.sendall(f"privmsg {recipient} :!weather: shows the weather\n".encode('utf-8'))
    time.sleep(1)
    io.sendall(f"privmsg {recipient} :!quit: quits the bot\n".encode('utf-8'))
    time.sleep(1)
    io.sendall(f"privmsg {recipient} :!help: shows cot commands\n".encode('utf-8'))

def get_date(io, recipient):
    today = date.today()
    msg = "privmsg " + recipient + " :Today's date: " + str(date.today()) + "\n"

    io.sendall(msg.encode('utf-8'))

nickname = ""
recipient = ""
command = ""
send_cmds = 0
while (True):

    # print(nickname)
    message = io.recv(4096).decode('utf-8')
    if ("PRIVMSG" in message):
        nickname = message.split('!')[0][1:]
        recipient = message.split(' ')[2]
        command = ''.join(i for i in message.split(' ')[3:])[1:]
    if ("#" not in recipient):
        recipient = nickname
    if (send_commands == 0):
        send_commands(io, recipient)
        send_cmds = 1

    if (command.startswith("!quiz")):
        do_quiz(io, recipient)

    elif (command.startswith("!date")):
        get_date(io, recipient)
    
    elif (command.startswith("!weather")):
        asyncio.run(getweather(io, recipient))
    elif (command.startswith("!help")):
        io.sendall(f'privmsg {recipient} :Commands available: !quiz, !date, !weather, !quit\n'.encode('utf-8'))
    if (command.startswith("!quit")):
        io.sendall(f'privmsg {recipient} :quitting ...\n'.encode('utf-8'))
        break

# close the connection
# io.close()

# pass 1337
# nick bot
# user bot
