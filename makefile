compile: rsctrl.c writing.c
	gcc rsctrl.c -o rsctrl
	gcc writing.c -o writing

clean:
	rm -f rsctrl
	rm -f writing
	rm -f story.txt
	rm -rf *~
