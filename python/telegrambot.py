import os
import json

import telegram.bot


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

    bot = None

    creds = None

    last_update_id = None

    def __init__(self):
        with open(os.path.expanduser("~/.credentials/telegram")) as f:
            self.creds = json.load(f)

        self.bot = telegram.Bot(self.creds["bot_key"])
        self.last_update_id = self.creds["last_update_id"]

        self.sendMessage(text="The bot has started...")

    def __del__(self):
        with open(os.path.expanduser("~/.credentials/telegram"), 'w') as f:
            json.dump(self.creds, f)

        self.sendMessage(text="Shutting down.")


    def sendMessage(self, text: str = None, user_id=None):
        if user_id is None:
            user_id = self.creds["user_id"]

        self.bot.sendMessage(user_id, text)


    def checkForUpdates(self) -> str:
        updates = self.bot.getUpdates(offset=self.last_update_id)

        s = []
        for update in updates:
            if update.update_id > self.last_update_id:
                s.append(update.message.text)
                self.last_update_id = update.update_id

        self.creds["last_update_id"] = self.last_update_id

        return s


if __name__ == "__main__":
    t = telegram_bot()
    c = t.checkForUpdates()

    t.sendMessage(text="Hello!")
