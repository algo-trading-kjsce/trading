import pandas as pd
import StrategyExecutor
import CalculatedData

def mymethod(dates, csv_data) -> CalculatedData.CalculatedData:

    final_data = CalculatedData.CalculatedData()

    for i in range(len(csv_data.datetime.dt.date.unique()) - 1):

        previous_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i]].reset_index(drop = True)
        current_day = csv_data[csv_data.datetime.dt.date.values == dates.Date[i+1]].reset_index(drop = True)

        if previous_day.close[len(previous_day)-1] < previous_day['20 DMA'][len(previous_day)-1]:
            pdh, pdl, pdc, cpr = StrategyExecutor.get_cpr(previous_day)

            flag = True

            for i in range(3):

                if (current_day.high.values[i] > pdh)&flag:

                    for j in range(i, i+4):

                        if (current_day.close.values[j] < pdh)&flag:

                            for k in range(j, 75):

                                if current_day.low.values[k] < current_day.low.values[j]:

                                    entry_time, exit_time, entry_price, exit_price, stop_loss, target, lowest_close = StrategyExecutor.get_entry_exit(current_day, k, j, pdh, cpr)

                                    final_data.m_EntryTimes.append(entry_time)
                                    final_data.m_EntryPrices.append(entry_price)

                                    final_data.m_ExitTimes.append(exit_time)
                                    final_data.m_ExitPrices.append(exit_price)

                                    final_data.m_PreviousDayHighs.append(pdh)

                                    final_data.m_StopLosses.append(stop_loss)

                                    final_data.m_Targets.append(target)

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
    executor = StrategyExecutor.Executor("C:/WS/Algo/src/StrategyExecutor/banknifty5min.csv", mymethod)

    executor.run()
    pass


if __name__ == '__main__':
    main()
