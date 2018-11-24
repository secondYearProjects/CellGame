## CellGame

#### 100% not  a cell game

## How to run
1) <a href="https://github.com/secondYearProjects/CellGame/releases"> Download </a> latest built binary of CellGame
2) unpackage archive and run next command to install dependencies (if you are running it first time)
```
./download-deps.sh
```
3) to run game use from directory where you have unpacked archive
```
./CellGame
```

## How to develop CellGame
1) download and install cocos2d-x
Follow the guide <a href="http://docs.cocos2d-x.org/cocos2d-x/en/installation/Linux.html"> here </a> for linux
2) clone CellGame repository
```
git clone https://github.com/secondYearProjects/CellGame
```
3) extract cocos2d archive in CellGame directory
```
sudo apt-get install p7zip
sudo apt-get install p7zip-full
7z e cocos2d.7z
```
4) use any IDE compatible with CMake or just use cocos2d-x console command: 
```
cocos build -p linux -debug
```

## Feel free to post issues, fork, etc.
