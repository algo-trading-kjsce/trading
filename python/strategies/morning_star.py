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

from utils import  StrategyExecutor
from utils import CalculatedData

from typing import Tuple


def get_entry_exit(day_data: pd.DataFrame, index: int) -> Tuple[int, float, float, float, float]:

    entry_price = day_data.open.values[index + 3]
    entry_time = day_data.datetime.values[index + 3]

    exit_price = day_data.open.values[-3]
    exit_time = day_data.datetime.values[-3]

    stop_loss = day_data.open.values[index + 1]

    for i in range(index + 4, len(day_data)):

        if (day_data.open.values[i] > stop_loss):

                stop_loss = day_data.close.values[i-1]

        else:
            if (stop_loss >= day_data.open.values[i]):
                exit_price = day_data.open.values[i]
                exit_time = day_data.datetime.values[i]
                break

    return i, entry_time, exit_time, entry_price, exit_price

def mymethod(dates, csv_data) -> CalculatedData.CalculatedData:

    final_data = CalculatedData.CalculatedData()

    for i in range(len(dates) - 1):

        current_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i]].reset_index(drop=True)

        j = 4

        while j < (len(current_day) - 5):

            open_price = current_day.open.values[j]
            close_price = current_day.close.values[j]
            open_price_second = current_day.open.values[j + 1]
            close_price_second = current_day.close.values[j + 1]
            open_price_third = current_day.open.values[j + 2]
            close_price_third = current_day.close.values[j + 2]

            if (open_price > close_price):

                if abs(open_price_second - close_price_second) <= 0.3 * abs(open_price - close_price):

                    condition1 = open_price_third < close_price_third
                    condition2 = abs(open_price_third - close_price_third) > abs(open_price_second - close_price_second) 

                    if condition1 and condition2:

                        j, ent, ext, enp, exp = get_entry_exit(current_day, j)

                        final_data.m_EntryTimes.append(ent)
                        final_data.m_EntryPrices.append(enp)
                        final_data.m_ExitTimes.append(ext)
                        final_data.m_ExitPrices.append(exp)

                        final_data.m_PreviousDayHighs.append(0)
                        final_data.m_PreviousDayLows.append(0)
                        final_data.m_StopLosses.append(0)
                        final_data.m_LowestClose.append(0)
                        final_data.m_CurrentDayOpen.append(0)
                        final_data.m_CurrentDayHigh.append(0)
                        final_data.m_CurrentDayLow.append(0)
                        final_data.m_CurrentDayClose.append(0)
                        final_data.m_Condition1.append(0)
                        final_data.m_Condition2.append(0)
                        final_data.m_Condition3.append(0)

                    else:
                        j+=1
                        pass


    return final_data





def main():
    executor = StrategyExecutor.Executor(fileNames=["./ACC.csv"], is_short=False, caller=__file__, func=mymethod)

    executor.run(plotResults=False, uploadResults=False)


if __name__ == '__main__':
    main()