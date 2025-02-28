# CS3502-Project
CS 3502: Project 1 - Multi-Threaded Programming and IPC

**Note**: These instructions are intended for users of Windows 11, these are the steps I followed in setting up my environment to work on this project. It is here to inform you of the environment I used to get everything working.

**Windows Subsystem for Linux (WSL) installed:**  I used WSL, you can follow the official Microsoft documentation to install WSL on your Windows machine. I used WSL2 version 2.4.11.0. It can be installed through the Microsoft Store
**Ubuntu installed within WSL:** During WSL installation, may be prompted to choose a distribution. Select Ubuntu. If not, it can be found on the Windows Store.


1.  **Launch Ubuntu WSL:** Open your terminal and type `wsl` to start your Ubuntu environment. Alternatively, look for the WSL executable which should be available on your computer now.

2.  **Install C/C++ Compiler and Build Tools:**

    Use:
    ```bash
    sudo apt update
    sudo apt install build-essential gdb
    ```
    To installo `gcc`, `g++` (C++ compiler), `make`, and `gdb` (GNU Debugger for compiling and debugging C/C++ code.
3. **Install Firefox** (optional but recommended as step 4 assumes you will have this installed.)
    Run:
   ```bash
   sudo apt install firefox
   ```
   To install Firefox in the Linux environment. 
5.  **Install Visual Studio Code (VS Code)** Download and install VS Code from the official website: [https://code.visualstudio.com/](https://code.visualstudio.com/)
       * Alternatively this also worked for me on one of my setups:
       * ```bash
            sudo apt update
            sudo apt install wget gpg
            wget -O- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
            sudo install -D -o root -g root -m 644 packages.microsoft.gpg /etc/apt/keyrings/packages.microsoft.gpg
            sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list'
            rm -f packages.microsoft.gpg
            sudo apt update
            sudo apt install code
            ```

7.  **Install VS Code Remote - WSL Extension:**
   Open VS Code on Windows.
    Go to the Extensions view (`Ctrl+Shift+X` or click the Extensions icon in the Activity Bar).
    Search for "Remote - WSL" and install the extension by Microsoft.

8.  **Connect VS Code to WSL:**
    After installing the Remote - WSL extension. 
    Ctr+Shift+P then type "Remote-WSL: New Window". to open a new VS Code window connected to Ubuntu WSL environment.

9.  **Install C/C++ Extension Pack in VS Code (WSL):**
    In the VS Code window connected to WSL, go to the Extensions
    Search for "C/C++ Extension Pack" and install the extension by Microsoft.

10.  **Install Code Runner Extension in VS Code (WSL):**
    In the VS Code window connected to WSL, go to the Extensions
    Search for "Code Runner" and install the extension by Jun Han. This is just to simplify running and building code snippets and files directly within VS Code.

11.  **Optional: Install a Graphical Desktop Environment (XFCE) for WSL (if you need GUI applications):**

     -**Install VcXsrv X Server on Windows:** Download and install VcXsrv from [https://sourceforge.net/projects/vcxsrv/](https://sourceforge.net/projects/vcxsrv/).

     -**Configure VcXsrv:**
        *   Run XLaunch.
        *   Select "Multiple windows" or "One large window" display mode.
        *   Choose "Start no client".
        *   Under "Extra settings", check "Disable access control".
        *   Click "Save configuration" 
        *   Click "Finish"

        -**Install XFCE and related packages in Ubuntu WSL:**
        ```bash
        sudo apt update
        sudo apt install xfce4 xfce4-goodies
        ```

        -**Launch XFCE session:** In your Ubuntu WSL terminal, execute:
        ```bash
        export DISPLAY=:0
        xfce4-session
        ```
        If that doesn't work. I was able to have some success with this guide the first go-around https://www.youtube.com/watch?v=YxQMDBnrMws, however after a windows reset, I wasn't able to get this working on a second Linux setup. 
