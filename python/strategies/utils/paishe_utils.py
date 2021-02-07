import os.path
import tempfile

from sys import platform

temp_directory = os.path.join(tempfile.gettempdir(), 'Paishe')

download_directory = os.path.join(temp_directory, 'Downloads')

if not os.path.exists(download_directory):
    os.makedirs(download_directory)


def is_linux() -> bool:
    """
    Check if platform is linux based

    Returns:
    -------
    bool
        Flag to see if platform is linux
    """
    return platform == 'linux'


def create_file(path: str):
    """
    Create a file through terminal if it does not exist.
    Python is not able to create and open a file in Ubuntu

    Parameters:
    ----------
    path : str
        Full path of the file to be created
    """

    # This function is specific to linux distributions only, don't call it from Windows
    assert is_linux()

    os.system(f"mkdir -p {os.path.dirname(path)} && touch {path}")

    assert os.path.exists(path)


def get_download_dir() -> str:
    """
    Returns the directory where all the downloads will be done by the bot.

    Returns:
    -------
    str
        Download directory for bot.
    """

    return download_directory


def get_temp_dir() -> str:
    """
    Get temp directory for all work done by Strategy computations.

    Returns:
    -------
    str
        Temp directory for strategy computations.
    """

    return temp_directory
