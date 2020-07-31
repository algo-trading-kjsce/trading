import numpy as np
import pandas as pd
from datetime import timedelta
import matplotlib.pyplot as plt

from typing import Tuple
from typing import List

import os
import paishe_utils

pd.set_option("max_row", None)

import CalculatedData

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
    def __init__(self, fileNames: List, is_short: bool, caller: str, func):
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

        self.m_fileNames = []

        for file in fileNames:
            code, fileName = paishe_utils.find_or_download_file(file)

            if code == 0:
                raise FileNotFoundError()

            self.m_fileNames.append(fileName)

        self.m_isShort = is_short
        self.m_currentFileName = ''
        self.m_strategyName = os.path.splitext(os.path.basename(caller))[0]
        self.m_func = func
        self.m_csv_data = None
        self.m_plot_data = pd.DataFrame()
        self.m_unique_dates = None

    def open_file(self):
        """Opens file for csv data."""

        self.m_csv_data = pd.read_csv(self.m_currentFileName)
        self.m_csv_data.datetime = pd.to_datetime(self.m_csv_data.datetime)
        self.m_csv_data["20 DMA"] = self.m_csv_data.close.rolling(50).mean()

        self.m_unique_dates = pd.DataFrame({"Date": self.m_csv_data.datetime.dt.date.unique()})
        pass

    def run_func(self):
        """Runs the strategy logic and gets the data to plot as final result."""

        calculatedData = self.m_func(self.m_unique_dates, self.m_csv_data)

        self.m_plot_data = pd.DataFrame({"entry_time": calculatedData.m_EntryTimes,
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
            self.m_plot_data['profit'] = self.m_plot_data.entry_price < self.m_plot_data.exit_price

            self.m_plot_data['pc_change'] = 1.0 - (self.m_plot_data['entry_price'] / self.m_plot_data['exit_price'])

        else:
            self.m_plot_data['profit'] = self.m_plot_data.entry_price > self.m_plot_data.exit_price

            self.m_plot_data['pc_change'] = 1.0 - (self.m_plot_data['exit_price'] / self.m_plot_data['entry_price'])


        self.m_plot_data.entry_time = pd.to_datetime(self.m_plot_data.entry_time)
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

        net_pos = [100000]
        profit = [0]

        equity_curve = pd.DataFrame({"date": self.m_csv_data.datetime.dt.date.unique()})

        j = 0

        for i in range(len(equity_curve) - 1):
            if j < len(self.m_plot_data):
                if equity_curve.date.values[i + 1] == self.m_plot_data.entry_time.dt.date.values[j]:
                    net_pos.append(net_pos[i] * (1.0 + (4.0 * self.m_plot_data.pc_change[j])))
                    profit.append(4.0 * self.m_plot_data.pc_change[j] * net_pos[i])
                    j += 1
                else:
                    net_pos.append(net_pos[i])
                    profit.append(0)
            else:
                net_pos.append(net_pos[i])
                profit.append(0)

        equity_curve['Total_Value'] = net_pos
        equity_curve['Profit'] = profit

        plt.plot(equity_curve.date, net_pos)
        plt.plot(equity_curve.date, profit)
        plt.title('Plot Results')

        plt.draw()

        if plotResults:
            plt.show()

        if uploadResults:
            files = []

            fileName = os.path.join(paishe_utils.get_temp_dir(), os.path.splitext(os.path.basename(self.m_currentFileName))[0])

            imageFileName = f"{fileName}.png"

            plt.savefig(fname=imageFileName)
            
            files.append(imageFileName)

            excelFileName = f"{fileName}.xlsx"

            with pd.ExcelWriter(excelFileName) as writer:
                self.m_csv_data.to_excel(writer, sheet_name='csvData', index=False)
                self.m_plot_data.to_excel(writer, sheet_name='final_data', index=False)
                equity_curve.to_excel(writer, sheet_name='plot_data', index=False)

            files.append(excelFileName)

            paishe_utils.upload_results(self.m_strategyName, files)

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
            self.open_file()
            self.run_func()
            self.plot_results(plotResults, uploadResults)
