import os
import json

import telegram


class telegram_bot:
    """
    Manageable Telegram bot object to use for sending messages.

    Parameters
    ----------
    bot: telegram.Bot
        Bot provided by Telegram

    creds: Json
        Bot token and user credentials
    """

    bot: telegram.Bot = None

    creds = None

    def __init__(self):
        with open(os.path.expanduser("~/.credentials/telegram")) as f:
            self.creds = json.load(f)

        self.bot = telegram.Bot(self.creds["bot_key"])

    def sendMessage(self, user_id=None, text: str = None):
        if user_id is None:
            user_id = self.creds["user_id"]

        self.updater.bot.sendMessage(user_id, text)

    def checkForUpdates(self):
        self.bot.getUpdates()


if __name__ == "__main__":
    t = telegram_bot()
    t.sendMessage(text="Hello!")
