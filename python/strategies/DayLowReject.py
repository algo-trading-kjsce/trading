import pandas as pd

from utils import StrategyExecutor
from utils import CalculatedData

from typing import Tuple

def get_entry_exit(day_data: pd.DataFrame, index: int, ent: int, pdh: float, pdl: float) -> Tuple[float, float, float, float, float, float, float]:
    """Calculate entry and exit prices, stop losses and target price

    Parameters:
    ----------
    df : pandas.DataFrame
        Series from which data needs to be extracted.

    index : int
        nth value from the series

    ent : int
        entry index

    pdh : float
        previous day high

    cpr : float
        average of high, low and close

    Returns:
    -------
    Tuple[float, float, float, float, float, float, float]
        entry time, exit time, entry price, exit price, stop loss, target, lowest close
    """
    #stoploss is 0.1% above prev day high   
    entry_price = day_data.low.values[ent]
    entry_time = day_data.datetime.values[index]
    exit_price = day_data.open.values[-1]
    exit_time = day_data.datetime.values[-1]

    stop_loss = .999 * pdl

    highest_close = entry_price

    i = 0

    for close in day_data.loc[index:]["close"]:
        if close <= stop_loss:
            exit_price = stop_loss
            exit_time = day_data.datetime.values[index + i]
            break

        elif close > highest_close:
            highest_close = close
            stop_loss = (.999 * pdl) + (close - entry_price)

        else:
            pass


        i+=1

    return entry_time, exit_time, entry_price, exit_price, stop_loss, highest_close


def mymethod(dates, csv_data) -> CalculatedData.CalculatedData:

    final_data = CalculatedData.CalculatedData()

    for i in range(len(csv_data.datetime.dt.date.unique()) - 1):

        previous_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i]].reset_index(drop=True)
        current_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i+1]].reset_index(drop=True)

        if previous_day.close[len(previous_day)-1] > previous_day['20 DMA'][len(previous_day)-1]:
            pdh, pdl, pdc = StrategyExecutor.get_hlc(previous_day)

            flag = True

            for i in range(3):

                if (current_day.low.values[i] < pdl) & flag:

                    for j in range(i, i + 4):

                        if (current_day.close.values[j] > pdl) & flag:

                            for k in range(j, len(current_day)):

                                if current_day.high.values[k] > current_day.high.values[j]:

                                    entry_time, exit_time, entry_price, exit_price, stop_loss, lowest_close = get_entry_exit(current_day, k, j, pdh, pdl)

                                    final_data.m_EntryTimes.append(entry_time)
                                    final_data.m_EntryPrices.append(entry_price)

                                    final_data.m_ExitTimes.append(exit_time)
                                    final_data.m_ExitPrices.append(exit_price)

                                    final_data.m_PreviousDayHighs.append(pdh)
                                    final_data.m_PreviousDayLows.append(pdl)

                                    final_data.m_StopLosses.append(stop_loss)

                                    final_data.m_LowestClose.append(lowest_close)

                                    final_data.m_CurrentDayOpen.append(current_day.open.values[i])
                                    final_data.m_CurrentDayHigh.append(current_day.high.values[i])
                                    final_data.m_CurrentDayLow.append(current_day.low.values[j])
                                    final_data.m_CurrentDayClose.append(current_day.close.values[j])

                                    final_data.m_Condition1.append(current_day.datetime.dt.time.values[i])
                                    final_data.m_Condition2.append(current_day.datetime.dt.time.values[j])
                                    final_data.m_Condition3.append(current_day.datetime.dt.time.values[k])

                                    flag = False
                                    break

                                else:
                                    pass
                        else:
                            pass

                else:
                    pass



    return final_data


def main():
    executor = StrategyExecutor.Executor(fileNames=["ACC.csv", "APOLLOTYRE.csv", "ASIANPAINT.csv"], is_short=True, caller=__file__, func=mymethod)

    executor.run(plotResults=True, uploadResults=False)


if __name__ == '__main__':
    main()
