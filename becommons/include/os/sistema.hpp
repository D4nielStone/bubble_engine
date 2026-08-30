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

#include "becommons_namespace.hpp"
#include "depuracao/debug.hpp"

#if defined(__MSYS__) || defined(__MINGW32__) || defined(__MINGW64__)
    #define BECOMMONS_UNIX_ENVIRONMENT
#elif defined(_WIN32) || defined(_WIN64)
    #define BECOMMONS_WINDOWS_NATIVE
#endif

#ifdef BECOMMONS_WINDOWS_NATIVE
    #include <windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

namespace BECOMMONS_NS {

inline static bool executarComando(const std::string& comando) {
    int result = std::system(comando.c_str());
    return result == 0;
}

inline static void abrirLink(const std::string& url) {
    depuracao::emitir(info, "Abrindo ajuda...");

#ifdef BECOMMONS_WINDOWS_NATIVE
    std::string comando = "start \"\" \"" + url + "\"";
#else
    std::string comando = "xdg-open \"" + url + "\" > /dev/null 2>&1 &";
#endif

    executarComando(comando);
}

inline static std::string obterExecDir() {
#ifdef BECOMMONS_WINDOWS_NATIVE

    std::vector<char> buffer(32768);

    DWORD tamanho = GetModuleFileNameA(
        nullptr,
        buffer.data(),
        static_cast<DWORD>(buffer.size())
    );

    if (tamanho == 0) {
        return "";
    }

    std::string exePath(buffer.data(), tamanho);

    size_t lastSlash = exePath.find_last_of("\\/");

    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }

#else

    std::vector<char> buffer(PATH_MAX);

    ssize_t tamanho = 1;

    if (tamanho <= 0) {
        return "";
    }

    buffer[tamanho] = '\0';

    std::string exePath(buffer.data());

    size_t lastSlash = exePath.find_last_of('/');

    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }

#endif

    return "";
}

inline static bool comandoDisponivel(const std::string& comando) {
#ifdef BECOMMONS_WINDOWS_NATIVE
    std::string check = "where \"" + comando + "\" >nul 2>&1";
#else
    std::string check =
        "command -v \"" + comando + "\" > /dev/null 2>&1";
#endif

    return executarComando(check);
}

inline static std::string obterEDT() {
    const char* envEditor = std::getenv("EDITOR");

    if (envEditor && *envEditor) {
        return envEditor;
    }

    const char* envVisual = std::getenv("VISUAL");

    if (envVisual && *envVisual) {
        return envVisual;
    }

    if (comandoDisponivel("nvim")) {
        return "nvim";
    }

    if (comandoDisponivel("code")) {
        return "code --wait";
    }

    if (comandoDisponivel("subl")) {
        return "subl --wait";
    }

    if (comandoDisponivel("vim")) {
        return "vim";
    }

#ifdef BECOMMONS_WINDOWS_NATIVE

    if (comandoDisponivel("notepad")) {
        return "notepad";
    }

    return "notepad";

#else

    if (comandoDisponivel("nano")) {
        return "nano";
    }

    if (comandoDisponivel("vi")) {
        return "vi";
    }

    return "";
#endif
}

inline static bool terminalDisponivel(const std::string& terminal) {
    return comandoDisponivel(terminal);
}

inline static std::string obterTerminal() {

#ifdef BECOMMONS_WINDOWS_NATIVE

    if (terminalDisponivel("wt")) {
        return "wt";
    }

    if (terminalDisponivel("powershell")) {
        return "powershell";
    }

    if (terminalDisponivel("cmd")) {
        return "cmd";
    }

    return "";

#else

    if (terminalDisponivel("gnome-terminal")) {
        return "gnome-terminal";
    }

    if (terminalDisponivel("konsole")) {
        return "konsole";
    }

    if (terminalDisponivel("xfce4-terminal")) {
        return "xfce4-terminal";
    }

    if (terminalDisponivel("mate-terminal")) {
        return "mate-terminal";
    }

    if (terminalDisponivel("lxterminal")) {
        return "lxterminal";
    }

    if (terminalDisponivel("xterm")) {
        return "xterm";
    }

    if (terminalDisponivel("alacritty")) {
        return "alacritty";
    }

    return "";
#endif
}

inline static void abrirNoTerminal(
    const std::string& editor,
    const std::string& arquivo
) {
    std::string terminal = obterTerminal();

    if (terminal.empty()) {
        std::cerr
            << "Nenhum terminal suportado encontrado.\n";

        return;
    }

    std::string comando;

#ifdef BECOMMONS_WINDOWS_NATIVE

    if (terminal == "wt") {
        comando =
            "wt new-tab "
            + editor
            + " \""
            + arquivo
            + "\"";
    }
    else if (terminal == "powershell") {
        comando =
            "start \"\" powershell "
            "-NoExit "
            "-Command \"& "
            + editor
            + " '"
            + arquivo
            + "'\"";
    }
    else if (terminal == "cmd") {
        comando =
            "start \"\" cmd /k \""
            + editor
            + " \\\""
            + arquivo
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
            + arquivo
            + "\"";
    }
    else {
        comando =
            terminal
            + " -- bash -c '"
            + editor
            + " \""
            + arquivo
            + "\"; exec bash'";
    }

#endif

    executarComando(comando);
}

} // namespace BECOMMONS_NS