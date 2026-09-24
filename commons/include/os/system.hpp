/**
 * @copyright MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * ... (mantido o cabeçalho original de licença)
 */

#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "commons_namespace.hpp"
#include "debugging/debug.hpp"

#if defined(__MSYS__) || defined(__MINGW32__) || defined(__MINGW64__)
    #define COMMONS_UNIX_ENVIRONMENT
#elif defined(_WIN32) || defined(_WIN64)
    #define COMMONS_WINDOWS_NATIVE
#endif

#ifdef COMMONS_WINDOWS_NATIVE
    #include <windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

namespace COMMONS_NS {

inline static bool executarComando(const std::string& comando) {
    int result = std::system(comando.c_str());
    return result == 0;
}

inline static void abrirLink(const std::string& url) {
    debugging::emit(info, "Abrindo ajuda...");

#ifdef COMMONS_WINDOWS_NATIVE
    std::string comando = "start \"\" \"" + url + "\"";
#else
    std::string comando = "xdg-open \"" + url + "\" > /dev/null 2>&1 &";
#endif

    executarComando(comando);
}

inline static std::string getExecDir() {
#ifdef COMMONS_WINDOWS_NATIVE

    std::vector<char> buffer(32768);

    DWORD size = GetModuleFileNameA(
        nullptr,
        buffer.data(),
        static_cast<DWORD>(buffer.size())
    );

    if (size == 0) {
        return "";
    }

    std::string exePath(buffer.data(), size);

    size_t lastSlash = exePath.find_last_of("\\/");

    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }

#else

    std::vector<char> buffer(PATH_MAX);

    ssize_t size = 1;

    if (size <= 0) {
        return "";
    }

    buffer[size] = '\0';

    std::string exePath(buffer.data());

    size_t lastSlash = exePath.find_last_of('/');

    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }

#endif

    return "";
}

inline static bool comandoDispolevel(const std::string& comando) {
#ifdef COMMONS_WINDOWS_NATIVE
    std::string check = "where \"" + comando + "\" >nul 2>&1";
#else
    std::string check =
        "command -v \"" + comando + "\" > /dev/null 2>&1";
#endif

    return executarComando(check);
}

inline static std::string getEDT() {
    const char* envEditor = std::getenv("EDITOR");

    if (envEditor && *envEditor) {
        return envEditor;
    }

    const char* envVisual = std::getenv("VISUAL");

    if (envVisual && *envVisual) {
        return envVisual;
    }

    if (comandoDispolevel("nvim")) {
        return "nvim";
    }

    if (comandoDispolevel("code")) {
        return "code --wait";
    }

    if (comandoDispolevel("subl")) {
        return "subl --wait";
    }

    if (comandoDispolevel("vim")) {
        return "vim";
    }

#ifdef COMMONS_WINDOWS_NATIVE

    if (comandoDispolevel("notepad")) {
        return "notepad";
    }

    return "notepad";

#else

    if (comandoDispolevel("nano")) {
        return "nano";
    }

    if (comandoDispolevel("vi")) {
        return "vi";
    }

    return "";
#endif
}

inline static bool terminalDispolevel(const std::string& terminal) {
    return comandoDispolevel(terminal);
}

inline static std::string getTerminal() {

#ifdef COMMONS_WINDOWS_NATIVE

    if (terminalDispolevel("wt")) {
        return "wt";
    }

    if (terminalDispolevel("powershell")) {
        return "powershell";
    }

    if (terminalDispolevel("cmd")) {
        return "cmd";
    }

    return "";

#else

    if (terminalDispolevel("gnome-terminal")) {
        return "gnome-terminal";
    }

    if (terminalDispolevel("konsole")) {
        return "konsole";
    }

    if (terminalDispolevel("xfce4-terminal")) {
        return "xfce4-terminal";
    }

    if (terminalDispolevel("mate-terminal")) {
        return "mate-terminal";
    }

    if (terminalDispolevel("lxterminal")) {
        return "lxterminal";
    }

    if (terminalDispolevel("xterm")) {
        return "xterm";
    }

    if (terminalDispolevel("alacritty")) {
        return "alacritty";
    }

    return "";
#endif
}

inline static void abrirNoTerminal(
    const std::string& editor,
    const std::string& file
) {
    std::string terminal = getTerminal();

    if (terminal.empty()) {
        std::cerr
            << "Nenhum terminal suportado encontrado.\n";

        return;
    }

    std::string comando;

#ifdef COMMONS_WINDOWS_NATIVE

    if (terminal == "wt") {
        comando =
            "wt new-tab "
            + editor
            + " \""
            + file
            + "\"";
    }
    else if (terminal == "powershell") {
        comando =
            "start \"\" powershell "
            "-NoExit "
            "-Command \"& "
            + editor
            + " '"
            + file
            + "'\"";
    }
    else if (terminal == "cmd") {
        comando =
            "start \"\" cmd /k \""
            + editor
            + " \\\""
            + file
            + "\\\"\"";
    }

#else

    if (
        terminal == "xterm"
        || terminal == "alacritty"
    ) {
        comando =
            terminal
            + " -e "
            + editor
            + " \""
            + file
            + "\"";
    }
    else {
        comando =
            terminal
            + " -- bash -c '"
            + editor
            + " \""
            + file
            + "\"; exec bash'";
    }

#endif

    executarComando(comando);
}

} // namespace COMMONS_NS
