import pandas as pd
import numpy as np
import StrategyExecutor
import CalculatedData
from typing import Tuple

def get_entry_exit(day_data: pd.DataFrame, index: int) -> Tuple[float, float, float, float]:
    """Calculate entry and exit prices, stop losses and target price

    Parameters:
    ----------
    day_data : pandas.DataFrame
        Series from which data needs to be extracted.

    index : int
        start index of 3 black crow pattern

    Returns:
    -------
    Tuple[float, float, float, float]
        entry time, exit time, entry price, exit price
    """
    #stoploss is high of first candle
       
    entry_price = day_data.open.values[index+3]
    entry_time = day_data.datetime.values[index+3]
    exit_price = day_data.open.values[-2]
    exit_time = day_data.datetime.values[-2]

    stop_loss = day_data.high.values[index]

    var_sl = entry_price
    #variable Sl to modify original SL

    for i in range (index+3, len(day_data)):
        
        if day_data.high.values[i] >= stop_loss:
            exit_price = stop_loss
            exit_time = day_data.datetime.values[i]
            break

        elif day_data.low.values[i] < var_sl:
            var_sl = day_data.low.values[i]
            stop_loss = day_data.high.values[index] - (entry_price - day_data.low.values[i])

        else:
            pass


    return entry_time, exit_time, entry_price, exit_price


def mymethod(dates, csv_data) -> CalculatedData.CalculatedData:

    final_data = CalculatedData.CalculatedData()

    for i in range (len(csv_data.datetime.dt.date.unique())-1):

        current_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i]].reset_index(drop = True)

        for j in range(4,64): #data only from 9:35 till 2:30 pm

            first_candle_size = current_day.open.values[j] - current_day.close.values[j]
            second_candle_size = current_day.open.values[j+1] - current_day.close.values[j+1]
            third_candle_size = current_day.open.values[j+2] - current_day.close.values[j+2]

            if (first_candle_size > 0) & (first_candle_size < second_candle_size) & (second_candle_size < third_candle_size):
                
                ent, ext, enp, exp = get_entry_exit(current_day, j)

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
                pass

    return final_data





def main():
    executor = StrategyExecutor.Executor(fileNames=["banknifty5min.csv"], is_short=True, caller=__file__, func=mymethod)

    executor.run(plotResults=True, uploadResults=False)
    pass


if __name__ == '__main__':
    main()