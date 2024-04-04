all: packed_fs/pack include/packed_fs.h

packed_fs/pack: packed_fs/pack.c
	cc -o ./packed_fs/pack packed_fs/pack.c

include/packed_fs.h: packed_fs/pack packed_fs/files/*
	cd packed_fs/files && ../pack * > ../../include/packed_fs.h && cd ../..

clean: 
	rm -f packed_fs/pack include/packed_fs.h