
MAKE = make

all:
	(cd src && $(MAKE))
	mv src/xdiamonds .

clean:
	(cd src && $(MAKE) clean)
	rm -f xdiamonds core

