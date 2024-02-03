import python_weather

import asyncio
import os

# get port from the command line
# port = sys.argv[1]
# io = socket.create_connection(('localhost', int(port)))


async def getweather():
  # declare the client. the measuring unit used defaults to the metric system (celcius, km/h, etc.)
  async with python_weather.Client(unit=python_weather.IMPERIAL) as client:
    # fetch a weather forecast from a city
    weather = await client.get('New York')
    
    # returns the current day's forecast temperature (int)
    print(weather.current.temperature)
    
    # get the weather forecast for a few days
    for forecast in weather.forecasts:
      print(forecast)
      
      # hourly forecasts
      for hourly in forecast.hourly:
        print(f' --> {hourly!r}')

if __name__ == '__main__':
  # see https://stackoverflow.com/questions/45600579/asyncio-event-loop-is-closed-when-getting-loop
  # for more details
  
  asyncio.run(getweather())