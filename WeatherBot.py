import socket
import urllib.request
import json

HOST = "localhost"
PORT = 6667
PASSWORD = "1234"
NICK = "WeatherBot"
CHANNEL = "#general"


def get_meteo(city):
    url = "http://wttr.in/" + city + "?format=j1"
    try:
        response = urllib.request.urlopen(url, timeout=5)
        data = json.loads(response.read().decode("utf-8"))
        current = data["current_condition"][0]
        temp = current["temp_C"]
        desc = current["weatherDesc"][0]["value"]
        feels = current["FeelsLikeC"]
        humidity = current["humidity"]
        return "{} : {}°C (feels like {}°C), {}, humidity {}%".format(
            city, temp, feels, desc, humidity
        )
    except Exception:
        return "Could not retrieve weather for: " + city


def send(sock, msg):
    sock.sendall((msg + "\r\n").encode("utf-8"))


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    send(sock, "PASS " + PASSWORD)
    send(sock, "NICK " + NICK)
    send(sock, "USER WeatherBot 0 * :Weather Bot")

    buffer = ""

    while True:
        buffer += sock.recv(2048).decode("utf-8", errors="ignore")
        while "\r\n" in buffer:
            line, buffer = buffer.split("\r\n", 1)
            print("<< " + line)

            if line.startswith("PING"):
                if ":" in line:
                    token = line.split(":", 1)[1]
                else:
                    token = line.split(" ", 1)[1]
                send(sock, "PONG :" + token)

            if "001" in line:
                send(sock, "JOIN " + CHANNEL)

            if "PRIVMSG" in line and "!weather" in line:
                parts = line.split("PRIVMSG", 1)[1].strip().split(" ", 1)
                target = parts[0]
                if len(parts) > 1:
                    message = parts[1].lstrip(":")
                else:
                    message = ""
                if message.startswith("!weather "):
                    city = message[9:].strip()
                    if city:
                        reply = get_meteo(city)
                        send(sock, "PRIVMSG " + target + " :" + reply)
                elif message.startswith("!weather"):
                    reply = "Usage: !weather <cityname>"
                    send(sock, "PRIVMSG " + target + " :" + reply)

if __name__ == "__main__":
    main()