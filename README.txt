This archive contains the following:
./Tcp/TcpClient/<VS dir>
./Tcp/TcpServer/<VS dir>
./ThroughputEvaluation.pdf
./README.txt

./Tcp/TcpClient/<VS dir> contains the Visual Studio solution of the TCP Client
./Tcp/TcpServer/<VS dir> contains the Visual Studio solution of the TCP Server
./ThroughputEvaluation.pdf details the throughput evaluation

Extract the VS directories to any local directory by personal choice.

In order to compile each solution, they should be opened in Visual Studio 2017 and then be compiled in x64 Release mode.
Upon compilation completion, an x64 folder is generated in the <VS dir> of the corresponding solution.
Open it and go to the Release folder which contains an .exe file.

To run the .exe file, hold the shift button and right-click inside the Release directory, then open Windows PowerShell and issue the following command:
{filename}.exe {ip-address}

An example command looks like this:
TCPClient.exe 127.0.0.1

It is important to run the TCP Server first and then the TCP Client.