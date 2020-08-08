from __future__ import print_function
from typing import Tuple
from typing import List
import pickle
import os.path
import io
import tempfile
from googleapiclient.http import MediaIoBaseDownload
from googleapiclient.http import MediaFileUpload
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request

from sys import platform

# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/drive']

temp_directory = os.path.join(tempfile.gettempdir(), 'Paishe')

download_directory = os.path.join(temp_directory, 'Downloads')

if not os.path.exists(download_directory):
    os.makedirs(download_directory)

token_file = os.path.join(temp_directory, 'token.pickle')


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

    assert is_linux() #This function is specific to linux distributions only, don't call it from Windows

    os.system(f"mkdir -p {os.path.dirname(path)} && touch {path}")

    assert os.path.exists(path)




class bot(object):
    m_service = None

    @staticmethod
    def login():
        """
        Log in to Google drive using bot email by default. Any code change in this function needs to be reviewed by Ashwin.
        Credential stored in repo is used and login details are cached on machine which will be used as long as it is available
        in the temp directory.

        Returns:
        -------
        service
            Logged in service that can be used to query information about files and folders in the google drive.
        """

        if bot.m_service:
            pass

        creds = None

        if os.path.exists(token_file):

            with open(token_file, 'rb') as token:
                creds = pickle.load(token)

        # If there are no (valid) credentials available, let the user log in.
        if not creds or not creds.valid:

            if creds and creds.expired and creds.refresh_token:

                creds.refresh(Request())

            else:

                flow = InstalledAppFlow.from_client_secrets_file(os.path.join(os.path.dirname(__file__), 'credentials.json'), SCOPES)

                creds = flow.run_local_server(port=0)

            # Save the credentials for the next run

            if is_linux():
                create_file(token_file)

            with open(token_file, 'wb') as token:
                pickle.dump(creds, token)

        bot.m_service = build('drive', 'v3', credentials=creds)


    @staticmethod
    def is_live() -> bool:
        """
        Check if currently logged in.

        Returns:
        -------
        bool
            flag to check if logged in or not
        """
        return bot.m_service is not None


    @staticmethod
    def service():
        """
        Get the google drive login token.

        Returns:
        -------
        service
            Logged in token
        """

        return bot.m_service


def find_on_drive(name: str, is_file: bool) -> str:
    """
    Finds the ID of the file/folder in Google Drive for easy retrieval later on.

    Parameters:
    ----------
    name
        Name of the file without the full path

    is_file
        Flag to specify whether the name is of a file or folder

    Returns:
    -------
    str
        ID of the file/folder in Google Drive
    """

    if not bot.is_live():
        bot.login()

    finished_searching = False

    page_token = None

    file_Id = ''

    search_condition = "mimeType='application/vnd.google-apps.folder'"

    if is_file:
        search_condition = f"name='{name}'"

    while finished_searching is False:

        results = bot.service().files().list(q=search_condition,
                                            fields="nextPageToken, files(id, name)",
                                            pageToken=page_token).execute()

        for item in results.get('files', []):
            if item['name'] == name:
                file_Id = item['id']
                finished_searching = True
                break

        page_token = results.get('nextPageToken', None)

        if page_token is None:
            break

    return file_Id


def create_folder_on_drive(parentDirectoryName: str, name: str) -> str:
    """
    Creates folder on Google Drive under specified parent directory.

    Parameters:
    ----------
    parentDirectoryName
        Name of the parent directory under which this folder needs to be created. This is assumed to be already present on Google Drive

    name
        Name of the folder to be created

    Returns:
    -------
    str
        ID of the new folder created

    Raises:
    ------
        ModuleNotFoundError if specified parent directory not found
    """

    fileId = find_on_drive(parentDirectoryName, False)

    if fileId == '':
        raise ModuleNotFoundError()

    metadata = {
        'name': name,
        'mimeType': 'application/vnd.google-apps.folder',
        'parents': [fileId]
    }

    newFolder = bot.service().files().create(body=metadata, fields='id').execute()

    return newFolder.get('id')


def upload_file(parentFolderId: str, fileName: str) -> str:
    """
    Upload file to Google Drive.

    Parameters:
    ----------
    parentFolderId
        Google Drive Id of the folder in which the file needs to be uploaded

    fileName
        Full path of the file to be uploaded

    Returns:
    -------
    str
        Id of the file in Google Drive
    """

    metadata = {
        'name': os.path.basename(fileName),
        'parents': [parentFolderId]
    }

    media = MediaFileUpload(fileName)

    fileId = bot.service().files().create(body=metadata, media_body=media, fields='id').execute()

    return fileId.get('id')


def download_file(fileName: str) -> bool:
    """
    Downloads the file from Google drive.

    Parameters:
    ----------

    fileName
        Name of the file without the full path, just the name and extension

    Returns:
    -------

    bool
        Flag to check if success or not
    """

    file_Id = find_on_drive(name=fileName, is_file=True)

    if file_Id:

        results = bot.service().files().get_media(fileId=file_Id)

        try:
            download_file = os.path.join(download_directory, fileName)

            if is_linux():
                create_file(download_file)

            file_handle = io.FileIO(download_file, 'wb')

            downloader = MediaIoBaseDownload(file_handle, results)

            done = False

            while done is False:
                status, done = downloader.next_chunk()
                print("Downloaded {}%.".format(int(status.progress() * 100)))

        finally:

            file_handle.close()

        return True

    return False


def find_or_download_file(fileName: str) -> Tuple[int, str]:
    """
    Looks for the file on disk. If found, name will be returned.

    Parameters:
    ----------
    fileName : str
        File name, either full path or just the filename to be searched/downloaded.

    Returns:
    -------
    Tuple[int, str]

    int
        0 if file is missing, 1 if file was found on machine, 2 if file was downloaded.
    str
        path to the file that was either found on the computer or downloaded.
    """

    stem = os.path.basename(fileName)

    downloaded_file = os.path.join(download_directory, stem)

    if os.path.exists(fileName):

        return 1, fileName

    elif os.path.exists(downloaded_file):

        return 1, downloaded_file

    elif download_file(stem):

        return 2, downloaded_file

    return 0, None


def upload_results(strategyName: str, files: List):
    """
    Upload the results to Google drive folder 'Plot Results'

    Parameters:
    ----------

    strategyName
        Name of the current strategy

    files
        List of files to be uploaded
    """

    strategyFolderId = find_on_drive(strategyName, False)

    if strategyFolderId == '':
        strategyFolderId = create_folder_on_drive('Results', strategyName)

    for file in files:
        fileId = upload_file(strategyFolderId, file)

        assert fileId != ''


    


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
