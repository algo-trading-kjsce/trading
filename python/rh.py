import os
import typing
import json

import getpass

import robin_stocks as rs


class rh_login:
    """
    Login Manager for Robinhood

    Attributes
    ----------
    token : Union[Any, Response, None, dict]
        a token from Robinhood login API call to hold on to and refresh if login has expired

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
            Override for using credential file saved on disk instead of parameters
        update_cred : bool
            Whether to save the incoming credentials on the computer
        """

        if os.path.exists(self.creds_path):
            with open(self.creds_path) as f:
                self.creds = json.load(f)
        else:
            self.creds = {"username": None, "password": None}


    def __enter__(self):
        token = rs.login(username=self.creds["username"], password=self.creds["password"])

        return self


    def __exit__(self, exc_type, exc_value, exc_traceback):
        rs.logout()
