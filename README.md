# Web Client
Web client written in plain c/c++ using windows socket apis (v2.2).
Programm makes request and shows response.
Main parts - Protocol independent networking(getaddrinfo), non blocking IO FIONBIO and select function.

## Usage From Cmd 
webClient hostname homepage files1 ...

## Usage From Visual Studio
Solution -> Properties -> Debugging -> Command Arguments and fill in arguments 
webClient hostname homepage files1 ...

### Example
hostname - www.facebook.com
homepage - /
files1 - example.pdf 