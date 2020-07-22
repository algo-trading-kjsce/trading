import numpy as np
import pandas as pd
from datetime import timedelta
import matplotlib.pyplot as plt
pd.set_option("max_row", None)

import CalculatedData

def get_entry_exit(df: pd.DataFrame, index: int, ent: int, pdh: float, cpr: float):
    
    #stoploss is 0.1% above prev day high   
    entry_price = df.low.values[ent]
    entry_time = df.datetime.values[index]
    exit_price = df.open.values[-1]
    exit_time = df.datetime.values[-1]
        
    stop_loss = 1.001 * pdh
    oneR = entry_price-(pdh-entry_price)
        
    if cpr < entry_price:
        target = max(cpr,oneR)
    else:
        target = oneR
        
    lowest_close = entry_price

    i = 0

    for close in df.loc[index:]["close"]:
        if close >= stop_loss:
            exit_price = stop_loss
            exit_time = df.datetime.values[index + i]
            break

        elif close < lowest_close:
            lowest_close = close
            stop_loss = stop_loss - (entry_price - close)

        else:
            pass


        i+=1

    return entry_time, exit_time, entry_price, exit_price, stop_loss, target, lowest_close


def get_cpr(data):
    high = data.high.max()
    low = data.low.min()
    close = data.close.values[-1]
    return high, low, close, (high + low + close)/3


class Executor:
    def __init__(self, fileName: str, func):
        self.m_fileName = fileName
        self.m_func = func
        self.m_csv_data = None
        self.m_plot_data = pd.DataFrame()
        self.m_unique_dates = None

    def open_file(self):
        self.m_csv_data = pd.read_csv(self.m_fileName)
        self.m_csv_data.datetime = pd.to_datetime(self.m_csv_data.datetime)
        self.m_csv_data["20 DMA"] = self.m_csv_data.close.rolling(50).mean()

        self.m_unique_dates = pd.DataFrame({"Date": self.m_csv_data.datetime.dt.date.unique()})
        pass

    def run_func(self):
        calculatedData = self.m_func(self.m_unique_dates, self.m_csv_data)

        self.m_plot_data = pd.DataFrame({"entry_time": calculatedData.m_EntryTimes,
                                  "entry_price": calculatedData.m_EntryPrices,
                                  "exit_time": calculatedData.m_ExitTimes,
                                  "exit_price": calculatedData.m_ExitPrices,
                                  "prevdayhigh": calculatedData.m_PreviousDayHighs,
                                  "fc_open": calculatedData.m_CurrentDayOpen,
                                  "fc_high": calculatedData.m_CurrentDayHigh,
                                  "ec_low": calculatedData.m_CurrentDayLow,
                                  "ec_close": calculatedData.m_CurrentDayClose,
                                  "Condition1": calculatedData.m_Condition1,
                                  "Condition2": calculatedData.m_Condition2,
                                  "Condition3": calculatedData.m_Condition3,
                                  "Target": calculatedData.m_Targets,
                                  "Lowest_Close": calculatedData.m_LowestClose,
                                  "Stop_Loss": calculatedData.m_StopLosses})

        self.m_plot_data['profit'] = self.m_plot_data.entry_price > self.m_plot_data.exit_price

        self.m_plot_data['pc_change'] = 1.0 - (self.m_plot_data['exit_price'] / self.m_plot_data['entry_price'])

        self.m_plot_data.entry_time = pd.to_datetime(self.m_plot_data.entry_time)
        pass


    def plot_results(self):
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
        
        # equity_curve['Net Pos'] = net_pos
        # equity_curve['Daily Profit'] = profit

        plt.plot(equity_curve.date, net_pos)
        plt.plot(equity_curve.date, profit)
        plt.title('python madarchod')

        plt.draw()
        plt.show()


        pass


    def run(self):
        self.open_file()
        self.run_func()
        self.plot_results()