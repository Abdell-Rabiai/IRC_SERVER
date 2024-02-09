from datetime import date
import random
import os
import socket
import threading
import time
import select
import asyncio
import sys

weather_not_installed = False
try:
    import python_weather
except ImportError:
    weather_not_installed = True
    pass

class IRCBot:
    def __init__(self, port):
        self.port = port
        self.bot_busy_with_quiz = False
        self.send_cmds = False
        client_doing_quiz = ""
        self.io = socket.create_connection(('localhost', int(port)))

    def get_weather(self, recipient):
        async def _get_weather():
            if weather_not_installed == True:
                self.io.sendall(f'privmsg {recipient} :python_weather is not installed, please install it first\n'.encode('utf-8'))
                return
            async with python_weather.Client(unit=python_weather.IMPERIAL) as client:
                weather = await client.get('Casablanca')
                temp = round((weather.current.temperature - 32) * 5/9, 2)
                self.io.sendall(f'privmsg {recipient} :Today\'s temperature in Casablanca is: {temp} °C\n'.encode('utf-8'))
                for forecast in weather.forecasts:
                    temp = round((forecast.temperature - 32) * 5/9, 2)
                    self.io.sendall(f'privmsg {recipient} :{forecast.date}: {temp} °C\n'.encode('utf-8'))

        asyncio.run(_get_weather())

    def fill_quest_answers(self):
        quest_ans = {
            "who created this server ??": "tabi3a",
            "what is the capital of italy ??": "rome",
            "what is the capital of japan ??": "tokyo",
            "what is the capital of russia ??": "moscow",
            "what is the capital of spain ??": "madrid",
            "what is the capital of turkey ??": "ankara",
            "what is the capital of china ??": "beijing",
            "what is the capital of india ??": "new delhi",
            "what is the capital of brazil ??": "brasilia",
            "what is the capital of canada ??": "ottawa",
            "what is the capital of australia ??": "canberra",
            "what is the capital of argentina ??": "buenos aires",
            "what is the capital of south africa ??": "pretoria",
            "what is the capital of nigeria ??": "abuja",
            "what is the capital of egypt ??": "cairo",
            "what is the capital of japan ??": "tokyo",
            "what is the capital of russia ??": "moscow",
            "do you condemn hamas ??": "No",
            "Which planet has the most moons?": "saturn",   
            "What is the most popular social media platform?": "facebook",
            "What Netflix show had the most streaming views in 2021?": "squid game"
        }
        keys = list(quest_ans.keys())
        random.shuffle(keys)
        quest_shuffled = {key: quest_ans[key] for key in keys}
        return quest_shuffled

    def do_quiz(self, recipient):
        quest_ans = self.fill_quest_answers()
        score, i = 0, 0
        list_of_quests = list(quest_ans.keys())
        max_questions = len(list_of_quests)
        self.bot_busy_with_quiz = True

        while i < max_questions:
            ans = quest_ans[list_of_quests[i]]
            msg = f"privmsg {recipient} :{list_of_quests[i]}\n"
            self.io.sendall(msg.encode('utf-8'))

            ready = select.select([self.io], [], [], 10)  # Wait for 10 seconds

            if not ready[0]:
                # Timeout occurred
                msg = f"privmsg {recipient} :Time's up! You didn't answer in time.\n"
                self.io.sendall(msg.encode('utf-8'))
                i += 1
                continue

            answer = self.io.recv(4096).decode('utf-8')

            if self.main_commands_checker(answer) == 1:
                break

            if "quit quiz" in answer:
                msg = f"privmsg {recipient} :Quitting Quiz :)! Your score is: {score}/{(i + 1 % 100) * 5}\n"
                self.io.sendall(msg.encode('utf-8'))
                self.bot_busy_with_quiz = False
                break

            if ans in answer and "PRIVMSG" in answer and ":bot!bot" not in answer:
                msg = f"privmsg {recipient} :Correct\n"
                score += 5
            elif "PRIVMSG" in answer and ":bot!bot" not in answer:
                msg = f"privmsg {recipient} :Wrong\n"
            else:
                msg = ""

            i += 1
            self.io.sendall(msg.encode('utf-8'))
            time.sleep(1)

        msg = f"privmsg {recipient} :Quiz finished! Your score is: {score}/{(i + 1 % 100) * 5}\n"
        self.bot_busy_with_quiz = False
        self.io.sendall(msg.encode('utf-8'))

    def send_commands(self, recipient):
        commands = [
            "Bot is ready :)\n",
            "commands: \n",
            "!date: shows today's date\n",
            "!quiz: launches a quiz game\n",
            "!weather: shows the weather\n",
            "!quit: quits the bot\n",
            "!help: shows bot commands\n"
        ]

        for cmd in commands:
            self.io.sendall(f'privmsg {recipient} :{cmd}'.encode('utf-8'))
            time.sleep(1)

    def get_date(self, recipient):
        today = date.today()
        msg = f"privmsg {recipient} :Today's date: {today}\n"
        self.io.sendall(msg.encode('utf-8'))

    def check_is_a_bot_command(self, command):
        bot_commands = ["!date", "!quiz", "!weather", "!help", "!quit"]
        return any(command.startswith(cmd) for cmd in bot_commands)

    def main_commands_checker(self, message):
        nickname, recipient, command = "", "", ""
        if "PRIVMSG" in message:
            nickname = message.split('!')[0][1:]
            recipient = message.split(' ')[2]
            command = ''.join(i for i in message.split(' ')[3:])[1:]
        if "#" not in recipient:
            recipient = nickname

        if not self.send_cmds:
            self.send_commands(recipient)
            self.send_cmds = True

        if command.startswith("!quiz"):
            if self.bot_busy_with_quiz:
                self.io.sendall(f'privmsg {recipient} :Bot is busy with a quiz, please wait\n'.encode('utf-8'))
            else:
                self.client_doing_quiz = recipient
                self.do_quiz(recipient)
        elif command.startswith("!date"):
            self.get_date(recipient)
        elif command.startswith("!weather"):
            self.get_weather(recipient)
        elif command.startswith("!help"):
            self.io.sendall(f'privmsg {recipient} :Commands available: !quiz, !date, !weather, !quit\n'.encode('utf-8'))
        elif command.startswith("!quit"):
            self.io.sendall(f'privmsg {recipient} :quitting ...\n'.encode('utf-8'))
            return 1

        return 2

if __name__ == '__main__':
    irc_bot = IRCBot(sys.argv[1])
    irc_bot.io.sendall(b'pass 1337\nuser bot\nnick bot\n')

    while True:
        message = irc_bot.io.recv(4096).decode('utf-8')
        result = irc_bot.main_commands_checker(message)
        if result == 1:
            break
