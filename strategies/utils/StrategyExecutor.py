import numpy as np
import pandas as pd
from datetime import timedelta
import matplotlib.pyplot as plt

from typing import Tuple
from typing import List

import math
import os
from utils import paishe_utils

pd.set_option("max_row", None)

import utils.CalculatedData

def get_hlc(day_data: pd.DataFrame) -> Tuple[float, float, float]:
    """Get the high, low, close values for the specific day.

    Parameters:
    ----------
    day_data : pandas.DataFrame
        The data series from which the high, low and close values need to be extracted.

    Returns:
    -------
    Tuple[float, float, float]
        High value, Low value, Close value

    """
    high = day_data.high.max()
    low = day_data.low.min()
    close = day_data.close.values[-1]
    return high, low, close


class Executor:
    def __init__(self, fileNames: List, is_short: bool, caller: str, func, initial_amount: float = 100000.0, n_shares: int = 10):
        """Constructor for Executor object.

        Parameters:
        ----------
        fileName : str
            File name or full path of the csv file.

        func : lambda
            lambda function that takes unique dates and data loaded from the file parameter.

        Raises:
        ------
        FileNotFoundError
            Thrown in case file does not exist on the computer or the google drive folder.
        """

        self.m_fileNames = fileNames

        """for file in fileNames:
            code, fileName = paishe_utils.find_or_download_file(file)

            if code == 0:
                raise FileNotFoundError()

            paishe_utils.custom_print(f"Found file: {file}")

            self.m_fileNames.append(fileName)"""

        self.m_isShort = is_short
        self.m_initialAmount = initial_amount
        self.m_number_of_shares = n_shares
        self.m_currentFileName = ''
        self.m_strategyName = os.path.splitext(os.path.basename(caller))[0]
        self.m_func = func
        self.m_csv_data = None
        self.m_calculated_data = pd.DataFrame()
        self.m_unique_dates = None

        paishe_utils.custom_print("")
        pass

    def open_file(self):
        """Opens file for csv data."""

        paishe_utils.custom_print("Opening...")

        """self.m_csv_data = pd.read_csv("ACC.csv", dtype={"datetime": pd.datetime, 
                                                        "open": float,
                                                        "high": float,
                                                        "low": float,
                                                        "close": float,
                                                        "volume": int})
        """
        self.m_csv_data = pd.read_csv("/Users/amarchheda/Desktop/pet_projects/trading/trading/data/ACC.csv")
        self.m_csv_data.datetime = self.m_csv_data.datetime.apply(lambda x: x.split("+")[0])
        self.m_csv_data.datetime = pd.to_datetime(self.m_csv_data.datetime)
        self.m_csv_data["20 DMA"] = self.m_csv_data.close.rolling(50).mean()

        self.m_unique_dates = pd.DataFrame({"Date": self.m_csv_data.datetime.dt.date.unique()})

        paishe_utils.custom_print("Opened.\n")
        pass

    def run_func(self):
        """Runs the strategy logic and gets the data to plot as final result."""

        paishe_utils.custom_print("Running Strategy...")

        calculatedData = self.m_func(self.m_unique_dates, self.m_csv_data)

        self.m_calculated_data = pd.DataFrame({"entry_time": calculatedData.m_EntryTimes,
                                         "entry_price": calculatedData.m_EntryPrices,
                                         "exit_time": calculatedData.m_ExitTimes,
                                         "exit_price": calculatedData.m_ExitPrices,
                                         "prevdayhigh": calculatedData.m_PreviousDayHighs,
                                         "prevdaylow": calculatedData.m_PreviousDayLows,
                                         "fc_open": calculatedData.m_CurrentDayOpen,
                                         "fc_high": calculatedData.m_CurrentDayHigh,
                                         "ec_low": calculatedData.m_CurrentDayLow,
                                         "ec_close": calculatedData.m_CurrentDayClose,
                                         "Condition1": calculatedData.m_Condition1,
                                         "Condition2": calculatedData.m_Condition2,
                                         "Condition3": calculatedData.m_Condition3,
                                         "Lowest_Close": calculatedData.m_LowestClose,
                                         "Stop_Loss": calculatedData.m_StopLosses})

        if self.m_isShort:
            self.m_calculated_data['profit'] = self.m_calculated_data.entry_price > self.m_calculated_data.exit_price
            self.m_calculated_data['change'] = self.m_calculated_data['entry_price'] - self.m_calculated_data['exit_price']

        else:
            self.m_calculated_data['profit'] = self.m_calculated_data.entry_price < self.m_calculated_data.exit_price
            self.m_calculated_data['change'] = self.m_calculated_data['exit_price'] - self.m_calculated_data['entry_price']


        self.m_calculated_data.entry_time = pd.to_datetime(self.m_calculated_data.entry_time)

        paishe_utils.custom_print("Finished Strategy.\n")

        pass


    def plot_results(self, plotResults: bool, uploadResults: bool):
        """Actual plotting function.
        
        Parameters:
        ----------
        plotResults
            Set to true if you want to plot the results

        uploadResults
            Set to True if you want to upload results to Google Drive
        """

        paishe_utils.custom_print("Starting write...")

        number_of_entries = len(self.m_calculated_data)

        dates = [self.m_csv_data.datetime.values[0]] * number_of_entries

        previous_total = [self.m_initialAmount] * number_of_entries
        current_total = [self.m_initialAmount] * number_of_entries

        profit_per_share = [0] * number_of_entries
        total_profit = [0] * number_of_entries

        number_of_shares = [self.m_number_of_shares] * number_of_entries

        for i in range(number_of_entries):
            calculatedDataRow = self.m_calculated_data.iloc[i]

            print(calculatedDataRow)
            print(calculatedDataRow.entry_time)
            print(type(calculatedDataRow.entry_time))
            print(self.m_csv_data.dtypes)
            available_shares = self.m_csv_data[self.m_csv_data.datetime == calculatedDataRow.entry_time].volume.tolist()[0]

            entry_price = float(calculatedDataRow.entry_price)

            number_of_shares_that_can_be_bought = math.floor(previous_total[i] / entry_price)

            actual_shares_bought = min(self.m_number_of_shares, min(number_of_shares_that_can_be_bought, available_shares))

            profit_per_share[i] = calculatedDataRow.change

            total_profit[i] = actual_shares_bought * calculatedDataRow.change

            current_total[i] = previous_total[i] + total_profit[i]

            if i != (number_of_entries - 1):
                previous_total[i + 1] = current_total[i]

            dates[i] = calculatedDataRow.entry_time

        equity_curve = pd.DataFrame({"date": dates})

        self.m_calculated_data['Previous Total'] = previous_total
        self.m_calculated_data['Profit per share'] = profit_per_share
        self.m_calculated_data['Number of shares'] = number_of_shares
        self.m_calculated_data['Total Profit'] = total_profit
        self.m_calculated_data['Current Total'] = current_total

        stockName = os.path.splitext(os.path.basename(self.m_currentFileName))[0]

        filePath = os.path.join(paishe_utils.get_temp_dir(), f"{stockName}_{self.m_strategyName}")

        plt.plot(equity_curve.date, current_total)
        plt.plot(equity_curve.date, total_profit)
        plt.title(f"{self.m_strategyName} strategy on {stockName}\nStart={previous_total[0]}, End={current_total[number_of_entries-1]}")

        plt.draw()

        imageFileName = f"{filePath}.png"
        plt.savefig(fname=imageFileName)

        excelFileName = f"{filePath}.xlsx"

        if paishe_utils.is_linux():
            paishe_utils.create_file(excelFileName)

        with pd.ExcelWriter(excelFileName) as writer:
            self.m_csv_data.to_excel(writer, sheet_name='csv_data', index=False)
            self.m_calculated_data.to_excel(writer, sheet_name='final_data', index=False)

        paishe_utils.custom_print("Finished writing data.\n")

        if uploadResults:
            paishe_utils.custom_print("Starting upload...")
            paishe_utils.upload_results(self.m_strategyName, [imageFileName, excelFileName])
            paishe_utils.custom_print("Finished upload.\n")

        if plotResults:
            plt.show()

        plt.close()
        pass


    def run(self, plotResults: bool, uploadResults: bool):
        """
        Opens the file specified in the constructor, runs the strategy logic and plots the result.

        Parameters:
        ----------
        plotResults
            Set to true if you want to plot the results

        uploadResults
            Set to True if you want to upload results to Google Drive

        """

        for file in self.m_fileNames:
            self.m_currentFileName = file

            paishe_utils.custom_print(f"\n\n----------Start File : {file}----------")

            self.open_file()
            self.run_func()
            self.plot_results(plotResults, uploadResults)

            paishe_utils.custom_print(f"----------End File : {file}----------")
