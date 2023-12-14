# F2recoveryLinuxPort
Linux port of the Fallout 2 game. 


To work you have to add data files from original game,
 Directories:
    ./data/
    ./sound/

 Files:
    critter.dat
    master.dat
    patch000.dat
 
External libraries: zlib, SDL2, OpenAL

 --- Initial release ---
You can load saved game, but scripts are blocked (it doesn't decompiled fully yet ). It also crash by exit isometric map due to scripts
( if you block them, world map works ).

To compile type 'make'. To run type './test'. Compile rules are only prototype. Also code is not optimised, and part of it are cutted ( directx, keyboard multilingual ). 
Some of low level functions like keyboard is 'just done', and need to be rewrited in the future.



