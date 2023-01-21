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
