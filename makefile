before_sleep:
	mkdir -p output
	gcc ae.c ae.h before_sleep_example.c -o output/before_sleep
file_event:
	mkdir -p output
	gcc ae.c ae.h file_event_example.c -o output/file_event
time_event:
	mkdir -p output
	gcc ae.c ae.h time_event_example.c -o output/time_event
clean:
	rm -rf output
