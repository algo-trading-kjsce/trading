import robin_stocks as rs
from rh import rh_login


def price_history(ticker: str = None,
                  interval: str = '5minute',
                  span: str = 'hour',
                  bounds: str = '24_7'):

    with rh_login() as manager:
        result = rs.get_crypto_historicals(ticker, interval, span, bounds)

        print(result)


def price(ticker: str = None):

    with rh_login() as manager:
        result = rs.get_crypto_info(ticker)

        print(result)


if __name__ == "__main__":
    for t in ["BTC", "ETH"]:
        price_history(t)
        price(t)