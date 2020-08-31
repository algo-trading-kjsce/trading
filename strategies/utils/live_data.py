#! /usr/bin/env python
### used for older python versions
from __future__ import print_function
from __future__ import absolute_import

###author defination
__author__ = "Amar Chheda: chheda.am@outlook.com"

###import statements go here
import requests
import pandas as pd
import json
import os

###logic implementation

class LiveData:
    """
    A class for getting the live data for a given ticker

    By default it gets the intra day 5 mins data

    All available functions are:
    
    1.) TIME_SERIES_INTRADAY: intraday data (need to specify the minutes: 1min, 5 min, 15min, 30min, 60min)
    2.) TIME_SERIES_DAILY: daily aggregated data 
    3.) TIME_SERIES_WEEKLY: weekly aggregated data
    4.) TIME_SERIES_MONTHLY: monthly aggregated data

    More information on this can be found at: https://www.alphavantage.co/documentation/

    methods
    -------

    """

    def __init__(self, ticker: str, function: str = "TIME_SERIES_INTRADAY", data_type: str = "json", period: str = "5min", output_size: str = "full") -> None:
        
        """initialization function"""

        self.ticker = ticker
        self.function = function
        self.data_type = data_type
        self.period = period
        self.output_size = output_size
        self.api = self.get_api_key()

    @staticmethod
    def get_api_key() -> str:
        """gets the api key from the json file"""
        cwd = os.getcwd()
        with open("trading/src/credentials/alpha_vantage_api.json") as f:
            data = json.load(f)
            f.close()
        
        return data['api_key']     

    @staticmethod
    def json_to_dataframe(json_data: dict) -> pd.DataFrame:
        """
        takes in a dictionary data and returns a data frame
        """

        data_key = list(json_data.keys())[1] #get the dict key
        dict_data = json_data[data_key] #get the data in key

        final_data =    {"date": [],
                        "open": [],
                        "high": [],
                        "low": [],
                        "close": [],
                        "volume": []}

        for date, ohlc in dict_data.items(): #looping over the dictionary
            final_data['date'].append(pd.to_datetime(date))
            final_data['open'].append(float(ohlc['1. open']))
            final_data['high'].append(float(ohlc['2. high']))
            final_data['low'].append(float(ohlc['3. low']))
            final_data['close'].append(float(ohlc['4. close']))
            final_data['volume'].append(int(ohlc['5. volume']))
                

        return pd.DataFrame(final_data)

    def get_ticker_data(self) -> pd.DataFrame:
        
        """
        get the ticker data for a given ticker and fucntion
        """

        base = "https://www.alphavantage.co/query?"

        query_url = base + "function={}".format(self.function) + \
                    "&symbol={}".format(self.ticker) + \
                    "&interval={}".format(self.period) + \
                    "&outputsize={}".format(self.output_size) + \
                    "&datatype={}".format(self.data_type) + \
                    "&apikey={}".format(self.api)

        json_data = requests.get(query_url).json()

        return self.json_to_dataframe(json_data)        




