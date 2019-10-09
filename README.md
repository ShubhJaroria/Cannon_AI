# Cannon_AI

//Will work only in Ubuntu with python 2.X

//You can also play with bot if you provide an empty file as one of the bot.

Installation:

1.First unzip the CannonAI folder and open terminal inside that folder

2.Install dependencies for python using following command

  a)"pip install -r requirements.txt"
  
3.Install any chromium based browser(or chromium browser let say) for ubuntu.

  a)"sudo apt install -y chromium-browser"
  
  b)Also don't forget to set the path as default so that while accessing browser, it should run by command "chromium"

4.After doing all this, open three new terminals and run server in one and client in other 2
 
 a)python server.py 10000 -n 8 -m 8 -NC 2 -TL 150 -LOG server.log
  
 b)export PATH=$PATH:'/home/chrome_driver_directory'
 
 c)python client.py 0.0.0.0 10000 RandomPlayer.py -mode GUI
 
 d)python client.py 0.0.0.0 10000 ./player.sh
  
  
 Note:player.sh contains:
 
 "g++ -std=c++11 test2.cpp -o test2
 
 ./test2"
 
 
 For more details refer to Readme of CannonAI.
