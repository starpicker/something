target := simple_jpeg_httpd

$(target): $(target).c
	gcc -W -Wall -lpthread -o $@ $^

clean:
	-rm -f $(target)
