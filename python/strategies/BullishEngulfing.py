import pandas as pd

from utils import StrategyExecutor
from utils import CalculatedData

from typing import Tuple

def get_entry_exit(day_data: pd.DataFrame, index: int) -> Tuple[int, float, float, float, float]:

    entry_price = day_data.open.values[index + 2]
    entry_time = day_data.datetime.values[index + 2]

    exit_price = day_data.open.values[-3]
    exit_time = day_data.datetime.values[-3]

    stop_loss = (day_data.open.values[index + 1] + day_data.close.values[index + 1]) / 2.0

    for i in range(index + 4, len(day_data)):

        if (day_data.open.values[i] < day_data.close.values[i]):

            if (day_data.open.values[i] > stop_loss):

                stop_loss = day_data.open.values[i]

        else:
            if (stop_loss >= day_data.open.values[i]):
                exit_price = day_data.open.values[i]
                exit_time = day_data.datetime.values[i]
                break

    return i, entry_time, exit_time, entry_price, exit_price




def mymethod(dates, csv_data) -> CalculatedData.CalculatedData:

    final_data = CalculatedData.CalculatedData()

    for i in range(len(csv_data.datetime.dt.date.unique()) - 1):

        current_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i]].reset_index(drop=True)

        j = 4

        while j < (len(current_day) - 5):

            found = False

            if (current_day.open.values[j] > current_day.close.values[j]):

                if (current_day.open.values[j] >= current_day.open.values[j + 1]):

                    if (current_day.close.values[j] <= current_day.close.values[j + 1]):

                        found = True

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
                        pass

            if not found:
                j += 1

    return final_data





def main():
    executor = StrategyExecutor.Executor(fileNames=["./ACC.csv"], is_short=False, caller=__file__, func=mymethod)

    executor.run(plotResults=False, uploadResults=False)


if __name__ == '__main__':
    main()