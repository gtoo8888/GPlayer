
# GPlayer
[Client](Client/GPlayer/README.md)<br/>
[Server](Server/GPlayerServer/README.md)<br/>

# Directory Structure
```bash
├─Client
│      GPlayer
│      GPlayerRemote      
└─Server
       GPlayerServer
```
# Usage

```bash
git clone git@github.com:gtoo8888/GPlayer.git
git submodule init
git submodule update 

# vim .git/config
# url = git@github.com:gtoo8888/GPlayer.git
# url = git@gitee.com:gtoo888/GPlayer.git
# url = git@192.168.1.25:cppproject/GPlayer.git
```

# Technology Stack
## Client：GPlayer
1. build
   1. VS2017
   2. msvc2017_64
2. Qt5.9.9
3. 3rdParty
   1. cJSON
   2. spdlog
   3. Google Test

## Server：GPlayerServer
1. build
   1. CMake
   2. make or ninja
2. test
   1. Google Test
3. analyzer
   1. coverage
      1. gcovr
      2. lcov
   2. gprof 
   3. valgrind
4. 3rdParty
   1. spdlog
        
