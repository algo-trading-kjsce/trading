import os
import json
import csv
import tempfile

import robin_stocks as rs

from datetime import datetime


def format_candle(entry) -> str:
    s = str(datetime.strptime(entry["begins_at"][0:10], "%Y-%m-%d").date()) + " " + \
        str(datetime.strptime(entry["begins_at"][11:19], "%H:%M:%S").time()) + "," + \
        str(entry["open_price"]) + "," + \
        str(entry["high_price"]) + "," + \
        str(entry["low_price"]) + "," + \
        str(entry["close_price"]) + "," + \
        str(entry["volume"]) + ",\n"

    return s


class rh_login:
    """
    Login Manager for Robinhood

    Attributes
    ----------
    token : Union[Any, Response, None, dict]
        a token from Robinhood login API call to hold on to and
        refresh if login has expired

    update_credentials : bool
        Whether manager saves login credentials on exit

    """

    token = None

    creds = None

    update_credentials: bool = True

    creds_path: str = os.path.expanduser("~/.credentials/robinhood")

    def __init__(self):
        """
        Parameters
        ----------
        username : str
            Username for account
        password : str
            Password for logging in
        use_credential : bool
            Override for using credential file saved on disk
        update_cred : bool
            Whether to save the incoming credentials on the computer
        """

        if os.path.exists(self.creds_path):
            with open(self.creds_path) as f:
                self.creds = json.load(f)
        else:
            self.creds = {"username": None, "password": None}

        token = rs.login(
            username=self.creds["username"], password=self.creds["password"])

    def __del__(self):
        rs.logout()

    def get_historical_data(self,
                            ticker: str = None,
                            interval: str = "15second",
                            span: str = "hour",
                            bounds: str = "24_7"):

        res = rs.get_crypto_historicals(ticker, interval, span, bounds)

        columns = ['', "datetime", "open", "high",
                   "low", "close", "volume"]

        csv_path = os.path.join(tempfile.gettempdir(), f"{ticker}.csv")

        with open(csv_path, 'w') as f:
            writer = csv.DictWriter(f, fieldnames=columns)
            writer.writeheader()

            i = 0
            for entry in res[-20:]:
                s = str(i) + ',' + format_candle(entry)
                i = i + 1
                f.write(s)

        return csv_path

    def get_last_price(self,
                       ticker: str = None,
                       interval: str = "15second",
                       span: str = "hour",
                       bounds: str = "24_7") -> str:
        res = rs.get_crypto_historicals(ticker, interval, span, bounds)

        return '0,' + format_candle(res[-1])


if __name__ == "__main__":
    r = rh_login()

    res = r.get_historical_data("ETH")

    del r
