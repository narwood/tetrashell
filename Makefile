a5 = tetrashell modify rank check recover

tetrashell: tetris.h tetrashell.c
	gcc tetris.h tetrashell.c -o tetrashell -lm

modify: modify.c tetris.h
	gcc modify.c tetris.h -o modify

rank: rank.c tetris.h
	gcc rank.c tetris.h -o rank

check: check.c verify.o tetris.h
	gcc check.c verify.o tetris.h -o check

recover: recover.c tetris.h
	gcc recover.c tetris.h -o recover
