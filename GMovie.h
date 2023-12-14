#pragma once

#define MOVIE_InterPlay_Logo		0
#define MOVIE_Intro			1
#define MOVIE_Elder_Speech		2
#define MOVIE_Vault_Suit_Recovery	3
#define MOVIE_Arroyo_Failure		4
#define MOVIE_Arroyo_Destroyed		5
#define MOVIE_Starting_the_Car		6
#define MOVIE_Family_Cartucci		7
#define MOVIE_Deathclaw_Leader		8
#define MOVIE_Tanker_to_Derrick		9
#define MOVIE_Prisoners_in_Enclave	10
#define MOVIE_Derrick_Exploding		11
#define MOVIE_Arroyo_Dream_1		12
#define MOVIE_Arroyo_Dream_2		13
#define MOVIE_Arroyo_Dream_3		14
#define MOVIE_Arroyo_Dream_4		15
#define MOVIE_Credits			16


// Play flags
#define MOVIE_FADE_IN		0x01
#define MOVIE_FADE_OUT		0x02
#define MOVIE_BGSND_CLOSE	0x04
#define MOVIE_BGSND_PAUSE	0x08

#define MOVIE_SUBS_FONT		101

int GMovieInit();
void GMovieClose();
int GMovieLoad( xFile_t *fh );
int GMovieSave( xFile_t *fh );
int GMoviePlay( int MovieId, int Flags );
void GMovieFade();
int GMovieEnabled( int MovieId );
int GMovieGetError();
char *GMovieCreateSubtitlesPath( char *Path );


