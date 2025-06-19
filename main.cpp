#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include "velopack/include/Velopack.hpp"

static void update_app()
{
    try
    {
        Velopack::UpdateManager manager("https://coollab-art.github.io/Launcher-Velopack-Tests");
        std::wcout << L"Checking for updates..." << std::endl;

        auto updInfo = manager.CheckForUpdates();
        if (!updInfo.has_value())
        {
            std::wcout << L"No updates found." << std::endl;
            return;
        }

        std::wcout << L"Update found: " << updInfo->TargetFullRelease.Version.c_str() << std::endl;

        manager.DownloadUpdates(updInfo.value());
        std::wcout << L"Downloaded update." << std::endl;

        manager.WaitExitThenApplyUpdates(updInfo.value());
        std::wcout << L"Applying update..." << std::endl;

        exit(0); // Terminate so Velopack can restart
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[EXCEPTION] " << ex.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "[EXCEPTION] Unknown exception\n";
    }
}

void LoggerCallback(void* /*user_data*/, const char* level, const char* message)
{
    std::ofstream log("velopack_log.txt", std::ios::app);
    if (log.is_open())
        log << "[" << level << "] " << message << std::endl;
}

int wmain(int argc, wchar_t* argv[], wchar_t* /*envp*/[])
{
    Velopack::VelopackApp::Build()
        .SetLogger(LoggerCallback, nullptr)
        .Run();

    std::wcout << L"App launched with arguments: ";
    for (int i = 0; i < argc; ++i)
    {
        std::wcout << argv[i] << L" ";
    }
    std::wcout << std::endl;

    std::wcout << L"Running version: 1.0.9\n"
               << std::endl;

    update_app();
    return 0;
}
