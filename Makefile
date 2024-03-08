APP_NAME = test

all:$(APP_NAME)

CC = gcc

SDL_CONFIG  ?= sdl2-config
CFLAGS_SDL  ?= $(shell $(SDL_CONFIG) --cflags )
LDFLAGS_SDL ?= $(shell $(SDL_CONFIG) --libs )
LDFLAGS_SDL += -lopenal
CFLAGS_SDL  += -DLIBAUDIO

OBJS = Error.o Memory.o Main.o Dfile.o Zfile.o Xfile.o Misc.o Config.o db.o Font.o Color.o Video.o Screen.o \
    Input.o Keyboard.o Keys.o Timer.o Mouse.o Inp.o Recorder.o Window.o Regions.o Msg.o TextBox.o SysInit.o MouseMgr.o Fade.o Heap.o Cache.o Art.o Message.o\
    Random.o MainMenu.o CycleColors.o FontMgr.o Gmouse.o OptionMenu.o Sound.o GSound.o Credits.o SfxCache.o Acm.o Audio.o AudioF.o Ng.o Proto.o \
    Feat.o Skill.o StrParse.o Party.o CharEditor.o Trait.o Critter.o Perk.o GlobVar.o Item.o Obj.o Pip.o DlgBox.o Script.o IntLib.o ScrInter.o ScpCmd.o ScpIfc.o Map.o \
    WordMap.o LoadSaveGame.o Combat.o Ai.o Automap.o GMovie.o Ev.o Iface.o FileCopy.o Tile.o IfcMsg.o Inventory.o Use.o Anim.o Game.o Text.o Gr.o Action.o Export.o \
    Nevs.o EndGame.o GDialog.o Lipsync.o Dialog.o Elevator.o DataFile.o Pcx.o Dbg.o Widget.o Window1.o Region.o Mve.o Mov.o Sdf.o Gmem.o ScpGame.o Light.o

LDFLAGS += $(LDFLAGS_SDL)
LDFLAGS += -lz -lm

CFLAGS += $(CFLAGS_SDL)
CFLAGS += -Wall -g

Mov.o: Mov.c MovDecode10.c MovDecode8bpp.c MovDecode16bpp.c Mov.h
	$(CC) $(CFLAGS) -c Mov.c

$(APP_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(APP_NAME) $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o
	rm -f $(APP_NAME)

