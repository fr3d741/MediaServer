#include <WinFSWatcher.h>

#include <CommonDefines.h>
#include <JsonNode.h>

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <stop_token>

using namespace File_System;

void WinFSWatcher::WatchDirectories(Logging::ILogger::Ptr logger, const std::vector<std::string>& paths, IMessageQueue::Ptr queue)
{
    static std::jthread worker_thread;

    worker_thread.request_stop();
    if (worker_thread.joinable())
        worker_thread.join();

    if (paths.empty())
        return;

    worker_thread = std::jthread([=](std::stop_token token)
        {
            DWORD dwWaitStatus;
            std::vector<HANDLE> dwChangeHandles;
            dwChangeHandles.reserve(paths.size());

            // Watch the directory for file creation and deletion. 

            for (auto path : paths) {
                dwChangeHandles.push_back(
                    FindFirstChangeNotificationA(path.c_str(),                         // directory to watch 
                        TRUE,                         // do not watch subtree 
                        FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME)); // watch file name changes 
            }

            // Make a final validation check on our handles.

            if (std::any_of(dwChangeHandles.begin(), dwChangeHandles.end(), [](HANDLE h) { return h == NULL; }))
            {
                printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
                ExitProcess(GetLastError());
            }

            // Change notification is set. Now wait on both notification 
            // handles and refresh accordingly. 

            while (token.stop_requested() == false)
            {
                // Wait for notification.

                dwWaitStatus = WaitForMultipleObjects(static_cast<DWORD>(dwChangeHandles.size()), dwChangeHandles.data(), FALSE, 1000);

                switch (dwWaitStatus)
                {
                case WAIT_TIMEOUT:

                    // A timeout occurred, this would happen if some value other 
                    // than INFINITE is used in the Wait call and no changes occur.
                    // In a single-threaded environment you might not want an
                    // INFINITE wait.
                    break;

                case WAIT_OBJECT_0:
                default:
                    // A file was created, renamed, or deleted in the directory.
                    // Refresh this directory and restart the notification.
                    auto json = JsonNode::Create(logger);
                    json->Add(KeyWords(Keys::NodeType), static_cast<int>(Tags::Path_Update));
                    json->Add(TagWords(Tags::Path_Update), paths.at(dwWaitStatus));
                    queue->Add(json->ToString());
                    if (FindNextChangeNotification(dwChangeHandles[dwWaitStatus]) == FALSE)
                    {
                        printf("\n ERROR: FindNextChangeNotification function failed.\n");
                        ExitProcess(GetLastError());
                    }
                    break;
                }
            }
        });
}

/*void WinFSWatcher::WatchDirectories(std::list<std::string>& paths)
{
    char const* cc = const_cast<char*>(dr);
    auto lpDir = dr;
    DWORD dwWaitStatus;
    std::vector<HANDLE> dwChangeHandles(paths.size());
    std::vector<char> lpD; lpD.reserve(4);
    char* lpDrive = lpD.data();
    std::vector<char> lpFile;
    lpFile.reserve(_MAX_FNAME);
    std::vector<char> lpExt;
    lpExt.reserve(_MAX_EXT);

    _splitpath_s(cc, lpDrive, 4, nullptr, 0, lpFile.data(), _MAX_FNAME, lpExt.data(), _MAX_EXT);

    lpDrive[2] = (TCHAR)'\\';
    lpDrive[3] = (TCHAR)'\0';

    // Watch the directory for file creation and deletion. 

    dwChangeHandles[0] = FindFirstChangeNotificationA(
        lpDir,                         // directory to watch 
        FALSE,                         // do not watch subtree 
        FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes 

    if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
    {
        printf("\n ERROR: FindFirstChangeNotification function failed.\n");
        ExitProcess(GetLastError());
    }

    // Watch the subtree for directory creation and deletion. 

    dwChangeHandles[1] = FindFirstChangeNotificationA(
        lpDrive,                       // directory to watch 
        TRUE,                          // watch the subtree 
        FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes 

    if (dwChangeHandles[1] == INVALID_HANDLE_VALUE)
    {
        printf("\n ERROR: FindFirstChangeNotification function failed.\n");
        ExitProcess(GetLastError());
    }


    // Make a final validation check on our handles.

    if ((dwChangeHandles[0] == NULL) || (dwChangeHandles[1] == NULL))
    {
        printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
        ExitProcess(GetLastError());
    }

    // Change notification is set. Now wait on both notification 
    // handles and refresh accordingly. 

    while (TRUE)
    {
        // Wait for notification.

        printf("\nWaiting for notification...\n");

        dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles,
            FALSE, INFINITE);

        switch (dwWaitStatus)
        {
        case WAIT_OBJECT_0:

            // A file was created, renamed, or deleted in the directory.
            // Refresh this directory and restart the notification.

            RefreshDirectory(lpDir);
            if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
            {
                printf("\n ERROR: FindNextChangeNotification function failed.\n");
                ExitProcess(GetLastError());
            }
            break;

        case WAIT_OBJECT_0 + 1:

            // A directory was created, renamed, or deleted.
            // Refresh the tree and restart the notification.

            RefreshTree(lpDrive);
            if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
            {
                printf("\n ERROR: FindNextChangeNotification function failed.\n");
                ExitProcess(GetLastError());
            }
            break;

        case WAIT_TIMEOUT:

            // A timeout occurred, this would happen if some value other 
            // than INFINITE is used in the Wait call and no changes occur.
            // In a single-threaded environment you might not want an
            // INFINITE wait.

            printf("\nNo changes in the timeout period.\n");
            break;

        default:
            printf("\n ERROR: Unhandled dwWaitStatus.\n");
            ExitProcess(GetLastError());
            break;
        }
    }
}*/
